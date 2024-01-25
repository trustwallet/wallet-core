// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::coin_context::CoinContext;
use tw_cosmos_sdk::proto::injective;
use tw_cosmos_sdk::public_key::secp256k1::Secp256PublicKey;
use tw_cosmos_sdk::public_key::{
    CosmosPublicKey, JsonPublicKey, ProtobufPublicKey, PublicKeyParams,
};
use tw_keypair::KeyPairResult;
use tw_memory::Data;
use tw_proto::{google, type_url};

/// https://github.com/cosmostation/cosmostation-chrome-extension/blob/e2fd27d71a17993f8eef07ce30f7a04a32e52788/src/constants/cosmos.ts#L4
const INJECTIVE_SECP256K1_PUBLIC_KEY_TYPE: &str = "injective/PubKeyEthSecp256k1";

pub struct InjectiveEthSecp256PublicKey(Secp256PublicKey);

impl InjectiveEthSecp256PublicKey {
    fn default_public_key_params(coin: &dyn CoinContext) -> PublicKeyParams {
        PublicKeyParams {
            // `NativeInjective` uses the same public key type as specified in `registry.json`.
            public_key_type: coin.public_key_type(),
            json_type: INJECTIVE_SECP256K1_PUBLIC_KEY_TYPE.to_string(),
            protobuf_type_url: type_url::<injective::crypto::v1beta1::ethsecp256k1::PubKey>(),
        }
    }
}

impl CosmosPublicKey for InjectiveEthSecp256PublicKey {
    fn from_bytes(
        coin: &dyn CoinContext,
        public_key_bytes: &[u8],
        maybe_params: Option<PublicKeyParams>,
    ) -> KeyPairResult<Self> {
        // Use default Ethermint public key parameters if otherwise is not specified,
        // however the uncompressed public key is used to generate an address.
        let params = maybe_params.unwrap_or_else(|| Self::default_public_key_params(coin));
        Secp256PublicKey::from_bytes(coin, public_key_bytes, Some(params))
            .map(InjectiveEthSecp256PublicKey)
    }

    fn to_bytes(&self) -> Data {
        self.0.to_bytes()
    }
}

impl JsonPublicKey for InjectiveEthSecp256PublicKey {
    fn public_key_type(&self) -> String {
        self.0.public_key_type()
    }
}

impl ProtobufPublicKey for InjectiveEthSecp256PublicKey {
    fn to_proto(&self) -> google::protobuf::Any {
        self.0.to_proto()
    }
}
