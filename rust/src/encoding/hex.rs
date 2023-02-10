// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use std::ffi::{c_char, CStr, CString};
use crate::memory::CByteArray;
use hex;

#[no_mangle]
pub extern "C" fn decode_hex(data: *const c_char) -> CByteArray {
    if data.is_null() {
        return CByteArray { data: std::ptr::null_mut(), size: 0 };
    }
    let c_str = unsafe { CStr::from_ptr(data) };
    let hex_string = c_str.to_str().unwrap();
    let hex_string = if hex_string.starts_with("0x") { &hex_string[2..] } else { hex_string };

    return match hex::decode(hex_string) {
        Ok(mut decoded) => {
            let size = decoded.len();
            let ptr = decoded.as_mut_ptr();
            std::mem::forget(decoded);
            CByteArray { data: ptr, size }
        }
        Err(_) => {
            CByteArray { data: std::ptr::null_mut(), size: 0 }
        }
    }
}

#[no_mangle]
pub extern "C" fn encode_hex(data: *const u8, len: usize, prefixed: bool) -> *mut c_char {
    let data = unsafe { std::slice::from_raw_parts(data, len) };
    let mut encoded = hex::encode(data);
    if prefixed {
        encoded = "0x".to_owned() + &encoded;
    }
    CString::new(encoded).unwrap().into_raw()
}

#[cfg(test)]
mod tests {
    use std::ffi::CString;
    use crate::encoding::hex::{encode_hex, decode_hex};

    #[test]
    fn test_encode_hex_without_prefix() {
        let data = b"hello world";
        let encoded = unsafe {
            std::ffi::CStr::from_ptr(encode_hex(data.as_ptr(), data.len(), false))
        };
        let expected = "68656c6c6f20776f726c64";
        assert_eq!(encoded.to_str().unwrap(), expected);
    }

    #[test]
    fn test_encode_hex_with_prefix() {
        let data = b"hello world";
        let encoded = unsafe {
            std::ffi::CStr::from_ptr(encode_hex(data.as_ptr(), data.len(), true))
        };
        let expected = "0x68656c6c6f20776f726c64";
        assert_eq!(encoded.to_str().unwrap(), expected);
    }

    #[test]
    fn test_vec_encode() {
        let v: Vec<u8> = vec![45,181,0,172,145,156,221,227,81,172,54,227,113,29,131,44,109,185,118,105];
        assert_eq!(hex::encode(v), "2db500ac919cdde351ac36e3711d832c6db97669");
    }

    #[test]
    fn test_decode_hex() {
        let encoded = "7d8bf18c7ce84b3e175b339c4ca93aed1dd166f1";

        let encoded_c_str = CString::new(encoded).unwrap();
        let encoded_ptr = encoded_c_str.as_ptr();

        let decoded_ptr = decode_hex(encoded_ptr);
        let decoded_slice = unsafe { std::slice::from_raw_parts(decoded_ptr.data, decoded_ptr.size) };

        assert_eq!(decoded_slice.is_empty(), false);
    }

    #[test]
    fn test_decode_hex_with_prefix() {
        let encoded = "0x7d8bf18c7ce84b3e175b339c4ca93aed1dd166f1";

        let encoded_c_str = CString::new(encoded).unwrap();
        let encoded_ptr = encoded_c_str.as_ptr();

        let decoded_ptr = decode_hex(encoded_ptr);
        let decoded_slice = unsafe { std::slice::from_raw_parts(decoded_ptr.data, decoded_ptr.size) };

        assert_eq!(decoded_slice.is_empty(), false);
    }
}
