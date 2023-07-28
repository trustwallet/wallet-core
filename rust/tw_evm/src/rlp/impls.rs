// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::address::Address;
use crate::rlp::buffer::RlpBuffer;
use crate::rlp::RlpEncode;
use tw_memory::Data;
use tw_number::U256;

impl RlpEncode for U256 {
    fn rlp_append(&self, buf: &mut RlpBuffer) {
        buf.append_data(&self.to_big_endian_compact())
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

impl RlpEncode for Data {
    fn rlp_append(&self, buf: &mut RlpBuffer) {
        buf.append_data(self.as_slice())
    }
}

impl<'a> RlpEncode for &'a [u8] {
    fn rlp_append(&self, buf: &mut RlpBuffer) {
        buf.append_data(self)
    }
}
