// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::hasher::{JsonHasher, ProtobufHasher};
use crate::proto::cosmos::tx::v1beta1::SignDoc;
use tw_hash::sha2::sha256;
use tw_memory::Data;

pub struct Sha256Hasher;

impl ProtobufHasher for Sha256Hasher {
    fn hash_sign_doc(&self, sign_doc: &[u8]) -> Data {
        sha256(sign_doc)
    }
}

impl JsonHasher for Sha256Hasher {
    fn hash_json_tx(&self, json: &str) -> Data {
        sha256(json.as_bytes())
    }
}
