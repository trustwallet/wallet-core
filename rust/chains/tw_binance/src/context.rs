// Copyright © 2017-2024 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::address::BinanceAddress;
use tw_cosmos_sdk::context::CosmosContext;
use tw_cosmos_sdk::hasher::sha256_hasher::Sha256Hasher;
use tw_cosmos_sdk::private_key::secp256k1::Secp256PrivateKey;
use tw_cosmos_sdk::public_key::secp256k1::Secp256PublicKey;
use tw_cosmos_sdk::signature::secp256k1::Secp256k1Signature;

pub struct BinanceContext;

impl CosmosContext for BinanceContext {
    type Address = BinanceAddress;
    /// Binance Beacon chain uses `sha256` hash.
    type TxHasher = Sha256Hasher;
    type PrivateKey = Secp256PrivateKey;
    type PublicKey = Secp256PublicKey;
    type Signature = Secp256k1Signature;
}
