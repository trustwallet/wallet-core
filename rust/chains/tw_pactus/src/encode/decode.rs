// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::io;

use tw_hash::Hash;
use tw_keypair::ed25519::{sha512::PublicKey, Signature};

use super::error::Error;
use crate::encode::var_int::VarInt;
use crate::encode::Decodable;

pub(crate) fn decode_var_slice(r: &mut dyn std::io::Read) -> Result<Vec<u8>, Error> {
    let len = *VarInt::decode(r)?;
    let mut buf = vec![0; len as usize];
    r.read(&mut buf)?;

    Ok(buf)
}

impl Decodable for Vec<u8> {
    fn decode(r: &mut dyn std::io::Read) -> Result<Self, Error> {
        decode_var_slice(r)
    }
}

impl Decodable for String {
    fn decode(r: &mut dyn std::io::Read) -> Result<Self, Error> {
        let data = decode_var_slice(r)?;
        String::from_utf8(data).map_err(|_| self::Error::ParseFailed("Invalid String"))
    }
}

impl Decodable for PublicKey {
    fn decode(r: &mut dyn std::io::Read) -> Result<Self, Error> {
        let data = decode_var_slice(r)?;
        PublicKey::try_from(data.as_slice())
            .map_err(|_| self::Error::ParseFailed("Invalid Public Key"))
    }
}

impl Decodable for Signature {
    fn decode(r: &mut dyn std::io::Read) -> Result<Self, Error> {
        let data = decode_var_slice(r)?;
        Signature::try_from(data.as_slice())
            .map_err(|_| self::Error::ParseFailed("Invalid Signature"))
    }
}

impl<const N: usize> Decodable for Hash<N> {
    fn decode(r: &mut dyn std::io::Read) -> Result<Self, Error> {
        let data = decode_var_slice(r)?;
        Hash::try_from(data.as_slice()).map_err(|_| self::Error::ParseFailed("Invalid Hash"))
    }
}

macro_rules! impl_decodable_for_int {
    ($int:ty, $size:literal, $write_fn:tt) => {
        impl Decodable for $int {
            #[inline]
            fn decode(r: &mut dyn std::io::Read) -> Result<Self, Error> {
                let mut buf = [0; $size];
                r.read(&mut buf[..])?;
                Ok(<$int>::from_le_bytes(buf))
            }
        }
    };
}

impl_decodable_for_int!(u8, 1, write_u8);
impl_decodable_for_int!(i32, 4, write_i32);
impl_decodable_for_int!(i64, 8, write_i64);
impl_decodable_for_int!(u16, 2, write_u16);
impl_decodable_for_int!(u32, 4, write_u32);
impl_decodable_for_int!(u64, 8, write_u64);

#[cfg(test)]
mod tests {
    use io::Cursor;
    use tw_encoding::hex::DecodeHex;

    use super::*;
    use crate::encode::{decode, deserialize, serialize};

    #[test]
    fn test_decode_var_slice() {
        let expected = vec![1, 2, 3, 4];
        let mut cursor = Cursor::new("0401020304".decode_hex().unwrap());
        let slice = decode_var_slice(&mut cursor).unwrap();

        assert_eq!(expected, slice);
    }

    #[test]
    fn test_encode_numbers() {
        let data = vec![1_u8, 2, 0, 3, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0];
        let mut cursor = Cursor::new(data);

        assert_eq!(1u8, u8::decode(&mut cursor).unwrap());
        assert_eq!(2u16, u16::decode(&mut cursor).unwrap());
        assert_eq!(3u32, u32::decode(&mut cursor).unwrap());
        assert_eq!(4u64, u64::decode(&mut cursor).unwrap());
    }

    #[test]
    fn test_decode_bytes() {
        let expected = "0145".decode_hex().unwrap();
        let bytes = "020145".decode_hex().unwrap();

        assert_eq!(expected, deserialize::<Vec<u8>>(&bytes).unwrap());
    }

    #[test]
    fn test_encode_string() {
        let expected = "hello".to_string();
        let bytes = "0568656c6c6f".decode_hex().unwrap();

        assert_eq!(expected, deserialize::<String>(&bytes).unwrap());
    }
}
