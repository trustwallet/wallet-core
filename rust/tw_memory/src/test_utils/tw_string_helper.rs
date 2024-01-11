// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::ffi::tw_string::{
    tw_string_create_with_utf8_bytes, tw_string_delete, tw_string_utf8_bytes, TWString,
};
use std::ffi::{CStr, CString};

pub struct TWStringHelper {
    ptr: *mut TWString,
}

impl TWStringHelper {
    pub fn create(s: &str) -> TWStringHelper {
        let cstring = CString::new(s).unwrap();
        let ptr = unsafe { tw_string_create_with_utf8_bytes(cstring.as_ptr()) };
        assert!(!ptr.is_null());
        TWStringHelper { ptr }
    }

    pub fn wrap(ptr: *mut TWString) -> Self {
        TWStringHelper { ptr }
    }

    pub fn ptr(&self) -> *mut TWString {
        self.ptr
    }

    pub fn to_string(&self) -> Option<String> {
        if self.ptr.is_null() {
            return None;
        }
        let c_str = unsafe { tw_string_utf8_bytes(self.ptr) };
        let str = unsafe { CStr::from_ptr(c_str) }
            .to_str()
            .expect("Received an invalid c_str")
            .to_string();
        Some(str)
    }
}

impl Drop for TWStringHelper {
    fn drop(&mut self) {
        if self.ptr.is_null() {
            return;
        }
        unsafe { tw_string_delete(self.ptr) }
    }
}
