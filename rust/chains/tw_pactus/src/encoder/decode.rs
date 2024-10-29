// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_hash::Hash;
use tw_keypair::ed25519::{sha512::PublicKey, Signature};

use super::error::Error;
use crate::encoder::var_int::VarInt;
use crate::encoder::Decodable;

pub(crate) fn decode_var_slice(r: &mut dyn std::io::Read) -> Result<Vec<u8>, Error> {
    let len = *VarInt::decode(r)?;
    let mut buf = vec![0; len as usize];
    r.read_exact(&mut buf)?;

    Ok(buf)
}

pub(crate) fn decode_fix_slice<const N: usize>(r: &mut dyn std::io::Read) -> Result<[u8; N], Error> {
    let mut buf: [u8; N] = [0; N];
    r.read_exact(&mut buf)?;

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
        let data = decode_fix_slice::<PublicKey::LEN>(r)?;
        PublicKey::try_from(data.as_slice())
            .map_err(|_| self::Error::ParseFailed("Invalid Public Key"))
    }
}

impl Decodable for Signature {
    fn decode(r: &mut dyn std::io::Read) -> Result<Self, Error> {
        let data = decode_fix_slice::<Signature::LEN>(r)?;
        Signature::try_from(data.as_slice())
            .map_err(|_| self::Error::ParseFailed("Invalid Signature"))
    }
}

impl<const N: usize> Decodable for Hash<N> {
    fn decode(r: &mut dyn std::io::Read) -> Result<Self, Error> {
        let data = decode_fix_slice::<N>(r)?;
        Hash::try_from(data.as_slice()).map_err(|_| self::Error::ParseFailed("Invalid Hash"))
    }
}

macro_rules! impl_decodable_for_int {
    ($int:ty, $size:literal) => {
        impl Decodable for $int {
            #[inline]
            fn decode(r: &mut dyn std::io::Read) -> Result<Self, Error> {
                let mut buf = [0; $size];
                r.read_exact(&mut buf[..])?;
                Ok(<$int>::from_le_bytes(buf))
            }
        }
    };
}

impl_decodable_for_int!(u8, 1);
impl_decodable_for_int!(i32, 4);
impl_decodable_for_int!(i64, 8);
impl_decodable_for_int!(u16, 2);
impl_decodable_for_int!(u32, 4);
impl_decodable_for_int!(u64, 8);

#[cfg(test)]
mod tests {
    use std::io::Cursor;

    use tw_encoding::hex::DecodeHex;

    use super::*;
    use crate::encoder::deserialize;

    #[test]
    fn test_decode_var_slice() {
        let expected = vec![1, 2, 3, 4];
        let mut cursor = Cursor::new("0401020304".decode_hex().unwrap());
        let slice = decode_var_slice(&mut cursor).unwrap();

        assert_eq!(expected, slice);
    }

    #[test]
    fn test_decode_fix_slice() {
        let expected = vec![1, 2, 3, 4];
        let mut cursor = Cursor::new("01020304".decode_hex().unwrap());
        let slice = decode_fix_slice::<4>(&mut cursor).unwrap();

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
        let bytes = "0568656c6c6f056844656c6c6e".decode_hex().unwrap();

        assert_eq!(expected, deserialize::<String>(&bytes).unwrap());
    }
}
