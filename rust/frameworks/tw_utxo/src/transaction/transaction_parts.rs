// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::encode::stream::Stream;
use crate::encode::Encodable;
use tw_hash::H256;

/// Amount in satoshis (Can be negative) in rare cases.
/// https://github.com/bitcoin/bitcoin/blob/bd5d1688b4311e21c0e0ff89a3ae02ef7d0543b8/src/consensus/amount.h#L11-L12
pub type Amount = i64;

#[derive(Clone, Copy, Debug, Default, Eq, Hash, PartialEq)]
pub struct OutPoint {
    pub hash: H256,
    pub index: u32,
}

impl Encodable for OutPoint {
    fn encode(&self, stream: &mut Stream) {
        stream.append(&self.hash).append(&self.index);
    }

    fn encoded_size(&self) -> usize {
        self.hash.encoded_size() + self.index.encoded_size()
    }
}
