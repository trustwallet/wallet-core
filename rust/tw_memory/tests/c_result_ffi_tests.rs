// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_memory::ffi::c_byte_array::{CByteArray, CByteArrayResult};
use tw_memory::ffi::c_result::{OK_CODE, UNKNOWN_ERROR};

#[test]
fn test_c_result_unwrap() {
    let c_res = CByteArrayResult::ok(CByteArray::new(vec![1, 2, 3]));
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
    let c_res = CByteArrayResult::ok(CByteArray::new(vec![1, 2, 3]));
    c_res.into_result().unwrap();

    let c_res = CByteArrayResult::error(10);
    c_res.into_result().unwrap_err();
}
