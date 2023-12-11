// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
