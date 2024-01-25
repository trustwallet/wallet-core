// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::coin_context::CoinContext;
use tw_cosmos_sdk::proto::injective;
use tw_cosmos_sdk::public_key::secp256k1::prepare_secp256k1_public_key;
use tw_cosmos_sdk::public_key::{
    CosmosPublicKey, JsonPublicKey, ProtobufPublicKey, PublicKeyParams,
};
use tw_keypair::KeyPairResult;
use tw_memory::Data;
use tw_proto::{google, to_any};

pub struct InjectiveEthSecp256PublicKey {
    public_key: Data,
}

impl CosmosPublicKey for InjectiveEthSecp256PublicKey {
    fn from_bytes(
        coin: &dyn CoinContext,
        public_key_bytes: &[u8],
        // Ignore custom public key parameters.
        _params: Option<PublicKeyParams>,
    ) -> KeyPairResult<Self> {
        let public_key = prepare_secp256k1_public_key(coin.public_key_type(), public_key_bytes)?;
        Ok(InjectiveEthSecp256PublicKey { public_key })
    }

    fn to_bytes(&self) -> Data {
        self.public_key.clone()
    }
}

impl JsonPublicKey for InjectiveEthSecp256PublicKey {
    fn public_key_type(&self) -> String {
        /// https://github.com/cosmostation/cosmostation-chrome-extension/blob/e2fd27d71a17993f8eef07ce30f7a04a32e52788/src/constants/cosmos.ts#L4
        const INJECTIVE_SECP256K1_PUBLIC_KEY_TYPE: &str = "injective/PubKeyEthSecp256k1";

        INJECTIVE_SECP256K1_PUBLIC_KEY_TYPE.to_string()
    }
}

impl ProtobufPublicKey for InjectiveEthSecp256PublicKey {
    fn to_proto(&self) -> google::protobuf::Any {
        let proto = injective::crypto::v1beta1::ethsecp256k1::PubKey {
            key: self.public_key.clone(),
        };
        to_any(&proto)
    }
}
