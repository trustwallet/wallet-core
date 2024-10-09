// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::{PublicKeyBytes, SignatureBytes};
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::signing_output_error;
use tw_keypair::ed25519;
use tw_proto::Pactus::Proto;
use tw_proto::TxCompiler::Proto as CompilerProto;

use crate::transaction::Transaction;

pub struct PactusCompiler;

impl PactusCompiler {
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
        let trx = Transaction::from_proto(&input)?;
        let sign_bytes = trx.sign_bytes()?;

        let output = CompilerProto::PreSigningOutput {
            data_hash: trx.id().into(),
            data: sign_bytes.into(),
            ..CompilerProto::PreSigningOutput::default()
        };

        Ok(output)
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
        let signature_bytes = signatures
            .first()
            .or_tw_err(SigningErrorType::Error_signatures_count)?;
        let public_key_bytes = public_keys
            .first()
            .or_tw_err(SigningErrorType::Error_signatures_count)?;

        let public_key = ed25519::sha512::PublicKey::try_from(public_key_bytes.as_slice())?;
        let signature = ed25519::Signature::try_from(signature_bytes.as_slice())?;

        let mut trx = Transaction::from_proto(&input)?;
        trx.set_signatory(public_key.to_owned(), signature.to_owned());

        let data = trx.to_bytes()?;

        let output = Proto::SigningOutput {
            transaction_id: trx.id().into(),
            signed_transaction_data: data.into(),
            signature: signature.to_bytes().to_vec().into(),
            ..Proto::SigningOutput::default()
        };

        Ok(output)
    }
}
