// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::Address;
use crate::rlp::buffer::RlpBuffer;
use crate::rlp::RlpEncode;
use tw_hash::H256;

/// A list of addresses and storage keys that the transaction plans to access.
pub struct Access {
    pub address: Address,
    pub storage_keys: Vec<H256>,
}

impl Access {
    #[inline]
    pub fn new(address: Address) -> Self {
        Access {
            address,
            storage_keys: Vec::default(),
        }
    }

    #[inline]
    pub fn add_storage_key(&mut self, key: H256) -> &mut Self {
        self.storage_keys.push(key);
        self
    }
}

impl RlpEncode for Access {
    fn rlp_append(&self, buf: &mut RlpBuffer) {
        buf.begin_list();
        self.address.rlp_append(buf);

        // append the list of keys
        {
            buf.begin_list();
            for storage_key in self.storage_keys.iter() {
                storage_key.rlp_append(buf);
            }
            buf.finalize_list();
        }

        buf.finalize_list();
    }
}

/// [EIP-2930](https://eips.ethereum.org/EIPS/eip-2930) access list.
#[derive(Default)]
pub struct AccessList(Vec<Access>);

impl AccessList {
    #[inline]
    pub fn add_access(&mut self, access: Access) -> &mut Self {
        self.0.push(access);
        self
    }
}

impl RlpEncode for AccessList {
    fn rlp_append(&self, buf: &mut RlpBuffer) {
        buf.begin_list();

        for access in self.0.iter() {
            access.rlp_append(buf);
        }

        buf.finalize_list();
    }
}
