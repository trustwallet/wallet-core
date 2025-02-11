// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::encode::{encode_tx, EncodeMode};
use crate::modules::protobuf_builder::ProtobufBuilder;
use crate::modules::transaction_signer::TransactionSigner;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::{PublicKeyBytes, SignatureBytes};
use tw_coin_entry::common::compile_input::SingleSignaturePubkey;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::signing_output_error;
use tw_keypair::ecdsa::secp256k1;
use tw_misc::traits::ToBytesVec;
use tw_proto::Ripple::Proto;
use tw_proto::TxCompiler::Proto as CompilerProto;

pub struct RippleCompiler;

impl RippleCompiler {
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
        let unsigned_tx = ProtobufBuilder::new(&input).build()?;
        let pre_image = TransactionSigner::pre_image(&unsigned_tx)?;

        Ok(CompilerProto::PreSigningOutput {
            data_hash: pre_image.hash_to_sign.to_vec().into(),
            data: pre_image.pre_image_tx_data.into(),
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
        let SingleSignaturePubkey {
            signature,
            public_key,
        } = SingleSignaturePubkey::from_sign_pubkey_list(signatures, public_keys)?;

        let signature = secp256k1::Signature::from_bytes(signature.as_slice())
            .into_tw()
            .context("Invalid signature")?;
        let public_key = secp256k1::PublicKey::try_from(public_key.as_slice())
            .into_tw()
            .context("Invalid public key")?;

        let unsigned_tx = ProtobufBuilder::new(&input).build()?;
        let signed_tx = TransactionSigner::compile(unsigned_tx, &signature, &public_key)?;

        let encoded = encode_tx(&signed_tx, EncodeMode::All)?.encoded;
        Ok(Proto::SigningOutput {
            encoded: encoded.into(),
            ..Proto::SigningOutput::default()
        })
    }
}
