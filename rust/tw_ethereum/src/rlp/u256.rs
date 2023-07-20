// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use rlp::{Encodable, RlpStream};
use tw_number::U256;

/// TODO refactor this by implementing `RlpEncode` for `Address` at the next iteration.
pub struct RlpU256(U256);

impl From<U256> for RlpU256 {
    fn from(num: U256) -> Self {
        RlpU256(num)
    }
}

impl Encodable for RlpU256 {
    fn rlp_append(&self, s: &mut RlpStream) {
        s.encoder().encode_value(&self.0.to_big_endian_compact());
    }
}
