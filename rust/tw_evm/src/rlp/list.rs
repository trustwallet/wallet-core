// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::rlp::buffer::RlpBuffer;
use crate::rlp::RlpEncode;
use tw_memory::Data;

/// An RLP list of items.
pub struct RlpList {
    buf: RlpBuffer,
}

impl Default for RlpList {
    fn default() -> Self {
        RlpList::new()
    }
}

impl RlpList {
    /// Creates a default `RlpList`.
    pub fn new() -> RlpList {
        let mut buf = RlpBuffer::new();
        buf.begin_list();
        RlpList { buf }
    }

    /// Appends an item.
    pub fn append<T>(&mut self, item: &T) -> &mut Self
    where
        T: RlpEncode + ?Sized,
    {
        item.rlp_append(&mut self.buf);
        self
    }

    /// Appends a sublist.
    pub fn append_list(&mut self, list: RlpList) -> &mut Self {
        let encoded_list = list.finish();
        self.buf.append_data(encoded_list.as_ref());
        self
    }

    /// Appends an empty sublist.
    pub fn append_empty_list(&mut self) -> &mut Self {
        self.buf.append_empty_list();
        self
    }

    /// Appends an already encoded with all required headers value.
    pub fn append_raw_encoded(&mut self, encoded: &[u8]) -> &mut Self {
        self.buf.append_raw_encoded(encoded);
        self
    }

    /// Finalizes the RLP list buffer. Returns encoded RLP list with a header.
    #[must_use]
    pub fn finish(mut self) -> Data {
        self.buf.finalize_list();
        self.buf.finish()
    }
}
