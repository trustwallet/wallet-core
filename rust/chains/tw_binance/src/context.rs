// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::BinanceAddress;
use tw_cosmos_sdk::context::CosmosContext;
use tw_cosmos_sdk::private_key::secp256k1::Secp256PrivateKey;
use tw_cosmos_sdk::public_key::secp256k1::Secp256PublicKey;
use tw_cosmos_sdk::signature::secp256k1::Secp256k1Signature;
use tw_hash::hasher::Hasher;

pub struct BinanceContext;

impl CosmosContext for BinanceContext {
    type Address = BinanceAddress;
    type PrivateKey = Secp256PrivateKey;
    type PublicKey = Secp256PublicKey;
    type Signature = Secp256k1Signature;

    /// Binance Beacon chain uses `sha256` hash.
    fn default_tx_hasher() -> Hasher {
        Hasher::Sha256
    }
}
