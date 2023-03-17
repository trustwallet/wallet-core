// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use base64::{Engine as _, engine::{general_purpose}};
use std::ffi::{CStr, CString};
use std::os::raw::c_char;
use tw_memory::CByteArray;

/// Encodes the `data` data as a padded, base64 string.
/// \param data *non-null* byte array.
/// \param len - the length of the `data` array.
/// \param is_url whether to use the [URL safe alphabet](https://www.rfc-editor.org/rfc/rfc3548#section-4).
/// \return *non-null* C-compatible, nul-terminated string.
#[no_mangle]
pub extern "C" fn encode_base64(data: *const u8, len: usize, is_url: bool) -> *mut c_char {
    let data = unsafe { std::slice::from_raw_parts(data, len) };
    let encoded = if is_url {
        general_purpose::URL_SAFE.encode(data)
    } else {
        general_purpose::STANDARD.encode(data)
    };
    CString::new(encoded).unwrap().into_raw()
}

/// Decodes the base64 `data` string.
/// \param data *non-null* C-compatible, nul-terminated string.
/// \param is_url whether to use the [URL safe alphabet](https://www.rfc-editor.org/rfc/rfc3548#section-4).
/// \return C-compatible byte array.
#[no_mangle]
pub extern "C" fn decode_base64(data: *const c_char, is_url: bool) -> CByteArray {
    if data.is_null() {
        return CByteArray { data: std::ptr::null_mut(), size: 0 };
    }
    let c_str = unsafe { CStr::from_ptr(data) };
    let str_slice = c_str.to_str().unwrap();
    let decoded = if is_url {
        general_purpose::URL_SAFE
            .decode(str_slice)
    } else {
        general_purpose::STANDARD
            .decode(str_slice)
    };
    let decoded = match decoded {
        Ok(decoded) => decoded,
        Err(_) => return CByteArray { data: std::ptr::null_mut(), size: 0 }
    };
    let size = decoded.len();
    let mut decoded_vec = decoded.to_vec();
    let ptr = decoded_vec.as_mut_ptr();
    std::mem::forget(decoded_vec);
    CByteArray { data: ptr, size }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_encode_base64_ffi() {
        let data = b"hello world";
        let encoded = unsafe {
            CStr::from_ptr(encode_base64(data.as_ptr(), data.len(), false))
        };
        let expected = "aGVsbG8gd29ybGQ=";
        assert_eq!(encoded.to_str().unwrap(), expected);
    }

    #[test]
    fn test_encode_base64_url_ffi() {
        let data = b"+'?ab";
        let encoded = unsafe {
            CStr::from_ptr(encode_base64(data.as_ptr(), data.len(), true))
        };
        let expected = "Kyc_YWI=";
        assert_eq!(encoded.to_str().unwrap(), expected);
    }

    #[test]
    fn test_decode_base64_url() {
        let encoded = "Kyc_YWI=";
        let expected = b"+'?ab";

        let encoded_c_str = CString::new(encoded).unwrap();
        let encoded_ptr = encoded_c_str.as_ptr();

        let decoded_ptr = decode_base64(encoded_ptr, true);
        let decoded_slice = unsafe { std::slice::from_raw_parts(decoded_ptr.data, decoded_ptr.size) };

        assert_eq!(decoded_slice, expected);
    }

    #[test]
    fn test_decode_base64() {
        let encoded = "aGVsbG8gd29ybGQh";
        let expected = b"hello world!";

        let encoded_c_str = CString::new(encoded).unwrap();
        let encoded_ptr = encoded_c_str.as_ptr();

        let decoded_ptr = decode_base64(encoded_ptr, false);
        let decoded_slice = unsafe { std::slice::from_raw_parts(decoded_ptr.data, decoded_ptr.size) };

        assert_eq!(decoded_slice, expected);
    }

    #[test]
    fn test_decode_base64_invalid() {
        let invalid_encoded = "_This_is_an_invalid_base64_";
        let encoded_c_str = CString::new(invalid_encoded).unwrap();
        let encoded_ptr = encoded_c_str.as_ptr();
        let decoded_ptr = decode_base64(encoded_ptr, false);
        assert_eq!(decoded_ptr.data.is_null(), true);
    }
}

