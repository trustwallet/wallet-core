// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::io::Write;
use tw_coin_entry::error::prelude::SigningResult;
use tw_memory::Data;

pub trait Encodable {
    fn encode(&self, dst: &mut Encoder) -> SigningResult<()>;
}

#[derive(Default)]
pub struct Encoder {
    buffer: Data,
}

impl Encoder {
    pub fn finish(self) -> Data {
        self.buffer
    }

    /// Appends raw bytes to the end of the stream.
    pub fn append_raw_slice(&mut self, bytes: &[u8]) {
        // Discard error for now, since we write simple vectors.
        self.buffer.write_all(bytes).unwrap();
    }
}
