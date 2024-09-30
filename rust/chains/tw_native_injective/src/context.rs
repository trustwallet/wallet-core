// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::injective_public_key::InjectiveEthSecp256PublicKey;
use tw_cosmos_sdk::address::Address;
use tw_cosmos_sdk::context::CosmosContext;
use tw_cosmos_sdk::private_key::secp256k1::Secp256PrivateKey;
use tw_cosmos_sdk::signature::secp256k1::Secp256k1Signature;
use tw_hash::hasher::Hasher;

pub struct NativeInjectiveContext;

impl CosmosContext for NativeInjectiveContext {
    type Address = Address;
    type PrivateKey = Secp256PrivateKey;
    type PublicKey = InjectiveEthSecp256PublicKey;
    type Signature = Secp256k1Signature;

    fn default_tx_hasher() -> Hasher {
        Hasher::Keccak256
    }
}
