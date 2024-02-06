// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_memory::ffi::c_byte_array::{CByteArray, CByteArrayResult};
use tw_memory::ffi::c_result::{OK_CODE, UNKNOWN_ERROR};

#[test]
fn test_c_result_unwrap() {
    let c_res = CByteArrayResult::ok(CByteArray::from(vec![1, 2, 3]));
    assert!(c_res.is_ok());
    assert!(!c_res.is_err());

    let actual = unsafe { c_res.unwrap().into_vec() };
    assert_eq!(actual, [1, 2, 3]);
}

#[test]
#[should_panic]
fn test_c_result_unwrap_error() {
    let c_res = CByteArrayResult::error(10);
    // Should panic.
    c_res.unwrap();
}

#[test]
fn test_c_result_error_with_ok_code() {
    let c_res = CByteArrayResult::error(OK_CODE);
    assert!(!c_res.is_ok());
    assert!(c_res.is_err());
    assert_eq!(c_res.code, UNKNOWN_ERROR);
}

#[test]
fn test_c_result_into_result() {
    let c_res = CByteArrayResult::ok(CByteArray::from(vec![1, 2, 3]));
    c_res.into_result().unwrap();

    let c_res = CByteArrayResult::error(10);
    c_res.into_result().unwrap_err();
}
