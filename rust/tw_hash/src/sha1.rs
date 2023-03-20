// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::hash_wrapper::hasher;
use sha1::Sha1;

pub fn sha1(input: &[u8]) -> Vec<u8> {
    hasher::<Sha1>(input)
}
