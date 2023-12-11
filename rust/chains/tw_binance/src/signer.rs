// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::modules::serializer::BinanceAminoSerializer;
use crate::modules::tx_builder::TxBuilder;
use crate::signature::BinanceSignature;
use crate::transaction::{JsonTxPreimage, SignerInfo};
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
        let JsonTxPreimage { tx_hash, .. } = unsigned_tx.preimage_hash()?;

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

// TODO remove this
// #[cfg(test)]
// mod tests {
//     use tw_encoding::base64;
//     use tw_encoding::hex::ToHex;
//     use tw_proto::Binance::Proto;
//
//     #[test]
//     fn test_foo() {
//         let data = base64::decode("yAHwYl3uCkgqLIf6CiAKFJk0ZA2+bZFukAgypQeKGy9OlDTQEggKA0JOQhCQThIgChRbIX6Z1PR2ShqNpO4/Icbd15EDkBIICgNCTkIQkE4Sbwom61rphyEC0upE6hpxiiBQ5B1+VK3ACPEwGHwC79TkcM2xMhHFnEUSQIlv+iYpbdTZbWagqawAg6D8fNfljGaQXK+2N4JhlzwhReCTAojp8HAfy1oRrwnfDP+bzjrMPnkZhCOos+bHMCUY8u7VAxoHcGF5YmFjaw==", false).unwrap();
//         let tx: Proto::Transaction = tw_proto::deserialize(&data[6..]).unwrap();
//
//         println!("{tx:?}");
//
//         let sign: Proto::Signature = tw_proto::deserialize(&tx.signatures[0]).unwrap();
//         println!("Signature Proto: {sign:?}");
//         println!("Signature: {}", sign.signature.to_hex());
//
//         let msg: Proto::SendOrder = tw_proto::deserialize(&tx.msgs[0][3..]).unwrap();
//         println!("{msg:?}");
//     }
// }
