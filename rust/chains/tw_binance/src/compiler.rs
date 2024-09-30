// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::context::BinanceContext;
use crate::modules::preimager::{JsonPreimager, JsonTxPreimage};
use crate::modules::serializer::BinanceAminoSerializer;
use crate::modules::tx_builder::TxBuilder;
use crate::signature::BinanceSignature;
use crate::transaction::SignerInfo;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::{PublicKeyBytes, SignatureBytes};
use tw_coin_entry::common::compile_input::SingleSignaturePubkey;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::signing_output_error;
use tw_cosmos_sdk::modules::serializer::json_serializer::JsonSerializer;
use tw_cosmos_sdk::public_key::secp256k1::Secp256PublicKey;
use tw_cosmos_sdk::public_key::CosmosPublicKey;
use tw_misc::traits::ToBytesVec;
use tw_proto::Binance::Proto;
use tw_proto::TxCompiler::Proto as CompilerProto;

pub struct BinanceCompiler;

impl BinanceCompiler {
    #[inline]
    pub fn preimage_hashes(
        coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
    ) -> CompilerProto::PreSigningOutput<'static> {
        Self::preimage_hashes_impl(coin, input)
            .unwrap_or_else(|e| signing_output_error!(CompilerProto::PreSigningOutput, e))
    }

    fn preimage_hashes_impl(
        coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
    ) -> SigningResult<CompilerProto::PreSigningOutput<'static>> {
        let unsigned_tx = TxBuilder::unsigned_tx_from_proto(coin, &input)?;
        let JsonTxPreimage {
            tx_hash,
            encoded_tx,
        } = JsonPreimager::preimage_hash(&unsigned_tx)?;

        Ok(CompilerProto::PreSigningOutput {
            data_hash: tx_hash.to_vec().into(),
            data: encoded_tx.as_bytes().to_vec().into(),
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
        coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
        signatures: Vec<SignatureBytes>,
        public_keys: Vec<PublicKeyBytes>,
    ) -> SigningResult<Proto::SigningOutput<'static>> {
        let SingleSignaturePubkey {
            signature,
            public_key,
        } = SingleSignaturePubkey::from_sign_pubkey_list(signatures, public_keys)?;
        let signature = BinanceSignature::try_from(signature.as_slice())?;
        let public_key_params = None;
        let public_key =
            Secp256PublicKey::from_bytes(coin, public_key.as_slice(), public_key_params)?;

        let signature_bytes = signature.to_vec();
        let signature_json = JsonSerializer::<BinanceContext>::serialize_signature(
            &public_key,
            signature_bytes.clone(),
        );

        let unsigned_tx = TxBuilder::unsigned_tx_from_proto(coin, &input)?;
        let signed_tx = unsigned_tx.into_signed(SignerInfo {
            public_key,
            signature,
        });

        let encoded_tx = BinanceAminoSerializer::serialize_signed_tx(&signed_tx)?;

        let signature_json =
            serde_json::to_string(&signature_json).tw_err(|_| SigningErrorType::Error_internal)?;
        Ok(Proto::SigningOutput {
            encoded: encoded_tx.into(),
            signature: signature_bytes.into(),
            signature_json: signature_json.into(),
            ..Proto::SigningOutput::default()
        })
    }
}
