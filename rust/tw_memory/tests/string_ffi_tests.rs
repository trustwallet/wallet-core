// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_memory::c_string_standalone;
use tw_memory::ffi::free_string;

#[test]
fn tests_ffi_string() {
    unsafe {
        free_string(c_string_standalone("foo"));
    }
}
