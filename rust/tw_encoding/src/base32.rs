// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

const ALPHABET_RFC4648: &[u8] = b"ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";

pub fn encode(input: &[u8], alphabet: Option<&[u8]>, padding: bool) -> Result<String, String> {
    let alphabet = alphabet.unwrap_or(ALPHABET_RFC4648);
    if alphabet.len() != 32 {
        return Err("Invalid alphabet: must contain 32 characters".to_string());
    }

    let mut result = String::new();
    let mut buffer: u32 = 0;
    let mut buffer_size = 0;

    for &byte in input {
        buffer = (buffer << 8) | u32::from(byte);
        buffer_size += 8;

        while buffer_size >= 5 {
            result.push(char::from(alphabet[(buffer >> (buffer_size - 5)) as usize & 31]));
            buffer_size -= 5;
        }
    }

    if buffer_size > 0 {
        result.push(char::from(alphabet[(buffer << (5 - buffer_size)) as usize & 31]));
    }

    if padding {
        let padding = 8 - (result.len() % 8);
        result.extend(std::iter::repeat('=').take(padding));
    }

    Ok(result)
}

/// TODO `base64::decode` requires for padding bytes to be present if `padding = true`.
/// This leads to an inconsistent behaviour.
pub fn decode(input: &str, alphabet: Option<&[u8]>, padding: bool) -> Result<Vec<u8>, String> {
    let alphabet = alphabet.unwrap_or(ALPHABET_RFC4648);
    let mut output = Vec::new();
    let mut buffer: u32 = 0;
    let mut bits_left = 0;
    let alphabet_map: std::collections::HashMap<u8, u32> = alphabet.iter().enumerate().map(|(i, &c)| (c, i as u32)).collect();
    let input = if padding {
        input.trim_end_matches('=')
    } else {
        input
    };

    for c in input.bytes() {
        let val = match alphabet_map.get(&c) {
            Some(val) => *val,
            None => return Err("Invalid character in input".to_string()),
        };
        buffer = (buffer << 5) | val;
        bits_left += 5;
        if bits_left >= 8 {
            output.push((buffer >> (bits_left - 8)) as u8);
            bits_left -= 8;
        }
    }

    if padding && bits_left >= 5 {
        return Err("Invalid padding in input".to_string());
    }

    if output == vec![0] {
        return Ok(vec![]);
    }
    Ok(output)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_base32_encode() {
        let data = b"Hello, world!";
        let expected = "JBSWY3DPFQQHO33SNRSCC";

        let result = encode(data, None, false).unwrap();
        assert_eq!(result, expected);
    }

    #[test]
    fn test_base32_encode_padding() {
        let data = b"Hello, world!";
        let expected = "JBSWY3DPFQQHO33SNRSCC===";

        let result = encode(data, None, true).unwrap();
        assert_eq!(result, expected);
    }

    #[test]
    fn test_base32_encode_filecoin() {
        let alphabet = "abcdefghijklmnopqrstuvwxyz234567";
        let data = b"7uoq6tp427uzv7fztkbsnn64iwotfrristwpryy";
        let expected = "g52w64jworydimrxov5hmn3gpj2gwyttnzxdmndjo5xxiztsojuxg5dxobzhs6i";
        let result = encode(data, Some(alphabet.as_bytes()), false).unwrap();
        assert_eq!(result, expected);

        let invalid_alphabet = "invalidalphabet";
        let result = encode(data, Some(invalid_alphabet.as_bytes()), false);
        assert_eq!(result.is_err(), true);
    }

    #[test]
    fn test_base32_decode() {
        let data = "JBSWY3DPFQQHO33SNRSCC";
        let expected = b"Hello, world!";

        let result = decode(data, None, false).unwrap();
        assert_eq!(result.as_slice(), expected);
    }

    #[test]
    fn test_base32_decode_abc() {
        let data = "ABC";
        let expected = b"";

        let result = decode(data, None, false).unwrap();
        assert_eq!(result.as_slice(), expected);
    }

    #[test]
    fn test_base32_decode_padding() {
        let data = "JBSWY3DPFQQHO33SNRSCC===";
        let expected = b"Hello, world!";

        let result = decode(data, None, true).unwrap();
        assert_eq!(result.as_slice(), expected);
    }

    #[test]
    fn test_base32_decode_filecoin() {
        let alphabet = "abcdefghijklmnopqrstuvwxyz234567";
        let data = "g52w64jworydimrxov5hmn3gpj2gwyttnzxdmndjo5xxiztsojuxg5dxobzhs6i";
        let expected = b"7uoq6tp427uzv7fztkbsnn64iwotfrristwpryy";

        let result = decode(data, Some(alphabet.as_bytes()), false).unwrap();
        assert_eq!(result.as_slice(), expected);
    }
}
