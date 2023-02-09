// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use std::ffi::{c_char, CStr};
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

#[cfg(test)]
mod tests {
    use std::ffi::CString;
    use crate::encoding::hex::{decode_hex};

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
