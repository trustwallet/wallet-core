// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use byteorder::{LittleEndian, WriteBytesExt};
use tw_hash::Hash;
use tw_keypair::ed25519::{sha512::PublicKey, Signature};

use super::error::Error;
use crate::encoder::var_int::VarInt;
use crate::encoder::Encodable;

pub(crate) fn encode_var_slice(data: &[u8], w: &mut dyn std::io::Write) -> Result<usize, Error> {
    let mut len = VarInt::from(data.len()).encode(w)?;
    len += w.write(data)?;

    Ok(len)
}

pub(crate) fn encode_fix_slice(data: &[u8], w: &mut dyn std::io::Write) -> Result<usize, Error> {
    Ok(w.write(data)?)
}

impl Encodable for Vec<u8> {
    fn encode(&self, w: &mut dyn std::io::Write) -> Result<usize, Error> {
        encode_var_slice(self, w)
    }

    fn encoded_size(&self) -> usize {
        VarInt::from(self.len()).encoded_size() + self.len()
    }
}

impl Encodable for String {
    fn encode(&self, w: &mut dyn std::io::Write) -> Result<usize, Error> {
        encode_var_slice(self.as_bytes(), w)
    }

    fn encoded_size(&self) -> usize {
        VarInt::from(self.len()).encoded_size() + self.len()
    }
}

impl Encodable for PublicKey {
    fn encode(&self, w: &mut dyn std::io::Write) -> Result<usize, Error> {
        encode_fix_slice(self.as_slice(), w)
    }

    fn encoded_size(&self) -> usize {
        PublicKey::LEN
    }
}

impl Encodable for Signature {
    fn encode(&self, w: &mut dyn std::io::Write) -> Result<usize, Error> {
        encode_fix_slice(self.to_bytes().as_slice(), w)
    }

    fn encoded_size(&self) -> usize {
        Signature::LEN
    }
}

impl<const N: usize> Encodable for Hash<N> {
    fn encode(&self, w: &mut dyn std::io::Write) -> Result<usize, Error> {
        encode_fix_slice(self.as_slice(), w)
    }

    fn encoded_size(&self) -> usize {
        N
    }
}

impl Encodable for u8 {
    #[inline]
    fn encode(&self, w: &mut dyn std::io::Write) -> Result<usize, Error> {
        w.write_u8(*self)?;

        Ok(1)
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
            fn encode(&self, w: &mut dyn std::io::Write) -> Result<usize, Error> {
                w.$write_fn::<LittleEndian>(*self)?;

                Ok($size)
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
    use crate::encoder::serialize;

    #[test]
    fn test_encode_var_slice() {
        let expected = "0401020304".decode_hex().unwrap();
        let slice = vec![1, 2, 3, 4];
        let mut w = Vec::new();
        encode_var_slice(&slice, &mut w).unwrap();

        assert_eq!(expected, w.to_vec());
    }

    #[test]
    fn test_encode_fix_slice() {
        let expected = "01020304".decode_hex().unwrap();
        let slice = vec![1, 2, 3, 4];
        let mut w = Vec::new();
        encode_fix_slice(&slice, &mut w).unwrap();

        assert_eq!(expected, w.to_vec());
    }

    #[test]
    fn test_encode_numbers() {
        let mut w = Vec::new();

        1u8.encode(&mut w).unwrap();
        2u16.encode(&mut w).unwrap();
        3u32.encode(&mut w).unwrap();
        4u64.encode(&mut w).unwrap();

        let expected = vec![1_u8, 2, 0, 3, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0];
        assert_eq!(w.to_vec(), expected);
    }

    #[test]
    fn test_encode_bytes() {
        let expected = "020145".decode_hex().unwrap();
        let bytes = "0145".decode_hex().unwrap();

        assert_eq!(expected, serialize(&bytes).unwrap());
        assert_eq!(expected.len(), bytes.encoded_size());
    }

    #[test]
    fn test_encode_string() {
        let expected = "0568656c6c6f".decode_hex().unwrap();
        let msg = "hello".to_string();

        assert_eq!(expected, serialize(&msg).unwrap());
        assert_eq!(expected.len(), msg.encoded_size());
    }
}
