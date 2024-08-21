// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use byteorder::{LittleEndian, WriteBytesExt};
use tw_hash::Hash;
use tw_keypair::ed25519::{sha512::PublicKey, Signature};

use crate::encode::compact_integer::CompactInteger;
use crate::encode::stream::Stream;
use crate::encode::Encodable;

impl Encodable for Vec<u8> {
    fn encode(&self, stream: &mut Stream) {
        stream
            .append(&CompactInteger::from(self.len()))
            .append_raw_slice(self.as_ref());
    }

    fn encoded_size(&self) -> usize {
        CompactInteger::from(self.len()).encoded_size() + self.len()
    }
}

impl Encodable for String {
    fn encode(&self, stream: &mut Stream) {
        stream
            .append(&CompactInteger::from(self.len()))
            .append_raw_slice(self.as_bytes());
    }

    fn encoded_size(&self) -> usize {
        CompactInteger::from(self.len()).encoded_size() + self.len()
    }
}

impl Encodable for PublicKey {
    fn encode(&self, stream: &mut Stream) {
        self.to_bytes().encode(stream);
    }

    fn encoded_size(&self) -> usize {
        self.to_bytes().encoded_size()
    }
}

impl Encodable for Signature {
    fn encode(&self, stream: &mut Stream) {
        self.to_bytes().encode(stream);
    }

    fn encoded_size(&self) -> usize {
        self.to_bytes().encoded_size()
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
    use tw_encoding::hex::DecodeHex;

    use super::*;
    use crate::encode::encode;

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
        assert_eq!(expected.len(), bytes.encoded_size());
    }

    #[test]
    fn test_steam_append_slice() {
        let expected = "64000000".decode_hex().unwrap();
        let mut slice = [0u8; 4];
        slice[0] = 0x64;
        let mut stream = Stream::default();
        stream.append_raw_slice(&slice);

        assert_eq!(expected, stream.out());
    }

    #[test]
    fn test_encode_string() {
        let expected = "0568656c6c6f".decode_hex().unwrap();
        let msg = "hello".to_string();

        assert_eq!(expected, encode(&msg));
        assert_eq!(expected.len(), msg.encoded_size());
    }
}
