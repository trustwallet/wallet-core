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
use tw_coin_entry::error::SigningResult;
use tw_coin_entry::signing_output_error;
use tw_keypair::ecdsa::secp256k1;
use tw_keypair::traits::{KeyPairTrait, SigningKeyTrait};
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
        let public_key = key_pair.public().clone();

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
