// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use std::ffi::{c_char, CString};
use tw_move_parser::ffi::{parse_function_argument_to_bcs, parse_type_tag, ETypeTag};

#[test]
fn tests_type_tag() {
    let tag = unsafe { parse_type_tag("0x1::aptos_coin::AptosCoin\0".as_ptr() as *const c_char) };
    assert_eq!(tag, ETypeTag::Struct);
}

#[test]
fn tests_function_argument_to_bcs() {
    let str = unsafe {
        let input = "10000000\0".as_ptr() as *const c_char;
        CString::from_raw(parse_function_argument_to_bcs(input).unwrap() as *mut c_char)
            .into_string()
            .unwrap()
    };
    assert_eq!(str, "8096980000000000");

    let str = unsafe {
        let input = "5047445908\0".as_ptr() as *const c_char;
        CString::from_raw(parse_function_argument_to_bcs(input).unwrap() as *mut c_char)
            .into_string()
            .unwrap()
    };
    assert_eq!(str, "94e9d92c01000000");
}

#[test]
fn tests_function_argument_to_bcs_another() {
    let str = unsafe {
        let input = "0xc95db29a67a848940829b3df6119b5e67b788ff0248676e4484c7c6f29c0f5e6\0".as_ptr()
            as *const c_char;
        CString::from_raw(parse_function_argument_to_bcs(input).unwrap() as *mut c_char)
            .into_string()
            .unwrap()
    };
    let decoded = hex::decode(str).unwrap();
    let v = vec![decoded];
    let actual = hex::encode(bcs::to_bytes(&v).unwrap());
    let expected = "012120c95db29a67a848940829b3df6119b5e67b788ff0248676e4484c7c6f29c0f5e6";
    assert_eq!(actual, expected);
}
