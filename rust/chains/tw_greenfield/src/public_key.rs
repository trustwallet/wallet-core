// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::coin_context::CoinContext;
use tw_cosmos_sdk::public_key::{CosmosPublicKey, JsonPublicKey, ProtobufPublicKey};
use tw_keypair::ecdsa::secp256k1;
use tw_keypair::tw::{PrivateKey, PublicKeyType};
use tw_keypair::{KeyPairError, KeyPairResult};
use tw_memory::Data;
use tw_proto::{google, to_any};

pub struct GreenfieldPublicKey(pub secp256k1::PublicKey);

impl JsonPublicKey for GreenfieldPublicKey {
    fn public_key_type(&self) -> String {
        "/cosmos.crypto.eth.ethsecp256k1.PubKey".to_string()
    }
}

impl ProtobufPublicKey for GreenfieldPublicKey {
    fn to_proto(&self) -> google::protobuf::Any {
        let proto = tw_cosmos_sdk::proto::cosmos::crypto::eth::ethsecp256k1::PubKey {
            key: self.0.compressed().to_vec(),
        };
        to_any(&proto)
    }
}

impl CosmosPublicKey for GreenfieldPublicKey {
    fn from_private_key(_coin: &dyn CoinContext, private_key: &PrivateKey) -> KeyPairResult<Self> {
        let public_key = private_key
            .get_public_key_by_type(PublicKeyType::Secp256k1)?
            .to_secp256k1()
            .ok_or(KeyPairError::InvalidPublicKey)?
            .clone();
        Ok(GreenfieldPublicKey(public_key))
    }

    fn from_bytes(_coin: &dyn CoinContext, public_key_bytes: &[u8]) -> KeyPairResult<Self> {
        secp256k1::PublicKey::try_from(public_key_bytes).map(GreenfieldPublicKey)
    }

    fn to_bytes(&self) -> Data {
        self.0.compressed().to_vec()
    }
}
