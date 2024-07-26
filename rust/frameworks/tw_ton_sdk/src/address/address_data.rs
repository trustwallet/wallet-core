// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_hash::H256;

const WORKCHAIN_MASK: i32 = 0xff;

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct AddressData {
    pub workchain: i32,
    pub hash_part: H256,
}

impl AddressData {
    pub const NULL: AddressData = AddressData::null();

    pub const fn null() -> AddressData {
        AddressData {
            workchain: 0,
            hash_part: H256::new(),
        }
    }

    pub fn new(workchain: i32, hash_part: H256) -> AddressData {
        AddressData {
            workchain,
            hash_part,
        }
    }

    pub fn workchain_byte(&self) -> u8 {
        (self.workchain & WORKCHAIN_MASK) as u8
    }
}
