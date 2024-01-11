// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_memory::Data;

pub mod keccak256_hasher;
pub mod sha256_hasher;

pub trait CosmosHasher {
    fn hash_sign_doc(sign_doc: &[u8]) -> Data;

    fn hash_json_tx(json: &str) -> Data;
}
