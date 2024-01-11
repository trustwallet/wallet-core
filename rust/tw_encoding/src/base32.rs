// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::{EncodingError, EncodingResult};
use data_encoding::{Encoding, Specification};
use std::cell::RefCell;
use std::collections::hash_map::Entry;
use std::collections::HashMap;

/// cbindgen:ignore
const ALPHABET_RFC4648: &str = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";

type EncodingMap = HashMap<EncodingParams, Encoding>;

thread_local! {
    /// Cache the encodings to avoid parsing already handled alphabets on each operation.
    static ENCODINGS: RefCell<EncodingMap> = RefCell::new(HashMap::new());
}

#[derive(Debug, Clone, Eq, Hash, PartialEq)]
struct EncodingParams {
    alphabet: String,
    padding: bool,
}

pub fn encode(input: &[u8], alphabet: Option<String>, padding: bool) -> EncodingResult<String> {
    let encoding = get_encoding(alphabet, padding)?;
    Ok(encoding.encode(input))
}

pub fn decode(input: &str, alphabet: Option<String>, padding: bool) -> EncodingResult<Vec<u8>> {
    let encoding = get_encoding(alphabet, padding)?;
    encoding
        .decode(input.as_bytes())
        .map_err(|_| EncodingError::InvalidInput)
}

fn get_encoding(alphabet: Option<String>, padding: bool) -> EncodingResult<Encoding> {
    let alphabet = alphabet.unwrap_or_else(|| ALPHABET_RFC4648.to_string());
    let encoding_params = EncodingParams { alphabet, padding };

    ENCODINGS.with(
        |encodings| match encodings.borrow_mut().entry(encoding_params.clone()) {
            Entry::Occupied(ready) => Ok(ready.get().clone()),
            Entry::Vacant(vacant) => {
                let new_encoding = create_encoding(encoding_params)?;
                Ok(vacant.insert(new_encoding).clone())
            },
        },
    )
}

fn create_encoding(params: EncodingParams) -> EncodingResult<Encoding> {
    let mut specification = Specification::new();
    specification.symbols = params.alphabet;
    if params.padding {
        specification.padding = Some('=');
    }

    specification
        .encoding()
        .map_err(|_| EncodingError::InvalidAlphabet)
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
        let result = encode(data, Some(alphabet.to_string()), false).unwrap();
        assert_eq!(result, expected);

        let invalid_alphabet = "invalidalphabet";
        let result = encode(data, Some(invalid_alphabet.to_string()), false);
        assert_eq!(result, Err(EncodingError::InvalidAlphabet));
    }

    #[test]
    fn test_base32_decode() {
        let data = "JBSWY3DPFQQHO33SNRSCC";
        let expected = b"Hello, world!";

        let result = decode(data, None, false).unwrap();
        assert_eq!(result.as_slice(), expected);
    }

    #[test]
    fn test_base32_decode_padding() {
        let data = "JBSWY3DPFQQHO33SNRSCC===";
        let expected = b"Hello, world!";

        let result = decode(data, None, true).unwrap();
        assert_eq!(result.as_slice(), expected);

        let data = "JBSWY3DPFQQHO33SNRSCC";
        decode(data, None, true).expect_err("No padding while it's required");
    }

    #[test]
    fn test_base32_decode_filecoin() {
        let alphabet = "abcdefghijklmnopqrstuvwxyz234567";
        let data = "g52w64jworydimrxov5hmn3gpj2gwyttnzxdmndjo5xxiztsojuxg5dxobzhs6i";
        let expected = b"7uoq6tp427uzv7fztkbsnn64iwotfrristwpryy";

        let result = decode(data, Some(alphabet.to_string()), false).unwrap();
        assert_eq!(result.as_slice(), expected);
    }

    #[test]
    fn test_decode_invalid() {
        const BASE32_ALPHABET_FILECOIN: &str = "abcdefghijklmnopqrstuvwxyz234567";

        decode("+-", None, false).unwrap_err();
        decode("A", None, false).unwrap_err();
        decode("ABC", None, false).unwrap_err();
        decode(
            "rw6wy7w6sbsguyn3yzeygg34fgf72n5ao5sxykz",
            Some(BASE32_ALPHABET_FILECOIN.to_string()),
            false,
        )
        .unwrap_err();
    }
}
