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
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::signing_output_error;
use tw_cosmos_sdk::modules::serializer::json_serializer::JsonSerializer;
use tw_cosmos_sdk::public_key::secp256k1::Secp256PublicKey;
use tw_keypair::ecdsa::secp256k1;
use tw_keypair::traits::{KeyPairTrait, SigningKeyTrait};
use tw_misc::traits::ToBytesVec;
use tw_proto::Binance::Proto;

pub struct BinanceSigner;

impl BinanceSigner {
    pub fn sign(
        coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
    ) -> Proto::SigningOutput<'static> {
        Self::sign_impl(coin, input)
            .unwrap_or_else(|e| signing_output_error!(Proto::SigningOutput, e))
    }

    fn sign_impl(
        coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
    ) -> SigningResult<Proto::SigningOutput<'static>> {
        let unsigned_tx = TxBuilder::unsigned_tx_from_proto(coin, &input)?;
        let JsonTxPreimage { tx_hash, .. } = JsonPreimager::preimage_hash(&unsigned_tx)?;

        let key_pair = secp256k1::KeyPair::try_from(input.private_key.as_ref())?;

        let signature = BinanceSignature::from(key_pair.sign(tx_hash)?);
        let public_key =
            Secp256PublicKey::from_secp256k1_public_key(coin.public_key_type(), key_pair.public())?;

        let signature_bytes = signature.to_vec();
        let signature_json = JsonSerializer::<BinanceContext>::serialize_signature(
            &public_key,
            signature_bytes.clone(),
        );

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
