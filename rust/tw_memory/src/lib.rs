// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::ffi::{c_char, CString};

pub type Data = Vec<u8>;

pub mod ffi;

#[cfg(feature = "test-utils")]
pub mod test_utils;

pub fn c_string_standalone<S: Into<String>>(input: S) -> *const c_char {
    let res = CString::new(input.into()).unwrap();
    let p = res.as_ptr();
    std::mem::forget(res);
    p
}
