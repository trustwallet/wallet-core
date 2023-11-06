// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::proto::ethermint;
use crate::public_key::secp256k1::prepare_secp256k1_public_key;
use crate::public_key::{CosmosPublicKey, ProtobufPublicKey};
use tw_coin_entry::coin_context::CoinContext;
use tw_keypair::tw;
use tw_keypair::KeyPairResult;
use tw_memory::Data;
use tw_proto::{google, to_any};

// TODO move to tw_ethermint blockchain
pub struct EthermintEthSecp256PublicKey {
    public_key: Data,
}

impl CosmosPublicKey for EthermintEthSecp256PublicKey {
    fn from_private_key(coin: &dyn CoinContext, private_key: &tw::PrivateKey) -> KeyPairResult<Self>
    where
        Self: Sized,
    {
        let public_key = private_key.get_public_key_by_type(coin.public_key_type())?;
        Ok(EthermintEthSecp256PublicKey {
            public_key: public_key.to_bytes(),
        })
    }

    fn from_bytes(coin: &dyn CoinContext, public_key_bytes: &[u8]) -> KeyPairResult<Self> {
        let public_key = prepare_secp256k1_public_key(coin, public_key_bytes)?;
        Ok(EthermintEthSecp256PublicKey { public_key })
    }

    fn to_bytes(&self) -> Data {
        self.public_key.clone()
    }
}

impl ProtobufPublicKey for EthermintEthSecp256PublicKey {
    fn to_proto(&self) -> google::protobuf::Any {
        let proto = ethermint::crypto::v1::ethsecp256k1::PubKey {
            key: self.public_key.clone(),
        };
        to_any(&proto)
    }
}
