// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::modules::preimager::{JsonPreimager, JsonTxPreimage};
use crate::modules::serializer::BinanceAminoSerializer;
use crate::modules::tx_builder::TxBuilder;
use crate::signature::BinanceSignature;
use crate::transaction::SignerInfo;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::{PublicKeyBytes, SignatureBytes};
use tw_coin_entry::common::compile_input::SingleSignaturePubkey;
use tw_coin_entry::error::SigningResult;
use tw_coin_entry::signing_output_error;
use tw_keypair::ecdsa::secp256k1;
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
        let public_key = secp256k1::PublicKey::try_from(public_key.as_slice())?;

        let unsigned_tx = TxBuilder::unsigned_tx_from_proto(coin, &input)?;
        let signed_tx = unsigned_tx.into_signed(SignerInfo {
            public_key,
            signature,
        });

        let encoded_tx = BinanceAminoSerializer::serialize_signed_tx(&signed_tx)?;

        Ok(Proto::SigningOutput {
            encoded: encoded_tx.into(),
            ..Proto::SigningOutput::default()
        })
    }
}
