// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::proto::{cosmos, ethermint};
use crate::public_key::secp256k1::private_to_public_key_bytes;
use crate::public_key::ProtobufPublicKey;
use tw_keypair::ecdsa::secp256k1;
use tw_keypair::tw::PublicKeyType;
use tw_memory::Data;
use tw_proto::{google, to_any};

// TODO move to tw_ethermint blockchain
pub struct EthermintEthSecp256PublicKey {
    public_key: Data,
}

impl EthermintEthSecp256PublicKey {
    pub fn with_private_key(private_key: &secp256k1::PrivateKey, compressed: bool) -> Self {
        EthermintEthSecp256PublicKey {
            public_key: private_to_public_key_bytes(private_key, compressed),
        }
    }
}

impl ProtobufPublicKey for EthermintEthSecp256PublicKey {
    fn into_proto(self) -> google::protobuf::Any {
        let proto = ethermint::crypto::v1::ethsecp256k1::PubKey {
            key: self.public_key,
        };
        to_any(&proto)
    }
}
