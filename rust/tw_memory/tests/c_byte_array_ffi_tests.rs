// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_memory::ffi::c_byte_array::{free_c_byte_array, CByteArray};

#[test]
fn test_free_c_byte_array() {
    unsafe {
        free_c_byte_array(std::ptr::null_mut());

        let mut raw_array = CByteArray::from(vec![1, 2, 3]);
        free_c_byte_array(&mut raw_array as *mut CByteArray);

        // The following leads to an undefined behaviour.
        // free_c_byte_array(raw_array);
    }
}

#[test]
fn test_drop_c_byte_array() {
    // The memory must be released on `Drop::drop`.
    let _ = CByteArray::from(vec![1, 2, 3]);
}

#[test]
fn test_c_byte_array_into_vec() {
    // The memory must be valid after `CByteArray::into_vec` and `CByteArray::drop`.
    let data = unsafe { CByteArray::from(vec![1, 2, 3]).into_vec() };
    assert_eq!(data, [1, 2, 3]);
}
