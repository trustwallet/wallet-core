// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_memory::ffi::c_byte_array::free_c_byte_array;
use tw_memory::ffi::CByteArray;

#[test]
fn test_free_c_byte_array() {
    unsafe {
        free_c_byte_array(std::ptr::null_mut());

        let raw_array = CByteArray::new_ptr(vec![1, 2, 3]);
        free_c_byte_array(raw_array);

        // The following leads to an undefined behaviour.
        // free_c_byte_array(raw_array);
    }
}
