// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

pub use hex::FromHexError;

pub trait ToHex {
    fn to_hex(&self) -> String;

    fn to_hex_prefixed(&self) -> String;
}

impl<T> ToHex for T
where
    T: AsRef<[u8]>,
{
    fn to_hex(&self) -> String {
        encode(self, false)
    }

    fn to_hex_prefixed(&self) -> String {
        encode(self, true)
    }
}

pub trait DecodeHex {
    fn decode_hex(&self) -> Result<Vec<u8>, FromHexError>;
}

impl<'a> DecodeHex for &'a str {
    fn decode_hex(&self) -> Result<Vec<u8>, FromHexError> {
        decode(self)
    }
}

pub fn decode(data: &str) -> Result<Vec<u8>, FromHexError> {
    let hex_string = data.trim_start_matches("0x");
    hex::decode(hex_string)
}

pub fn encode<T: AsRef<[u8]>>(data: T, prefixed: bool) -> String {
    let encoded = hex::encode(data.as_ref());
    if prefixed {
        return format!("0x{encoded}");
    }
    encoded
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_vec_encode_prefixed() {
        let v: &[u8] = &[
            45, 181, 0, 172, 145, 156, 221, 227, 81, 172, 54, 227, 113, 29, 131, 44, 109, 185, 118,
            105,
        ];
        assert_eq!(
            encode(v, true),
            "0x2db500ac919cdde351ac36e3711d832c6db97669"
        );
    }

    #[test]
    fn test_vec_decode_prefixed() {
        let expected = vec![
            45, 181, 0, 172, 145, 156, 221, 227, 81, 172, 54, 227, 113, 29, 131, 44, 109, 185, 118,
            105,
        ];
        assert_eq!(
            decode("0x2db500ac919cdde351ac36e3711d832c6db97669"),
            Ok(expected)
        );
    }
}
