// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::address::Address;
use rlp::{Encodable, RlpStream};

/// TODO implement `RlpEncode` for `Address` when Ethereum ABI is designed manually.
pub struct RlpAddress(Address);

impl From<Address> for RlpAddress {
    fn from(addr: Address) -> Self {
        RlpAddress(addr)
    }
}

impl Encodable for RlpAddress {
    fn rlp_append(&self, s: &mut RlpStream) {
        s.encoder().encode_value(self.0.as_slice());
    }
}

pub struct RlpAddressOption(Option<Address>);

impl From<Option<Address>> for RlpAddressOption {
    fn from(addr: Option<Address>) -> Self {
        RlpAddressOption(addr)
    }
}

impl Encodable for RlpAddressOption {
    fn rlp_append(&self, s: &mut RlpStream) {
        let addr_data = match self.0 {
            Some(ref addr) => addr.as_slice(),
            None => &[],
        };
        s.encoder().encode_value(addr_data)
    }
}
