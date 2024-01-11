// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_memory::Data;

#[derive(Default)]
pub struct RlpBuffer {
    stream: rlp::RlpStream,
}

impl RlpBuffer {
    /// cbindgen:ignore
    const NO_ITEMS: usize = 0;
    /// cbindgen:ignore
    const ONE_ITEM: usize = 1;

    /// Creates `RlpBuffer` with a default capacity.
    pub fn new() -> RlpBuffer {
        RlpBuffer::default()
    }

    /// Begins an RLP list.
    /// Please note that it should be finalized using [`RlpBuffer::finalize_list`].
    pub(crate) fn begin_list(&mut self) {
        self.stream.begin_unbounded_list();
    }

    /// Appends an item by its `bytes` representation.
    /// This method is usually called from [`RlpEncode::rlp_append`].
    pub fn append_data(&mut self, bytes: &[u8]) {
        self.stream.encoder().encode_value(bytes)
    }

    /// Appends an empty list.
    pub(crate) fn append_empty_list(&mut self) {
        self.stream.begin_list(Self::NO_ITEMS);
    }

    /// Appends an already encoded with all required headers value.
    pub(crate) fn append_raw_encoded(&mut self, bytes: &[u8]) {
        self.stream.append_raw(bytes, Self::ONE_ITEM);
    }

    /// Finalize the list.
    ///
    /// # Panic
    ///
    /// The method may panic if [`RlpBuffer::begin_list`] hasn't been called before.
    pub(crate) fn finalize_list(&mut self) {
        self.stream.finalize_unbounded_list();
    }

    /// Streams out encoded bytes.
    ///
    /// # Panic
    ///
    /// Tte method panic if [`RlpBuffer::begin_list`] has been called without [`RlpBuffer::finalize_list`].
    pub fn finish(self) -> Data {
        self.stream.out().into()
    }
}
