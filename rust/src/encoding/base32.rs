// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use std::ffi::{c_char, CStr, CString};
use crate::memory::CByteArray;

const ALPHABET_RFC4648: &[u8] = b"ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";

pub fn get_alphabet(alphabet: *const c_char) -> Option<&'static [u8]> {
    let alphabet = match alphabet.is_null() {
        false => {
            let alphabet = unsafe { CStr::from_ptr(alphabet).to_str().unwrap().as_bytes() };
            Some(alphabet)
        }
        true => None,
    };
    alphabet
}

fn base32_encode(input: &[u8], alphabet: Option<&[u8]>, padding: bool) -> Result<String, String> {
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

#[no_mangle]
pub extern "C" fn encode_base32(input: *const u8, input_len: usize, alphabet: *const c_char, padding: bool) -> *mut c_char {
    let input = unsafe { std::slice::from_raw_parts(input, input_len) };

    let alphabet = get_alphabet(alphabet);

    match base32_encode(input, alphabet, padding) {
        Ok(result) => CString::new(result).unwrap().into_raw(),
        Err(_) => std::ptr::null_mut(),
    }
}

fn base32_decode(input: &str, alphabet: Option<&[u8]>, padding: bool) -> Result<Vec<u8>, String> {
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

#[no_mangle]
pub extern "C" fn decode_base32(input: *const c_char, alphabet: *const c_char, padding: bool) -> CByteArray {
    let input = unsafe { CStr::from_ptr(input).to_str().unwrap() };
    let alphabet = get_alphabet(alphabet);

    match base32_decode(input, alphabet, padding) {
        Ok(decoded) => {
            decoded.into()
        }
        Err(_) => {
            CByteArray { data: std::ptr::null_mut(), size: 0 }
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_base32_encode() {
        let data = b"Hello, world!";
        let expected = "JBSWY3DPFQQHO33SNRSCC";

        let result = base32_encode(data, None, false).unwrap();
        assert_eq!(result, expected);
    }

    #[test]
    fn test_base32_encode_padding() {
        let data = b"Hello, world!";
        let expected = "JBSWY3DPFQQHO33SNRSCC===";

        let result = base32_encode(data, None, true).unwrap();
        assert_eq!(result, expected);
    }

    #[test]
    fn test_base32_encode_filecoin() {
        let alphabet = "abcdefghijklmnopqrstuvwxyz234567";
        let data = b"7uoq6tp427uzv7fztkbsnn64iwotfrristwpryy";
        let expected = "g52w64jworydimrxov5hmn3gpj2gwyttnzxdmndjo5xxiztsojuxg5dxobzhs6i";
        let result = base32_encode(data, Some(alphabet.as_bytes()), false).unwrap();
        assert_eq!(result, expected);

        let invalid_alphabet = "invalidalphabet";
        let result = base32_encode(data, Some(invalid_alphabet.as_bytes()), false);
        assert_eq!(result.is_err(), true);
    }

    #[test]
    fn test_base32_encode_ffi() {
        let data = b"Hello, world!";
        let expected_1 = "JBSWY3DPFQQHO33SNRSCC===";
        let expected_2 = "JBSWY3DPFQQHO33SNRSCC";
        let expected_3 = "jbswy3dpfqqho33snrscc===";
        let expected_4 = "jbswy3dpfqqho33snrscc";

        let result_ptr = encode_base32(data.as_ptr(), data.len(), std::ptr::null(), true);
        let result = unsafe { CString::from_raw(result_ptr) };
        assert_eq!(result.to_str().unwrap(), expected_1);

        let result_ptr = encode_base32(data.as_ptr(), data.len(), std::ptr::null(), false);
        let result = unsafe { CString::from_raw(result_ptr) };
        assert_eq!(result.to_str().unwrap(), expected_2);

        let alphabet = CString::new("abcdefghijklmnopqrstuvwxyz234567").unwrap();
        let result_ptr = encode_base32(data.as_ptr(), data.len(), alphabet.as_ptr(), true);
        let result = unsafe { CString::from_raw(result_ptr) };
        assert_eq!(result.to_str().unwrap(), expected_3);

        let result_ptr = encode_base32(data.as_ptr(), data.len(), alphabet.as_ptr(), false);
        let result = unsafe { CString::from_raw(result_ptr) };
        assert_eq!(result.to_str().unwrap(), expected_4);
    }

    #[test]
    fn test_base32_decode() {
        let data = "JBSWY3DPFQQHO33SNRSCC";
        let expected = b"Hello, world!";

        let result = base32_decode(data, None, false).unwrap();
        assert_eq!(result.as_slice(), expected);
    }

    #[test]
    fn test_base32_decode_abc() {
        let data = "ABC";
        let expected = b"";

        let result = base32_decode(data, None, false).unwrap();
        assert_eq!(result.as_slice(), expected);
    }

    #[test]
    fn test_base32_decode_padding() {
        let data = "JBSWY3DPFQQHO33SNRSCC===";
        let expected = b"Hello, world!";

        let result = base32_decode(data, None, true).unwrap();
        assert_eq!(result.as_slice(), expected);
    }

    #[test]
    fn test_base32_decode_filecoin() {
        let alphabet = "abcdefghijklmnopqrstuvwxyz234567";
        let data = "g52w64jworydimrxov5hmn3gpj2gwyttnzxdmndjo5xxiztsojuxg5dxobzhs6i";
        let expected = b"7uoq6tp427uzv7fztkbsnn64iwotfrristwpryy";

        let result = base32_decode(data, Some(alphabet.as_bytes()), false).unwrap();
        assert_eq!(result.as_slice(), expected);
    }

    #[test]
    fn test_base32_decode_ffi() {
        let input_1 = "JBSWY3DPFQQHO33SNRSCC===";
        let input_2 = "JBSWY3DPFQQHO33SNRSCC";
        let input_3 = "jbswy3dpfqqho33snrscc===";
        let input_4 = "jbswy3dpfqqho33snrscc";
        let expected = b"Hello, world!";


        let input_1 = CString::new(input_1).unwrap();
        let decoded_ptr = decode_base32(input_1.as_ptr(), std::ptr::null(), true);
        let decoded_slice = unsafe { std::slice::from_raw_parts(decoded_ptr.data, decoded_ptr.size) };
        assert_eq!(decoded_slice, expected);

        let input_2 = CString::new(input_2).unwrap();
        let decoded_ptr = decode_base32(input_2.as_ptr(), std::ptr::null(), false);
        let decoded_slice = unsafe { std::slice::from_raw_parts(decoded_ptr.data, decoded_ptr.size) };
        assert_eq!(decoded_slice, expected);


        let alphabet = CString::new("abcdefghijklmnopqrstuvwxyz234567").unwrap();
        let input_3 = CString::new(input_3).unwrap();
        let decoded_ptr = decode_base32(input_3.as_ptr(), alphabet.as_ptr(), true);
        let decoded_slice = unsafe { std::slice::from_raw_parts(decoded_ptr.data, decoded_ptr.size) };
        assert_eq!(decoded_slice, expected);

        let input_4 = CString::new(input_4).unwrap();
        let decoded_ptr = decode_base32(input_4.as_ptr(), alphabet.as_ptr(), false);
        let decoded_slice = unsafe { std::slice::from_raw_parts(decoded_ptr.data, decoded_ptr.size) };
        assert_eq!(decoded_slice, expected);
    }
}
