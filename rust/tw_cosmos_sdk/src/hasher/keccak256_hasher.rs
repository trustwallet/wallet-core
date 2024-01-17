// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::hasher::CosmosHasher;
use tw_hash::sha3::keccak256;
use tw_memory::Data;

pub struct Keccak256Hasher;

impl CosmosHasher for Keccak256Hasher {
    fn hash_sign_doc(sign_doc: &[u8]) -> Data {
        keccak256(sign_doc)
    }

    fn hash_json_tx(json: &str) -> Data {
        keccak256(json.as_bytes())
    }
}
