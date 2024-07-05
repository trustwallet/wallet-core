// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::encode::stream::Stream;
use tw_memory::Data;

pub mod compact_integer;
pub mod impls;
pub mod stream;

pub fn encode<T>(t: &T) -> Data
where
    T: Encodable,
{
    let mut stream = Stream::default();
    stream.append(t);
    stream.out()
}

pub trait Encodable {
    /// Serialize the struct and appends it to the end of stream.
    fn encode(&self, stream: &mut Stream);

    /// Hint about the size of serialized struct.
    fn encoded_size(&self) -> usize;
}
