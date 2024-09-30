// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::ethermint_public_key::EthermintEthSecp256PublicKey;
use tw_cosmos_sdk::address::Address;
use tw_cosmos_sdk::context::CosmosContext;
use tw_cosmos_sdk::private_key::secp256k1::Secp256PrivateKey;
use tw_cosmos_sdk::signature::secp256k1::Secp256k1Signature;
use tw_hash::hasher::Hasher;

pub struct NativeEvmosContext;

impl CosmosContext for NativeEvmosContext {
    type Address = Address;
    type PrivateKey = Secp256PrivateKey;
    type PublicKey = EthermintEthSecp256PublicKey;
    type Signature = Secp256k1Signature;

    fn default_tx_hasher() -> Hasher {
        Hasher::Keccak256
    }
}
