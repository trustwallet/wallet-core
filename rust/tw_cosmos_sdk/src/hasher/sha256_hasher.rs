// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::hasher::CosmosHasher;
use tw_hash::sha2::sha256;
use tw_memory::Data;

pub struct Sha256Hasher;

impl CosmosHasher for Sha256Hasher {
    fn hash_sign_doc(sign_doc: &[u8]) -> Data {
        sha256(sign_doc)
    }

    fn hash_json_tx(json: &str) -> Data {
        sha256(json.as_bytes())
    }
}
