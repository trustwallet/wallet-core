// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::modules::tx_builder::{TWTransaction, TWTransactionBuilder};
use crate::modules::tx_signer::{TransactionPreimage, TxSigner};
use crate::signature::SuiSignatureInfo;
use std::borrow::Cow;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::{PublicKeyBytes, SignatureBytes};
use tw_coin_entry::common::compile_input::SingleSignaturePubkey;
use tw_coin_entry::error::SigningResult;
use tw_coin_entry::signing_output_error;
use tw_encoding::base64;
use tw_keypair::ed25519;
use tw_proto::Sui::Proto;
use tw_proto::TxCompiler::Proto as CompilerProto;

pub struct SuiCompiler;

impl SuiCompiler {
    #[inline]
    pub fn preimage_hashes(
        coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
    ) -> CompilerProto::PreSigningOutput<'static> {
        Self::preimage_hashes_impl(coin, input)
            .unwrap_or_else(|e| signing_output_error!(CompilerProto::PreSigningOutput, e))
    }

    fn preimage_hashes_impl(
        _coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
    ) -> SigningResult<CompilerProto::PreSigningOutput<'static>> {
        let builder = TWTransactionBuilder::new(input);
        let tx_to_sign = builder.build()?;

        let TransactionPreimage {
            tx_data_to_sign,
            tx_hash_to_sign,
            ..
        } = match tx_to_sign {
            TWTransaction::Transaction(tx) => TxSigner::preimage(&tx)?,
            TWTransaction::SignDirect(tx_data) => TxSigner::preimage_direct(tx_data)?,
        };

        Ok(CompilerProto::PreSigningOutput {
            data: Cow::from(tx_data_to_sign),
            data_hash: Cow::from(tx_hash_to_sign.to_vec()),
            ..CompilerProto::PreSigningOutput::default()
        })
    }

    #[inline]
    pub fn compile(
        coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
        signatures: Vec<SignatureBytes>,
        public_keys: Vec<PublicKeyBytes>,
    ) -> Proto::SigningOutput<'static> {
        Self::compile_impl(coin, input, signatures, public_keys)
            .unwrap_or_else(|e| signing_output_error!(Proto::SigningOutput, e))
    }

    fn compile_impl(
        _coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
        signatures: Vec<SignatureBytes>,
        public_keys: Vec<PublicKeyBytes>,
    ) -> SigningResult<Proto::SigningOutput<'static>> {
        let builder = TWTransactionBuilder::new(input);
        let tx_to_sign = builder.build()?;

        let TransactionPreimage {
            unsigned_tx_data, ..
        } = match tx_to_sign {
            TWTransaction::Transaction(tx) => TxSigner::preimage(&tx),
            TWTransaction::SignDirect(tx_data) => TxSigner::preimage_direct(tx_data),
        }?;

        let SingleSignaturePubkey {
            signature: raw_signature,
            public_key: public_key_bytes,
        } = SingleSignaturePubkey::from_sign_pubkey_list(signatures, public_keys)?;

        let signature = ed25519::Signature::try_from(raw_signature.as_slice())?;
        let public_key = ed25519::sha512::PublicKey::try_from(public_key_bytes.as_slice())?;

        let signature_info = SuiSignatureInfo::ed25519(&signature, &public_key);

        let is_url = false;
        let unsigned_tx = base64::encode(&unsigned_tx_data, is_url);
        Ok(Proto::SigningOutput {
            unsigned_tx: Cow::from(unsigned_tx),
            signature: Cow::from(signature_info.to_base64()),
            ..Proto::SigningOutput::default()
        })
    }
}
