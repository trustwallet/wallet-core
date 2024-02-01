// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#![allow(clippy::missing_safety_doc)]

use std::ffi::{c_char, CString};

pub mod c_byte_array;
pub mod c_byte_array_ref;
pub mod c_result;
pub mod tw_data;
pub mod tw_data_vector;
pub mod tw_string;

/// Releases the memory previously allocated for the `ptr` string.
/// \param ptr *non-null* C-compatible, nul-terminated string.
#[no_mangle]
pub unsafe extern "C" fn free_string(ptr: *const c_char) {
    // Take the ownership back to rust and drop the owner.
    let _ = CString::from_raw(ptr as *mut _);
}

pub trait RawPtrTrait: Sized {
    fn into_ptr(self) -> *mut Self {
        Box::into_raw(Box::new(self))
    }

    unsafe fn from_ptr(raw: *mut Self) -> Option<Self> {
        if raw.is_null() {
            return None;
        }
        Some(*Box::from_raw(raw))
    }

    unsafe fn from_ptr_as_ref(raw: *const Self) -> Option<&'static Self> {
        if raw.is_null() {
            return None;
        }
        Some(&*raw)
    }

    unsafe fn from_ptr_as_mut(raw: *mut Self) -> Option<&'static mut Self> {
        if raw.is_null() {
            return None;
        }
        Some(&mut *raw)
    }

    unsafe fn from_ptr_as_box(raw: *mut Self) -> Option<Box<Self>> {
        if raw.is_null() {
            return None;
        }
        Some(Box::from_raw(raw))
    }
}
