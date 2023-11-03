// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_coin_entry::coin_context::CoinContext;
use tw_keypair::{tw, KeyPairResult};
use tw_proto::google;

pub mod ethermint_eth_secp256k1;
pub mod injective_eth_secp256k1;
pub mod secp256k1;

pub trait CosmosPublicKey: Sized {
    fn from_private_key(
        coin: &dyn CoinContext,
        private_key: &tw::PrivateKey,
    ) -> KeyPairResult<Self>;

    fn from_bytes(coin: &dyn CoinContext, public_key_bytes: &[u8]) -> KeyPairResult<Self>;
}

pub trait ProtobufPublicKey: CosmosPublicKey {
    fn to_proto(&self) -> google::protobuf::Any;
}

// pub trait JsonPublicKey {
//     fn to_json(&self) -> Json;
// }
