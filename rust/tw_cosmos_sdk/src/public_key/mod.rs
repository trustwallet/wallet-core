// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::coin_context::CoinContext;
use tw_keypair::{tw, KeyPairResult};
use tw_memory::Data;
use tw_proto::google;

pub mod secp256k1;

pub struct PublicKeyParams {
    pub public_key_type: tw::PublicKeyType,
    pub json_type: String,
    pub protobuf_type_url: String,
}

pub trait CosmosPublicKey: JsonPublicKey + ProtobufPublicKey + Sized {
    fn from_private_key(
        coin: &dyn CoinContext,
        private_key: &tw::PrivateKey,
        params: Option<PublicKeyParams>,
    ) -> KeyPairResult<Self> {
        let public_key_type = match params {
            Some(ref params) => params.public_key_type,
            None => coin.public_key_type(),
        };
        let public_key = private_key.get_public_key_by_type(public_key_type)?;
        Self::from_bytes(coin, &public_key.to_bytes(), params)
    }

    fn from_bytes(
        coin: &dyn CoinContext,
        public_key_bytes: &[u8],
        params: Option<PublicKeyParams>,
    ) -> KeyPairResult<Self>;

    fn to_bytes(&self) -> Data;
}

pub trait ProtobufPublicKey {
    fn to_proto(&self) -> google::protobuf::Any;
}

pub trait JsonPublicKey {
    /// In most cases, [`JsonPublicKey::public_key_type`] returns a corresponding protobuf message type.
    fn public_key_type(&self) -> String;
}
