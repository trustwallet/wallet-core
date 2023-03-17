// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

// TODO declare `free_c_byte_array`, `is_c_byte_array_null` FFI.
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
