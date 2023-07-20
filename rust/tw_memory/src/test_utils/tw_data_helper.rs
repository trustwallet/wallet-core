// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::ffi::tw_data::{
    tw_data_bytes, tw_data_create_with_bytes, tw_data_delete, tw_data_size, TWData,
};

pub struct TWDataHelper {
    ptr: *mut TWData,
}

impl TWDataHelper {
    pub fn create(bytes: Vec<u8>) -> Self {
        let ptr = unsafe { tw_data_create_with_bytes(bytes.as_ptr(), bytes.len()) };
        assert!(!ptr.is_null());
        TWDataHelper { ptr }
    }

    pub fn wrap(ptr: *mut TWData) -> Self {
        TWDataHelper { ptr }
    }

    pub fn ptr(&self) -> *mut TWData {
        self.ptr
    }

    pub fn is_null(&self) -> bool {
        self.ptr.is_null()
    }

    pub fn to_vec(&self) -> Option<Vec<u8>> {
        if self.ptr.is_null() {
            return None;
        }
        let bytes_ptr = unsafe { tw_data_bytes(self.ptr) };
        let len = unsafe { tw_data_size(self.ptr) };

        let bytes = unsafe { std::slice::from_raw_parts(bytes_ptr, len) }.to_vec();
        Some(bytes)
    }
}

impl Drop for TWDataHelper {
    fn drop(&mut self) {
        if self.ptr.is_null() {
            return;
        }
        unsafe { tw_data_delete(self.ptr) }
    }
}
