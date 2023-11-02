// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_coin_entry::error::SigningResult;
use crate::proto::cosmos;
use crate::public_key::{CosmosPubkey, ProtobufPublicKey};
use tw_keypair::ecdsa::secp256k1;
use tw_keypair::tw::PublicKeyType;
use tw_memory::Data;
use tw_proto::{google, to_any};

pub struct Secp256PublicKey {
    public_key: Data,
}

impl Secp256PublicKey {
    pub fn with_private_key(private_key: &secp256k1::PrivateKey, compressed: bool) -> Self {
        Secp256PublicKey {
            public_key: private_to_public_key_bytes(private_key, compressed),
        }
    }
}

impl CosmosPubkey for Secp256PublicKey {
    fn from_private_key_bytes(private: &[u8]) -> SigningResult<Self> {
        let key = secp256k1::PrivateKey::try_from(private)?;
        Secp256PublicKey {
            public_key:
        }
    }

    fn from_public_key_bytes(public: &[u8]) -> SigningResult<Self> {
        todo!()
    }
}

impl ProtobufPublicKey for Secp256PublicKey {
    fn into_proto(self) -> google::protobuf::Any {
        let proto = cosmos::crypto::secp256k1::PubKey {
            key: self.public_key,
        };
        to_any(&proto)
    }
}

pub(crate) fn private_to_public_key_bytes(
    private_key: &secp256k1::PrivateKey,
    compressed: bool,
) -> Data {
    if compressed {
        private_key.public().compressed().to_vec()
    } else {
        private_key.public().uncompressed().to_vec()
    }
}
