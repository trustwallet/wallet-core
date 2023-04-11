// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use std::ffi::{c_char, CString};

pub mod ffi;

pub fn c_string_standalone<S: Into<String>>(input: S) -> *const c_char {
    let res = CString::new(input.into()).unwrap();
    let p = res.as_ptr();
    std::mem::forget(res);
    p
}
