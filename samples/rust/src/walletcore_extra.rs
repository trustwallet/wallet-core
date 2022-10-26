// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

// Extra type helpers, traits for wallet-core interfaces

use crate::walletcore_iface::*;
use hex::ToHex;
use std::ffi::CString;
use std::ffi::CStr;


// Some traits
pub trait FromString {
    fn from_str(s: &str) -> Self;
}

pub trait ToVec {
    fn to_vec(self: &Self) -> Vec<u8>;
}

pub trait FromVec {
    fn from_vec(vec: &Vec<u8>) -> Self;
}

pub trait ToHexString {
    fn to_hex(self: &Self) -> String;
}


// TWString trait implementations
impl FromString for TWString {
    fn from_str(s: &str) -> Self {
        let cstring = CString::new(s).unwrap();
        tw_string_create_with_utf8_bytes(cstring.as_ptr())
    }
}

impl ToString for TWString {
    fn to_string(&self) -> String {
        let s1 = tw_string_utf8_bytes(&self);
        let c_str: &CStr = unsafe { CStr::from_ptr(s1) };
        let str_slice: &str = c_str.to_str().unwrap();
        str_slice.to_owned()
    }
}


// TWData trait implementations
impl ToVec for TWData {
    fn to_vec(&self) -> Vec<u8> {
        tw_data_bytes(&self)
    }
}

impl FromVec for TWData {
    fn from_vec(v: &Vec<u8>) -> Self {
        tw_data_create_with_bytes(v)
    }
}

impl ToHexString for TWData {
    fn to_hex(&self) -> String {
        self.to_vec().encode_hex::<String>()
    }
}
