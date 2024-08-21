// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::encode::stream::Stream;
use crate::encode::Encodable;

/// A type of variable-length integer commonly used in the Bitcoin P2P protocol and Bitcoin serialized data structures.
#[derive(Default, Debug, Clone, Copy, PartialEq)]
pub struct CompactInteger(u64);

impl From<usize> for CompactInteger {
    fn from(value: usize) -> Self {
        CompactInteger(value as u64)
    }
}

impl Encodable for CompactInteger {
    fn encode(&self, stream: &mut Stream) {
        let mut val = self.0;
        // Make sure that there is one after this
        while val >= 0x80 {
            let n = (val as u8 & 0x7f) | 0x80;
            stream.append(&{ n });
            val >>= 7; // It should be in multiples of 7, this should just get the next part
        }

        stream.append(&(val as u8));
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

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_compact_integer_stream() {
        let mut stream = Stream::default();

        stream
            .append(&CompactInteger::from(0_usize))
            .append(&CompactInteger::from(0xfc_usize))
            .append(&CompactInteger::from(0xfd_usize))
            .append(&CompactInteger::from(0xffff_usize))
            .append(&CompactInteger::from(0x10000_usize))
            .append(&CompactInteger::from(0xffff_ffff_usize))
            .append(&CompactInteger(0x1_0000_0000_u64));

        let expected = vec![
            0_u8, // 0
            0xfc, 0x1, // 0xfc
            0xfd, 0x1, // 0xfd
            0xff, 0xff, 0x3, // 0xffff
            0x80, 0x80, 0x04, // 0x10000
            0xff, 0xff, 0xff, 0xff, 0xf, // 0xffff_ffff
            0x80, 0x80, 0x80, 0x80, 0x10, // 0x1_0000_0000
        ];

        assert_eq!(stream.out(), expected);
    }

    #[test]
    fn test_write_var_int() {
        let tests = vec![
            (0x0u64, vec![0x00u8]),
            (0xffu64, vec![0xff, 0x01]),
            (0x7fffu64, vec![0xff, 0xff, 0x01]),
            (0x3fffffu64, vec![0xff, 0xff, 0xff, 0x01]),
            (0x1fffffffu64, vec![0xff, 0xff, 0xff, 0xff, 0x01]),
            (0xfffffffffu64, vec![0xff, 0xff, 0xff, 0xff, 0xff, 0x01]),
            (
                0x7ffffffffffu64,
                vec![0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01],
            ),
            (
                0x3ffffffffffffu64,
                vec![0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01],
            ),
            (
                0x1ffffffffffffffu64,
                vec![0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01],
            ),
            (
                0xffffffffffffffffu64,
                vec![0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01],
            ),
            (0x200u64, vec![0x80, 0x04]),
            (0x027fu64, vec![0xff, 0x04]),
            (0xff00000000u64, vec![0x80, 0x80, 0x80, 0x80, 0xf0, 0x1f]),
            (0xffffffffu64, vec![0xff, 0xff, 0xff, 0xff, 0x0f]),
            (0x100000000u64, vec![0x80, 0x80, 0x80, 0x80, 0x10]),
            (0x7ffffffffu64, vec![0xff, 0xff, 0xff, 0xff, 0x7f]),
            (0x800000000u64, vec![0x80, 0x80, 0x80, 0x80, 0x80, 0x01]),
        ];

        for (i, (input, expected_buf)) in tests.iter().enumerate() {
            let mut stream = Stream::default();
            let com_int = CompactInteger(*input);
            stream.append(&com_int);

            let out = stream.out();
            assert_eq!(
                &out, expected_buf,
                "Test case {} failed: buffer mismatch",
                i
            );
            assert_eq!(
                com_int.encoded_size(),
                out.len(),
                "Test case {} failed: size mismatch",
                i
            );
        }
    }
}
