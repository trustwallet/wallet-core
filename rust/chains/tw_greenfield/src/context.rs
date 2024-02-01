// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::GreenfieldAddress;
use crate::public_key::GreenfieldPublicKey;
use tw_cosmos_sdk::context::CosmosContext;
use tw_cosmos_sdk::private_key::secp256k1::Secp256PrivateKey;
use tw_cosmos_sdk::signature::secp256k1::Secp256k1Signature;
use tw_hash::hasher::Hasher;

pub struct GreenfieldContext;

impl CosmosContext for GreenfieldContext {
    type Address = GreenfieldAddress;
    type PrivateKey = Secp256PrivateKey;
    type PublicKey = GreenfieldPublicKey;
    type Signature = Secp256k1Signature;

    /// Greenfield uses EIP712 message signing algorithm built upon `keccak256` hash.
    fn default_tx_hasher() -> Hasher {
        Hasher::Keccak256
    }
}
