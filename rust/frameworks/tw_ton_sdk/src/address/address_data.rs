// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_hash::H256;

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct AddressData {
    pub workchain: i32,
    pub hash_part: H256,
}

impl AddressData {
    pub fn new(workchain: i32, hash_part: H256) -> AddressData {
        AddressData {
            workchain,
            hash_part,
        }
    }
}
