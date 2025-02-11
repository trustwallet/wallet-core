// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::classic_address::ClassicAddress;
use crate::encode::{encode_tx, EncodeMode, TxEncoded};
use crate::transaction::RippleTransaction;
use serde_json::Value as Json;
use tw_coin_entry::error::prelude::*;
use tw_encoding::hex::as_hex::AsHex;
use tw_hash::sha2::sha512;
use tw_hash::H256;
use tw_keypair::ecdsa::secp256k1;
use tw_keypair::traits::{SigningKeyTrait, VerifyingKeyTrait};
use tw_memory::Data;

pub const NETWORK_PREFIX: [u8; 4] = [0x53, 0x54, 0x58, 0x00];

pub struct TxPreImage {
    pub unsigned_tx: Json,
    pub pre_image_tx_data: Data,
    pub hash_to_sign: H256,
}

pub struct TransactionSigner;

impl TransactionSigner {
    pub fn sign<Transaction: RippleTransaction>(
        tx: Transaction,
        private_key: &secp256k1::PrivateKey,
    ) -> SigningResult<Transaction> {
        let public_key = private_key.public();
        Self::check_signing_public_key(&tx, &public_key)?;
        Self::check_source_account(&tx, &public_key)?;

        if tx.common_types().txn_signature.is_some() {
            return SigningError::err(SigningErrorType::Error_signatures_count)
                .context("Transaction is signed already");
        }

        let TxPreImage { hash_to_sign, .. } = Self::pre_image(&tx)?;
        let signature = private_key
            .sign(hash_to_sign)
            .into_tw()
            .context("Error signing transaction")?;
        Self::compile_unchecked(tx, &signature)
    }

    pub fn pre_image<Transaction: RippleTransaction>(
        tx: &Transaction,
    ) -> SigningResult<TxPreImage> {
        let TxEncoded { json, encoded } = encode_tx(tx, EncodeMode::SigningOnly)?;
        let pre_image: Data = NETWORK_PREFIX.iter().copied().chain(encoded).collect();

        let hash512 = sha512(&pre_image);
        let hash256 = H256::try_from(&hash512[..H256::LEN]).expect("Expected a valid H512 length");
        Ok(TxPreImage {
            unsigned_tx: json,
            pre_image_tx_data: pre_image,
            hash_to_sign: hash256,
        })
    }

    /// Compiles `signature` into the `transaction` validating the signature.
    pub fn compile<Transaction: RippleTransaction>(
        tx: Transaction,
        signature: &secp256k1::Signature,
        public_key: &secp256k1::PublicKey,
    ) -> SigningResult<Transaction> {
        let TxPreImage { hash_to_sign, .. } = Self::pre_image(&tx)?;
        Self::check_source_account(&tx, public_key)?;
        Self::check_signing_public_key(&tx, public_key)?;

        if !public_key.verify(signature.to_verify_sig(), hash_to_sign) {
            return SigningError::err(SigningErrorType::Error_signing)
                .context("Error verifying the given signature");
        }

        Self::compile_unchecked(tx, signature)
    }

    /// Compiles `signature` into the `transaction` without signature validation.
    /// Should only be used at [`TransactionSigner::sign`].
    fn compile_unchecked<Transaction: RippleTransaction>(
        mut tx: Transaction,
        signature: &secp256k1::Signature,
    ) -> SigningResult<Transaction> {
        let der_signature = signature
            .to_der()
            .into_tw()
            .context("Error converting a secp256k1 signature to DER")?;

        // Set the signature.
        tx.common_types_mut().txn_signature = Some(AsHex(der_signature));
        Ok(tx)
    }

    /// Checks whether the given transaction has an expected source account (if provided).
    fn check_source_account<Transaction: RippleTransaction>(
        tx: &Transaction,
        public_key: &secp256k1::PublicKey,
    ) -> SigningResult<()> {
        let expected_account = tx
            .common_types()
            .account
            .as_ref()
            .or_tw_err(SigningErrorType::Error_invalid_params)
            .context("Source account is not provided")?
            .to_classic_address()
            .into_tw()
            .context("Internal: error converting an X address to Classic")?;

        let actual_account = ClassicAddress::with_public_key(public_key)
            .into_tw()
            .context("Error generating an account address from PrivateKey")?;
        if expected_account != actual_account {
            let error = format!(
                "Expected a private key of the account '{expected_account}', found for '{actual_account}'"
            );
            return SigningError::err(SigningErrorType::Error_missing_private_key).context(error);
        }

        Ok(())
    }

    /// Checks whether the given transaction has an expected signing public key (if provided).
    fn check_signing_public_key<Transaction: RippleTransaction>(
        tx: &Transaction,
        public_key: &secp256k1::PublicKey,
    ) -> SigningResult<()> {
        let expected_signing_pubkey = &tx
            .common_types()
            .signing_pub_key
            .as_ref()
            .or_tw_err(SigningErrorType::Error_invalid_params)
            .context("Signing public key is not provided")?
            .0;

        let actual_pubkey = public_key.compressed();
        if *expected_signing_pubkey != actual_pubkey {
            let error = format!(
                "Public key mismatch: actual signing pubkey '{actual_pubkey}', expected '{expected_signing_pubkey}'"
            );
            return SigningError::err(SigningErrorType::Error_missing_private_key).context(error);
        }

        Ok(())
    }
}
