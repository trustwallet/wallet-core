// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::Address;
use crate::rlp::buffer::RlpBuffer;
use crate::rlp::RlpEncode;
use tw_hash::H256;
use tw_number::U256;

impl RlpEncode for U256 {
    fn rlp_append(&self, buf: &mut RlpBuffer) {
        buf.append_data(&self.to_big_endian_compact())
    }
}

impl RlpEncode for H256 {
    fn rlp_append(&self, buf: &mut RlpBuffer) {
        buf.append_data(self.as_slice())
    }
}

impl RlpEncode for Address {
    fn rlp_append(&self, buf: &mut RlpBuffer) {
        buf.append_data(self.as_slice())
    }
}

impl RlpEncode for Option<Address> {
    fn rlp_append(&self, buf: &mut RlpBuffer) {
        match self {
            Some(ref addr) => addr.rlp_append(buf),
            None => buf.append_data(&[]),
        }
    }
}

impl RlpEncode for [u8] {
    fn rlp_append(&self, buf: &mut RlpBuffer) {
        buf.append_data(self)
    }
}

impl RlpEncode for str {
    fn rlp_append(&self, buf: &mut RlpBuffer) {
        buf.append_data(self.as_bytes())
    }
}
