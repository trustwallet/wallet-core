// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::ffi::CStr;
use std::ffi::CString;
use tw_memory::ffi::tw_result::{
    tw_result_void_create_error, tw_result_void_create_ok, tw_result_void_delete,
    tw_result_void_get_err, tw_result_void_is_err, tw_result_void_is_success,
};
use tw_memory::ffi::tw_string::{
    tw_string_create_with_utf8_bytes, tw_string_delete, tw_string_utf8_bytes,
};

unsafe fn take_string(ptr: *mut tw_memory::ffi::tw_string::TWString) -> String {
    assert!(!ptr.is_null());
    let c_str = CStr::from_ptr(tw_string_utf8_bytes(ptr));
    let s = c_str.to_str().expect("valid UTF-8").to_owned();
    tw_string_delete(ptr);
    s
}

#[test]
fn test_create_ok_is_success() {
    unsafe {
        let result = tw_result_void_create_ok();
        assert!(!result.is_null());
        assert!(tw_result_void_is_success(result));
        assert!(!tw_result_void_is_err(result));
        tw_result_void_delete(result);
    }
}

#[test]
fn test_create_ok_get_err_is_null() {
    unsafe {
        let result = tw_result_void_create_ok();
        let err_str = tw_result_void_get_err(result);
        assert!(err_str.is_null());
        tw_result_void_delete(result);
    }
}

#[test]
fn test_create_error_is_err() {
    unsafe {
        let cstr = CString::new("something went wrong").unwrap();
        let tw_str = tw_string_create_with_utf8_bytes(cstr.as_ptr());
        let result = tw_result_void_create_error(tw_str);
        tw_string_delete(tw_str);

        assert!(!result.is_null());
        assert!(!tw_result_void_is_success(result));
        assert!(tw_result_void_is_err(result));
        tw_result_void_delete(result);
    }
}

#[test]
fn test_create_error_get_err_message() {
    unsafe {
        let cstr = CString::new("something went wrong").unwrap();
        let tw_str = tw_string_create_with_utf8_bytes(cstr.as_ptr());
        let result = tw_result_void_create_error(tw_str);
        tw_string_delete(tw_str);

        let err_str = tw_result_void_get_err(result);
        assert_eq!(take_string(err_str), "something went wrong");
        tw_result_void_delete(result);
    }
}

#[test]
fn test_create_error_empty_message() {
    unsafe {
        let cstr = CString::new("").unwrap();
        let tw_str = tw_string_create_with_utf8_bytes(cstr.as_ptr());
        let result = tw_result_void_create_error(tw_str);
        tw_string_delete(tw_str);

        assert!(tw_result_void_is_err(result));
        let err_str = tw_result_void_get_err(result);
        assert_eq!(take_string(err_str), "");
        tw_result_void_delete(result);
    }
}

#[test]
fn test_null_ptr_is_success_returns_false() {
    unsafe {
        assert!(!tw_result_void_is_success(std::ptr::null()));
    }
}

#[test]
fn test_null_ptr_is_err_returns_false() {
    unsafe {
        assert!(!tw_result_void_is_err(std::ptr::null()));
    }
}

#[test]
fn test_null_ptr_get_err_returns_null() {
    unsafe {
        let err_str = tw_result_void_get_err(std::ptr::null());
        assert!(err_str.is_null());
    }
}

#[test]
fn test_delete_ok() {
    unsafe {
        let result = tw_result_void_create_ok();
        assert!(!result.is_null());
        tw_result_void_delete(result);
        // No double-free: ownership is consumed by delete.
    }
}

#[test]
fn test_delete_error() {
    unsafe {
        let cstr = CString::new("error").unwrap();
        let tw_str = tw_string_create_with_utf8_bytes(cstr.as_ptr());
        let result = tw_result_void_create_error(tw_str);
        tw_string_delete(tw_str);
        tw_result_void_delete(result);
    }
}
