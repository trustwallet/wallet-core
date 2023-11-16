// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::ethermint_public_key::EthermintEthSecp256PublicKey;
use tw_cosmos_sdk::address::Address;
use tw_cosmos_sdk::context::CosmosContext;
use tw_cosmos_sdk::hasher::keccak256_hasher::Keccak256Hasher;
use tw_cosmos_sdk::private_key::secp256k1::Secp256PrivateKey;
use tw_cosmos_sdk::signature::secp256k1::Secp256k1Signature;

pub struct NativeEvmosContext;

impl CosmosContext for NativeEvmosContext {
    type Address = Address;
    type TxHasher = Keccak256Hasher;
    type PrivateKey = Secp256PrivateKey;
    type PublicKey = EthermintEthSecp256PublicKey;
    type Signature = Secp256k1Signature;
}
