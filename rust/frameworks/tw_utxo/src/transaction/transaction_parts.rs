// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::encode::stream::Stream;
use crate::encode::Encodable;
use tw_hash::H256;

// TODO: Shouldn't this be u64?
pub type Amount = i64;

#[derive(Clone, Copy, Debug, Default, Eq, PartialEq)]
pub struct OutPoint {
    pub hash: H256,
    pub index: u32,
}

impl Encodable for OutPoint {
    fn encode(&self, stream: &mut Stream) {
        stream.append(&self.hash).append(&self.index);
    }
}
