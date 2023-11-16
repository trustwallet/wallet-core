// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::address::{Address, CosmosAddress};
use crate::hasher::sha256_hasher::Sha256Hasher;
use crate::hasher::CosmosHasher;
use crate::private_key::secp256k1::Secp256PrivateKey;
use crate::private_key::CosmosPrivateKey;
use crate::public_key::secp256k1::Secp256PublicKey;
use crate::public_key::CosmosPublicKey;
use crate::signature::secp256k1::Secp256k1Signature;
use crate::signature::CosmosSignature;

pub trait CosmosContext {
    type Address: CosmosAddress;
    type TxHasher: CosmosHasher;
    type PrivateKey: CosmosPrivateKey;
    type PublicKey: CosmosPublicKey;
    type Signature: CosmosSignature;
}

#[derive(Default)]
pub struct StandardCosmosContext;

impl CosmosContext for StandardCosmosContext {
    type Address = Address;
    type TxHasher = Sha256Hasher;
    type PrivateKey = Secp256PrivateKey;
    type PublicKey = Secp256PublicKey;
    type Signature = Secp256k1Signature;
}
