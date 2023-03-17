// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use std::ffi::{c_char, CString};

/// TODO declare `free_cbyte_array`, `is_cbyte_array_null` FFI.
#[repr(C)]
pub struct CByteArray {
    pub data: *mut u8,
    pub size: usize,
}

impl From<Vec<u8>> for CByteArray {
    fn from(value: Vec<u8>) -> Self {
        let size = value.len();
        let mut mut_vec = value.clone();
        let ptr = mut_vec.as_mut_ptr();
        std::mem::forget(mut_vec);
        CByteArray { data: ptr, size }
    }
}

impl CByteArray {
    pub fn null() -> CByteArray {
        CByteArray {
            data: std::ptr::null_mut(),
            size: 0,
        }
    }
}

/// Releases the memory previously allocated for the `ptr` string.
/// \param ptr *non-null* C-compatible, nul-terminated string.
#[no_mangle]
pub unsafe extern "C" fn free_string(ptr: *const c_char) {
    // Take the ownership back to rust and drop the owner
    let _ = CString::from_raw(ptr as *mut _);
}

pub fn c_string_standalone(input: String) -> *const c_char {
    let res = CString::new(input).unwrap();
    let p = res.as_ptr();
    std::mem::forget(res);
    p
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn tests_ffi_string() {
        unsafe {
            free_string(c_string_standalone("foo".to_string()));
        }
    }
}
