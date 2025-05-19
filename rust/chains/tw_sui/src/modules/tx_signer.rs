// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::SuiAddress;
use crate::modules::intent::Intent;
use crate::signature::SuiSignatureInfo;
use crate::transaction::transaction_data::TransactionData;
use tw_coin_entry::error::prelude::*;
use tw_encoding::bcs;
use tw_hash::blake2::blake2_b;
use tw_hash::H256;
use tw_keypair::ed25519;
use tw_keypair::traits::{KeyPairTrait, SigningKeyTrait};
use tw_memory::Data;

pub struct TransactionPreimage {
    /// Transaction `bcs` encoded representation.
    pub unsigned_tx_data: Data,
    /// [`TransactionPreimage::unsigned_tx_data`] extended with the `IntentMessage`.
    pub tx_data_to_sign: Data,
    /// Hash of the [`TransactionPreimage::tx_data_to_sign`].
    pub tx_hash_to_sign: H256,
}

pub struct TxSigner;

impl TxSigner {
    pub fn sign(
        tx: &TransactionData,
        signer_key: &ed25519::sha512::KeyPair,
    ) -> SigningResult<(TransactionPreimage, SuiSignatureInfo)> {
        let public_key = signer_key.public();
        let signer_address = SuiAddress::with_ed25519_pubkey(public_key)?;
        if signer_address != tx.sender() {
            return SigningError::err(SigningErrorType::Error_missing_private_key)
                .context("Given private key does not belong to the sender address");
        }

        let unsigned_tx_data = bcs::encode(tx)
            .tw_err(SigningErrorType::Error_internal)
            .context("Error serializing TransactionData")?;
        Self::sign_direct(unsigned_tx_data, signer_key)
    }

    pub fn sign_direct(
        unsigned_tx_data: Data,
        signer_key: &ed25519::sha512::KeyPair,
    ) -> SigningResult<(TransactionPreimage, SuiSignatureInfo)> {
        let preimage = Self::preimage_direct(unsigned_tx_data)?;
        let signature = signer_key.sign(preimage.tx_hash_to_sign.into_vec())?;
        let signature_info = SuiSignatureInfo::ed25519(&signature, signer_key.public());
        Ok((preimage, signature_info))
    }

    pub fn preimage(tx: &TransactionData) -> SigningResult<TransactionPreimage> {
        let unsigned_tx_data = bcs::encode(tx)
            .tw_err(SigningErrorType::Error_internal)
            .context("Error serializing TransactionData")?;
        Self::preimage_direct(unsigned_tx_data)
    }

    pub fn preimage_direct(unsigned_tx_data: Data) -> SigningResult<TransactionPreimage> {
        let intent = Intent::sui_transaction();
        let intent_data = bcs::encode(&intent)
            .tw_err(SigningErrorType::Error_internal)
            .context("Error serializing Intent message")?;

        let tx_data_to_sign: Data = intent_data
            .into_iter()
            .chain(unsigned_tx_data.iter().copied())
            .collect();
        let tx_hash_to_sign = blake2_b(&tx_data_to_sign, H256::LEN)
            .and_then(|hash| H256::try_from(hash.as_slice()))
            .tw_err(SigningErrorType::Error_internal)?;

        Ok(TransactionPreimage {
            unsigned_tx_data,
            tx_data_to_sign,
            tx_hash_to_sign,
        })
    }
}
