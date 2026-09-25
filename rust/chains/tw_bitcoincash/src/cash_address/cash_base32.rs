// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_encoding::{EncodingError, EncodingResult};

/// Charset for converting from base32.
const CHARSET_REV: [i8; 128] = [
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    15, -1, 10, 17, 21, 20, 26, 30, 7, 5, -1, -1, -1, -1, -1, -1, -1, 29, -1, 24, 13, 25, 9, 8, 23,
    -1, 18, 22, 31, 27, 19, -1, 1, 0, 3, 16, 11, 28, 12, 14, 6, 4, 2, -1, -1, -1, -1, -1, -1, 29,
    -1, 24, 13, 25, 9, 8, 23, -1, 18, 22, 31, 27, 19, -1, 1, 0, 3, 16, 11, 28, 12, 14, 6, 4, 2, -1,
    -1, -1, -1, -1,
];

/// Charset for converting to base32.
const CHARSET: [char; 32] = [
    'q', 'p', 'z', 'r', 'y', '9', 'x', '8', 'g', 'f', '2', 't', 'v', 'd', 'w', '0', 's', '3', 'j',
    'n', '5', '4', 'k', 'h', 'c', 'e', '6', 'm', 'u', 'a', '7', 'l',
];

/// Encodes a 5-bit packed array as BCH base32.
pub fn encode(input: &[u8]) -> EncodingResult<String> {
    input
        .iter()
        .map(|i| CHARSET.get(*i as usize))
        .collect::<Option<String>>()
        .ok_or(EncodingError::InvalidInput)
}

/// Decods a BCH base32 string as a 5-bit packed array.
pub fn decode(input: &str) -> EncodingResult<Vec<u8>> {
    input
        .chars()
        .map(|c| {
            let val = *CHARSET_REV
                .get(c as usize)
                .ok_or(EncodingError::InvalidInput)?;
            if val == -1 {
                return Err(EncodingError::InvalidInput);
            }
            Ok(val as u8)
        })
        .collect()
}

#[cfg(test)]
mod tests {
    use super::*;
    use tw_encoding::hex::DecodeHex;

    /// `raw` must be 5-bit packed - the condition is required by base32 encode and decode functions.
    #[track_caller]
    fn test_base32_impl(raw_hex: &str, encoded: &str) {
        let raw = raw_hex.decode_hex().unwrap();
        let actual_encoded = encode(&raw).unwrap();
        assert_eq!(actual_encoded, encoded);
        let actual_raw = decode(&actual_encoded).unwrap();
        assert_eq!(actual_raw, raw);
    }

    #[test]
    fn test_base32() {
        test_base32_impl(
            "180e0919131e16011c001e1c1607010b120701071317151e1819141b031b1d0a",
            "cwfen7kpuq7uk8ptj8p8nh47ce5mrma2",
        );
        test_base32_impl(
            "08080f081c1d1002011119051104020811150f140b181d1006140b0216121605",
            "gg0guaszp3e93yzg3405tcasx5tzkjk9",
        );
        test_base32_impl(
            "0c0215180000050e07061619161f1409120c0a060b1c070e13090f1d0f160b1b",
            "vz4cqq9w8xkekl5fjv2xtu8wnf0a0ktm",
        );
    }
}
