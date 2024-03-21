// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#![allow(clippy::missing_safety_doc)]

use std::ffi::{c_char, CString};

/// Creates a random UUID.
/// This uses the [`getrandom`] crate to utilise the operating system's RNG
/// as the source of random numbers.
#[no_mangle]
pub unsafe extern "C" fn tw_uuid_random() -> *const c_char {
    let res = uuid::Uuid::new_v4();
    CString::new(res.to_string()).unwrap().into_raw()
}
