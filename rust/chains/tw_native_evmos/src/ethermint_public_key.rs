// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::coin_context::CoinContext;
use tw_cosmos_sdk::proto::ethermint;
use tw_cosmos_sdk::public_key::secp256k1::Secp256PublicKey;
use tw_cosmos_sdk::public_key::{
    CosmosPublicKey, JsonPublicKey, ProtobufPublicKey, PublicKeyParams,
};
use tw_keypair::{tw, KeyPairResult};
use tw_memory::Data;
use tw_proto::{google, type_url};

const ETHERMINT_SECP256K1_PUBLIC_KEY_TYPE: &str = "ethermint/PubKeyEthSecp256k1";

pub struct EthermintEthSecp256PublicKey(Secp256PublicKey);

impl EthermintEthSecp256PublicKey {
    fn default_public_key_params() -> PublicKeyParams {
        PublicKeyParams {
            // `NativeEvmos` requires the public key to be compressed,
            // however the uncompressed public key is used to generate an address.
            public_key_type: tw::PublicKeyType::Secp256k1,
            json_type: ETHERMINT_SECP256K1_PUBLIC_KEY_TYPE.to_string(),
            protobuf_type_url: type_url::<ethermint::crypto::v1::ethsecp256k1::PubKey>(),
        }
    }
}

impl CosmosPublicKey for EthermintEthSecp256PublicKey {
    fn from_bytes(
        coin: &dyn CoinContext,
        public_key_bytes: &[u8],
        maybe_params: Option<PublicKeyParams>,
    ) -> KeyPairResult<Self> {
        // Use default Ethermint public key parameters if otherwise is not specified,
        // however the uncompressed public key is used to generate an address.
        let params = maybe_params.unwrap_or_else(Self::default_public_key_params);
        Secp256PublicKey::from_bytes(coin, public_key_bytes, Some(params))
            .map(EthermintEthSecp256PublicKey)
    }

    fn to_bytes(&self) -> Data {
        self.0.to_bytes()
    }
}

impl JsonPublicKey for EthermintEthSecp256PublicKey {
    fn public_key_type(&self) -> String {
        self.0.public_key_type()
    }
}

impl ProtobufPublicKey for EthermintEthSecp256PublicKey {
    fn to_proto(&self) -> google::protobuf::Any {
        self.0.to_proto()
    }
}
