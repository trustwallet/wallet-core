// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::coin_context::CoinContext;
use tw_keypair::{tw, KeyPairResult};
use tw_memory::Data;
use tw_proto::google;

pub mod secp256k1;

pub trait CosmosPublicKey: JsonPublicKey + ProtobufPublicKey + Sized {
    fn from_private_key(
        coin: &dyn CoinContext,
        private_key: &tw::PrivateKey,
    ) -> KeyPairResult<Self>;

    fn from_bytes(coin: &dyn CoinContext, public_key_bytes: &[u8]) -> KeyPairResult<Self>;

    fn to_bytes(&self) -> Data;
}

pub trait ProtobufPublicKey {
    fn to_proto(&self) -> google::protobuf::Any;
}

pub trait JsonPublicKey {
    /// In most cases, [`JsonPublicKey::public_key_type`] returns a corresponding protobuf message type.
    fn public_key_type(&self) -> String;
}
