// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::coin_context::CoinContext;
use tw_cosmos_sdk::proto::ethermint;
use tw_cosmos_sdk::public_key::{
    CosmosPublicKey, JsonPublicKey, ProtobufPublicKey, PublicKeyParams,
};
use tw_keypair::ecdsa::secp256k1;
use tw_keypair::KeyPairResult;
use tw_memory::Data;
use tw_proto::{google, to_any};

pub struct EthermintEthSecp256PublicKey {
    public_key: Data,
}

impl EthermintEthSecp256PublicKey {
    pub fn new(public_key: &secp256k1::PublicKey) -> KeyPairResult<EthermintEthSecp256PublicKey> {
        Ok(EthermintEthSecp256PublicKey {
            // NativeEvmos chain requires the public key to be compressed.
            // This trick is needed because `registry.json` contains extended public key type.
            public_key: public_key.compressed().to_vec(),
        })
    }
}

impl CosmosPublicKey for EthermintEthSecp256PublicKey {
    fn from_bytes(
        _coin: &dyn CoinContext,
        public_key_bytes: &[u8],
        // Ignore custom public key parameters.
        _params: Option<PublicKeyParams>,
    ) -> KeyPairResult<Self> {
        // `NativeEvmos` requires the public key to be compressed,
        // however the uncompressed public key is used to generate an address.
        let public_key = secp256k1::PublicKey::try_from(public_key_bytes)?;
        EthermintEthSecp256PublicKey::new(&public_key)
    }

    fn to_bytes(&self) -> Data {
        self.public_key.clone()
    }
}

impl JsonPublicKey for EthermintEthSecp256PublicKey {
    fn public_key_type(&self) -> String {
        const ETHERMINT_SECP256K1_PUBLIC_KEY_TYPE: &str = "ethermint/PubKeyEthSecp256k1";

        ETHERMINT_SECP256K1_PUBLIC_KEY_TYPE.to_string()
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
