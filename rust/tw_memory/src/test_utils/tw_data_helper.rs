// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::ffi::tw_data::{
    tw_data_bytes, tw_data_create_with_bytes, tw_data_delete, tw_data_size, TWData,
};
use crate::Data;

pub struct TWDataHelper {
    ptr: *mut TWData,
}

impl TWDataHelper {
    pub fn create(bytes: Data) -> Self {
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

    pub fn to_vec(&self) -> Option<Data> {
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
