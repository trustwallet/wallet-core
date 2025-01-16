// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::proto::cosmos;
use crate::public_key::{CosmosPublicKey, JsonPublicKey, ProtobufPublicKey, PublicKeyParams};
use tw_coin_entry::coin_context::CoinContext;
use tw_keypair::ecdsa::secp256k1;
use tw_keypair::tw;
use tw_keypair::{KeyPairError, KeyPairResult};
use tw_memory::Data;
use tw_misc::traits::ToBytesVec;
use tw_proto::{google, to_any_with_type_url, type_url};

const DEFAULT_JSON_PUBLIC_KEY_TYPE: &str = "tendermint/PubKeySecp256k1";

#[derive(Clone)]
pub struct Secp256PublicKey {
    public_key: Data,
    json_type: String,
    protobuf_type_url: String,
}

impl Secp256PublicKey {
    pub fn from_secp256k1_public_key(
        public_key_type: tw::PublicKeyType,
        public_key: &secp256k1::PublicKey,
    ) -> KeyPairResult<Self> {
        let public_key =
            prepare_secp256k1_public_key(public_key_type, public_key.compressed().as_slice())?;
        Ok(Secp256PublicKey {
            public_key,
            json_type: DEFAULT_JSON_PUBLIC_KEY_TYPE.to_string(),
            protobuf_type_url: type_url::<cosmos::crypto::secp256k1::PubKey>(),
        })
    }
}

impl CosmosPublicKey for Secp256PublicKey {
    fn from_bytes(
        coin: &dyn CoinContext,
        public_key_bytes: &[u8],
        maybe_params: Option<PublicKeyParams>,
    ) -> KeyPairResult<Self> {
        let params = maybe_params.unwrap_or_else(|| PublicKeyParams {
            public_key_type: coin.public_key_type(),
            json_type: DEFAULT_JSON_PUBLIC_KEY_TYPE.to_string(),
            protobuf_type_url: type_url::<cosmos::crypto::secp256k1::PubKey>(),
        });

        let public_key = prepare_secp256k1_public_key(params.public_key_type, public_key_bytes)?;
        Ok(Secp256PublicKey {
            public_key,
            json_type: params.json_type,
            protobuf_type_url: params.protobuf_type_url,
        })
    }

    fn to_bytes(&self) -> Data {
        self.public_key.clone()
    }
}

impl ProtobufPublicKey for Secp256PublicKey {
    fn to_proto(&self) -> google::protobuf::Any {
        let proto = cosmos::crypto::secp256k1::PubKey {
            key: self.public_key.clone(),
        };
        to_any_with_type_url(&proto, self.protobuf_type_url.clone())
    }
}

impl JsonPublicKey for Secp256PublicKey {
    fn public_key_type(&self) -> String {
        self.json_type.clone()
    }
}

pub fn prepare_secp256k1_public_key(
    public_key_type: tw::PublicKeyType,
    public_key_bytes: &[u8],
) -> KeyPairResult<Data> {
    let public_key = secp256k1::PublicKey::try_from(public_key_bytes)?;
    match public_key_type {
        tw::PublicKeyType::Secp256k1 => Ok(public_key.compressed().to_vec()),
        tw::PublicKeyType::Secp256k1Extended => Ok(public_key.uncompressed().to_vec()),
        _ => Err(KeyPairError::InvalidPublicKey),
    }
}
