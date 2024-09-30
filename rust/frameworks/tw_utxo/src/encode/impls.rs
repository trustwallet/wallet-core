// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::encode::compact_integer::CompactInteger;
use crate::encode::stream::Stream;
use crate::encode::Encodable;
use byteorder::{LittleEndian, WriteBytesExt};
use tw_hash::Hash;
use tw_memory::Data;

impl Encodable for Data {
    fn encode(&self, stream: &mut Stream) {
        stream
            .append(&CompactInteger::from(self.len()))
            .append_raw_slice(self.as_slice());
    }

    fn encoded_size(&self) -> usize {
        CompactInteger::from(self.len()).encoded_size() + self.len()
    }
}

impl<const N: usize> Encodable for Hash<N> {
    #[inline]
    fn encode(&self, stream: &mut Stream) {
        stream.append_raw_slice(self.as_slice());
    }

    #[inline]
    fn encoded_size(&self) -> usize {
        N
    }
}

impl Encodable for u8 {
    #[inline]
    fn encode(&self, s: &mut Stream) {
        s.write_u8(*self).unwrap();
    }

    #[inline]
    fn encoded_size(&self) -> usize {
        1
    }
}

macro_rules! impl_encodable_for_int {
    ($int:ty, $size:literal, $write_fn:tt) => {
        impl Encodable for $int {
            #[inline]
            fn encode(&self, s: &mut Stream) {
                s.$write_fn::<LittleEndian>(*self).unwrap();
            }

            #[inline]
            fn encoded_size(&self) -> usize {
                $size
            }
        }
    };
}

impl_encodable_for_int!(i32, 4, write_i32);
impl_encodable_for_int!(i64, 8, write_i64);
impl_encodable_for_int!(u16, 2, write_u16);
impl_encodable_for_int!(u32, 4, write_u32);
impl_encodable_for_int!(u64, 8, write_u64);

#[cfg(test)]
mod tests {
    use super::*;
    use crate::encode::encode;
    use tw_encoding::hex::{DecodeHex, ToHex};

    #[test]
    fn test_stream_append() {
        let mut stream = Stream::default();

        stream
            .append(&1u8)
            .append(&2u16)
            .append(&3u32)
            .append(&4u64);

        let expected = vec![1_u8, 2, 0, 3, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0];
        assert_eq!(stream.out(), expected);
    }

    #[test]
    fn test_bytes_serialize() {
        let expected = "020145".decode_hex().unwrap();
        let bytes = "0145".decode_hex().unwrap();
        assert_eq!(expected, encode(&bytes));
    }

    #[test]
    fn test_steam_append_slice() {
        let mut slice = [0u8; 4];
        slice[0] = 0x64;
        let mut stream = Stream::default();
        stream.append_raw_slice(&slice);
        assert_eq!(stream.out().to_hex(), "64000000");
    }
}
