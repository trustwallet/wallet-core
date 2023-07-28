// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#[derive(Default)]
pub struct RlpBuffer {
    stream: rlp::RlpStream,
}

impl RlpBuffer {
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
        self.stream.begin_list(0);
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
    pub fn finish(self) -> impl AsRef<[u8]> {
        self.stream.out()
    }
}
