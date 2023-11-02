// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::proto::cosmos::tx::v1beta1::SignDoc;
use tw_memory::Data;

pub mod sha256_hasher;

pub trait ProtobufHasher {
    fn hash_sign_doc(&self, sign_doc: &[u8]) -> Data;
}

pub trait JsonHasher {
    fn hash_json_tx(&self, json: &str) -> Data;
}
