// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::encode::stream::Stream;
use crate::encode::Encodable;
use std::ops::RangeInclusive;

const ONE_BYTE_RANGE: RangeInclusive<u64> = 0..=0xFC;
const TWO_BYTES_RANGE: RangeInclusive<u64> = 0xFD..=0xFFFF;
const FOUR_BYTES_RANGE: RangeInclusive<u64> = 0x10000..=0xFFFF_FFFF;

const TWO_BYTES_FLAG: u8 = 0xFD_u8;
const FOUR_BYTES_FLAG: u8 = 0xFE_u8;
const EIGHT_BYTES_FLAG: u8 = 0xFF_u8;

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
        let v = self.0;

        if ONE_BYTE_RANGE.contains(&v) {
            stream.append(&(v as u8));
        } else if TWO_BYTES_RANGE.contains(&v) {
            stream.append(&TWO_BYTES_FLAG).append(&(v as u16));
        } else if FOUR_BYTES_RANGE.contains(&v) {
            stream.append(&FOUR_BYTES_FLAG).append(&(v as u32));
        } else {
            stream.append(&EIGHT_BYTES_FLAG).append(&v);
        }
    }

    fn encoded_size(&self) -> usize {
        const BYTE_FLAG: usize = 1;

        let v = self.0;
        if ONE_BYTE_RANGE.contains(&v) {
            BYTE_FLAG
        } else if TWO_BYTES_RANGE.contains(&v) {
            BYTE_FLAG + 2
        } else if FOUR_BYTES_RANGE.contains(&v) {
            BYTE_FLAG + 4
        } else {
            BYTE_FLAG + 8
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
            0_u8, 0xfc, 0xfd, 0xfd, 0x00, 0xfd, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x01, 0x00, 0xfe,
            0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
        ];

        assert_eq!(stream.out(), expected);
    }
}
