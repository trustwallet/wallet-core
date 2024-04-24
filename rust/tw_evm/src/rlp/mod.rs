// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::rlp::buffer::RlpBuffer;

pub mod buffer;
pub mod impls;
pub mod list;

/// The trait should be implemented for all types that need to be encoded in RLP.
pub trait RlpEncode {
    fn rlp_append(&self, buf: &mut RlpBuffer);
}
