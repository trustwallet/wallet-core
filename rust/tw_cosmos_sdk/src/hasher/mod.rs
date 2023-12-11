// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_memory::Data;

pub mod keccak256_hasher;
pub mod sha256_hasher;

pub trait CosmosHasher {
    fn hash_sign_doc(sign_doc: &[u8]) -> Data;

    fn hash_json_tx(json: &str) -> Data;
}
