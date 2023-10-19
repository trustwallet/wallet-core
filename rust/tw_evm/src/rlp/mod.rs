// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::rlp::buffer::RlpBuffer;

pub mod buffer;
pub mod impls;
pub mod list;

/// The trait should be implemented for all types that need to be encoded in RLP.
pub trait RlpEncode {
    fn rlp_append(&self, buf: &mut RlpBuffer);
}
