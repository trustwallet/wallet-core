// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use std::ffi::{c_char, CString};

#[no_mangle]
pub unsafe extern fn free_string(ptr: *const c_char) {
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
    use crate::memory::{c_string_standalone, free_string};

    #[test]
    fn tests_ffi_string() {
        unsafe { free_string(c_string_standalone("foo".to_string())); }
    }
}
