// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::ops::Deref;

use byteorder::ReadBytesExt;

use super::{error::Error, Decodable};
use crate::encoder::Encodable;

/// A type of variable-length integer commonly used in the Bitcoin P2P protocol and Bitcoin serialized data structures.
#[derive(Default, Debug, Clone, Copy, PartialEq)]
pub struct VarInt(u64);

impl From<usize> for VarInt {
    fn from(value: usize) -> Self {
        VarInt(value as u64)
    }
}

impl Deref for VarInt {
    type Target = u64;

    fn deref(&self) -> &u64 {
        &self.0
    }
}

impl Encodable for VarInt {
    fn encode(&self, w: &mut dyn std::io::Write) -> Result<(), Error> {
        let mut val = self.0;
        // Make sure that there is one after this
        while val >= 0x80 {
            let n = (val as u8 & 0x7f) | 0x80;
            w.write_all(&[n])?;
            val >>= 7; // It should be in multiples of 7, this should just get the next part
        }

        w.write_all(&[val as u8])?;

        Ok(())
    }

    fn encoded_size(&self) -> usize {
        match self.0 {
            val if val >= 0x8000000000000000 => 10,
            val if val >= 0x100000000000000 => 9,
            val if val >= 0x2000000000000 => 8,
            val if val >= 0x40000000000 => 7,
            val if val >= 0x800000000 => 6,
            val if val >= 0x10000000 => 5,
            val if val >= 0x200000 => 4,
            val if val >= 0x4000 => 3,
            val if val >= 0x80 => 2,
            _ => 1,
        }
    }
}

impl Decodable for VarInt {
    fn decode(r: &mut dyn std::io::Read) -> Result<Self, Error> {
        let mut res: Vec<u8> = vec![];
        loop {
            let n = r.read_u8()?;
            // Zero in any position other than the first is invalid
            // since it is not the shortest encoding.
            if n == 0 && !res.is_empty() {
                return Err(Error::ParseFailed("VarInt has a zero in a position other than the first. This is not the shortest encoding."));
            }
            res.push(n & 0b0111_1111);
            if n & 0b1000_0000 == 0 {
                break;
            }
        }
        let mut int = 0u64;
        res.reverse();
        let (last, arr) = res.split_last().unwrap();
        for bits in arr {
            int |= *bits as u64;
            int = if int.leading_zeros() >= 7 {
                int << 7
            } else {
                return Err(Error::ParseFailed("VarInt overflows u64"));
            };
        }
        int |= *last as u64;
        Ok(VarInt(int))
    }
}

#[cfg(test)]
mod tests {

    use super::*;
    use crate::encoder::deserialize;

    #[test]
    fn test_var_int_encode_data() {
        let mut w = Vec::new();

        VarInt::from(0_usize).encode(&mut w).unwrap();
        VarInt::from(0xfc_usize).encode(&mut w).unwrap();
        VarInt::from(0xfd_usize).encode(&mut w).unwrap();
        VarInt::from(0xffff_usize).encode(&mut w).unwrap();
        VarInt::from(0x10000_usize).encode(&mut w).unwrap();
        VarInt::from(0xffff_ffff_usize).encode(&mut w).unwrap();
        VarInt(0x1_0000_0000_u64).encode(&mut w).unwrap();

        let expected = vec![
            0_u8, // 0
            0xfc, 0x1, // 0xfc
            0xfd, 0x1, // 0xfd
            0xff, 0xff, 0x3, // 0xffff
            0x80, 0x80, 0x04, // 0x10000
            0xff, 0xff, 0xff, 0xff, 0xf, // 0xffff_ffff
            0x80, 0x80, 0x80, 0x80, 0x10, // 0x1_0000_0000
        ];

        assert_eq!(w.to_vec(), expected);
    }

    // Define the common test cases as a constant
    const VARINT_TEST_CASES: &[(u64, &[u8])] = &[
        (0x0u64, &[0x00u8]),
        (0xffu64, &[0xff, 0x01]),
        (0x7fffu64, &[0xff, 0xff, 0x01]),
        (0x3fffffu64, &[0xff, 0xff, 0xff, 0x01]),
        (0x1fffffffu64, &[0xff, 0xff, 0xff, 0xff, 0x01]),
        (0xfffffffffu64, &[0xff, 0xff, 0xff, 0xff, 0xff, 0x01]),
        (
            0x7ffffffffffu64,
            &[0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01],
        ),
        (
            0x3ffffffffffffu64,
            &[0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01],
        ),
        (
            0x1ffffffffffffffu64,
            &[0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01],
        ),
        (
            0xffffffffffffffffu64,
            &[0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01],
        ),
        (0x200u64, &[0x80, 0x04]),
        (0x027fu64, &[0xff, 0x04]),
        (0xff00000000u64, &[0x80, 0x80, 0x80, 0x80, 0xf0, 0x1f]),
        (0xffffffffu64, &[0xff, 0xff, 0xff, 0xff, 0x0f]),
        (0x100000000u64, &[0x80, 0x80, 0x80, 0x80, 0x10]),
        (0x7ffffffffu64, &[0xff, 0xff, 0xff, 0xff, 0x7f]),
        (0x800000000u64, &[0x80, 0x80, 0x80, 0x80, 0x80, 0x01]),
    ];

    #[test]
    fn test_var_int_encode() {
        for (i, (value, encoded)) in VARINT_TEST_CASES.iter().enumerate() {
            let mut w = Vec::new();
            let var_int = VarInt(*value);
            let encoded_size = var_int.encoded_size();
            var_int.encode(&mut w).unwrap();
            let out = w.as_slice();

            assert_eq!(out, *encoded, "Test {i} failed: data mismatch");
            assert_eq!(
                encoded_size,
                out.len(),
                "Test {i} failed: encoded size mismatch"
            );
        }
    }

    #[test]
    fn test_var_int_decode() {
        for (i, (value, data)) in VARINT_TEST_CASES.iter().enumerate() {
            let var_int = deserialize::<VarInt>(data).unwrap();

            assert_eq!(*value, *var_int, "Test {i} failed: value mismatch");
        }
    }

    #[test]
    fn test_var_int_parse_error() {
        // varint must be shortest encoding
        let res = deserialize::<VarInt>(&[0x98, 0]);
        assert!(matches!(res.unwrap_err(), Error::ParseFailed(_)));

        // If the last number is not a 0, it will error with an IO error (UnexpectedEof)
        let res = deserialize::<VarInt>(&[0xff; 1]);
        assert!(matches!(res.unwrap_err(), Error::IoError(_)));

        let res = deserialize::<VarInt>(&[
            0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 1u8,
        ]);
        assert!(matches!(res.unwrap_err(), Error::ParseFailed(_)));
    }
}
