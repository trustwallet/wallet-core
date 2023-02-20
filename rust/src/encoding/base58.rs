// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use std::ffi::{c_char, CStr, CString};
use bs58::{encode, decode, Alphabet};
use crate::memory::CByteArray;

#[repr(C)]
#[derive(PartialEq, Debug)]
pub enum Base58Alphabet {
    Bitcoin = 1,
    Ripple = 2,
}

impl From<Base58Alphabet> for &Alphabet {
    fn from(value: Base58Alphabet) -> Self {
        match value {
            Base58Alphabet::Bitcoin => Alphabet::BITCOIN,
            Base58Alphabet::Ripple => Alphabet::RIPPLE
        }
    }
}

fn base58_encode(input: &[u8], alphabet: Base58Alphabet) -> String {
    encode(input)
        .with_alphabet(alphabet.into())
        .into_string()
}

#[no_mangle]
pub extern "C" fn encode_base58(input: *const u8, input_len: usize, alphabet: Base58Alphabet) -> *mut c_char {
    let input = unsafe { std::slice::from_raw_parts(input, input_len) };
    CString::new(base58_encode(input, alphabet)).unwrap().into_raw()
}

fn base58_decode(input: &str, alphabet: Base58Alphabet) -> decode::Result<Vec<u8>> {
    decode(input).with_alphabet(alphabet.into()).into_vec()
}

#[no_mangle]
pub extern "C" fn decode_base58(input: *const c_char, alphabet: Base58Alphabet) -> CByteArray {
    let input = unsafe { CStr::from_ptr(input).to_str().unwrap() };

    match base58_decode(input, alphabet) {
        Ok(decoded) => {
           decoded.into()
        },
        Err(_) => {
            CByteArray { data: std::ptr::null_mut(), size: 0 }
        },
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_base58_encode() {
        let data = b"Hello, world!";
        let expected = "72k1xXWG59wUsYv7h2";

        let result = base58_encode(data, Base58Alphabet::Bitcoin);
        assert_eq!(result, expected);
    }

    #[test]
    fn test_base58_decode() {
        let data = "72k1xXWG59wUsYv7h2";
        let expected = b"Hello, world!";

        let result = base58_decode(data, Base58Alphabet::Bitcoin).unwrap();
        assert_eq!(result, expected.to_vec());
    }

    #[test]
    fn test_base58_encode_ffi() {
        let data = b"Hello, world!";
        let expected = "72k1xXWG59wUsYv7h2";

        let result_ptr = encode_base58(data.as_ptr(), data.len(), Base58Alphabet::Bitcoin);
        let result = unsafe { CString::from_raw(result_ptr) };
        assert_eq!(result.to_str().unwrap(), expected);
    }

    #[test]
    fn test_base58_decode_ffi() {
        let data = "72k1xXWG59wUsYv7h2";
        let expected = b"Hello, world!";

        let input = CString::new(data).unwrap();
        let decoded_ptr = decode_base58(input.as_ptr(), Base58Alphabet::Bitcoin);
        let decoded_slice = unsafe { std::slice::from_raw_parts(decoded_ptr.data, decoded_ptr.size) };
        assert_eq!(decoded_slice, expected);
    }
}
