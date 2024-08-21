// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::io;
use std::io::Write;

use tw_memory::Data;

use crate::encode::Encodable;

/// Stream used for serialization of Bitcoin structures
#[derive(Default)]
pub struct Stream {
    buffer: Data,
}

impl Stream {
    /// New stream
    pub fn new() -> Self {
        Stream {
            buffer: Data::default(),
        }
    }

    /// New stream with capacity
    pub fn with_capacity(capacity: usize) -> Self {
        Stream {
            buffer: Data::with_capacity(capacity),
        }
    }

    /// Serializes the struct and appends it to the end of stream.
    pub fn append<T>(&mut self, t: &T) -> &mut Self
    where
        T: Encodable,
    {
        t.encode(self);
        self
    }

    /// Appends raw bytes to the end of the stream.
    pub fn append_raw_slice(&mut self, bytes: &[u8]) -> &mut Self {
        // discard error for now, since we write to simple vector
        self.buffer.write_all(bytes).unwrap();
        self
    }

    /// Full stream.
    pub fn out(self) -> Data {
        self.buffer
    }
}

impl Write for Stream {
    #[inline]
    fn write(&mut self, buf: &[u8]) -> Result<usize, io::Error> {
        self.buffer.write(buf)
    }

    #[inline]
    fn flush(&mut self) -> Result<(), io::Error> {
        self.buffer.flush()
    }
}
