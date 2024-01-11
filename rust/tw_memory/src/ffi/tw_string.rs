// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::ffi::RawPtrTrait;
use std::ffi::{c_char, CStr, CString};

/// Defines a resizable string.
///
/// The implementation of these methods should be language-specific to minimize translation overhead.
/// For instance it should be a `jstring` for Java and an `NSString` for Swift.
/// Create allocates memory, the delete call should be called at the end to release memory.
#[derive(Debug, Default)]
pub struct TWString(CString);

impl TWString {
    /// Returns an empty `TWString` instance.
    pub fn new() -> TWString {
        TWString(CString::default())
    }

    /// Creates a `TWString` from a null-terminated UTF8 byte array.
    pub unsafe fn from_c_str(ptr: *const c_char) -> Option<TWString> {
        if ptr.is_null() {
            return None;
        }
        let str = CStr::from_ptr(ptr);
        Some(TWString(CString::from(str)))
    }

    /// Converts `TWString` into `String` without additional allocation.
    pub fn into_string(self) -> Option<String> {
        self.0.into_string().ok()
    }

    /// Returns a string slice.
    pub fn as_str(&self) -> Option<&str> {
        self.0.to_str().ok()
    }

    /// Returns the const pointer to the string.
    pub fn as_c_char(&self) -> *const c_char {
        self.0.as_ptr()
    }
}

impl From<String> for TWString {
    fn from(s: String) -> Self {
        TWString(CString::new(s).expect("CString::new(String) should never fail"))
    }
}

impl RawPtrTrait for TWString {}

/// Creates a `TWString` from a null-terminated UTF8 byte array. It must be deleted at the end.
/// \param bytes a null-terminated UTF8 byte array.
#[no_mangle]
pub unsafe extern "C" fn tw_string_create_with_utf8_bytes(bytes: *const c_char) -> *mut TWString {
    TWString::from_c_str(bytes)
        .map(TWString::into_ptr)
        .unwrap_or_else(std::ptr::null_mut)
}

/// Returns the raw pointer to the string's UTF8 bytes (null-terminated).
/// \param str a TWString pointer.
#[no_mangle]
pub unsafe extern "C" fn tw_string_utf8_bytes(str: *const TWString) -> *const c_char {
    TWString::from_ptr_as_ref(str)
        .map(|str| str.as_c_char())
        .unwrap_or_else(std::ptr::null)
}

/// Deletes a string created with a `TWStringCreate*` method and frees the memory.
/// \param str a `TWString` pointer.
#[no_mangle]
pub unsafe extern "C" fn tw_string_delete(str: *mut TWString) {
    let _ = TWString::from_ptr(str);
}
