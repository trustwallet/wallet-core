// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#![allow(clippy::missing_safety_doc)]

use std::ffi::{c_char, CString};

pub mod c_byte_array;
pub mod c_byte_array_ref;
pub mod c_result;

/// Releases the memory previously allocated for the `ptr` string.
/// \param ptr *non-null* C-compatible, nul-terminated string.
#[no_mangle]
pub unsafe extern "C" fn free_string(ptr: *const c_char) {
    // Take the ownership back to rust and drop the owner
    let _ = CString::from_raw(ptr as *mut _);
}
