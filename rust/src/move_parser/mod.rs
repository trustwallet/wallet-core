// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use std::{
    ffi::c_char,
    ffi::CStr
};
use move_core_types::*;
use move_core_types::language_storage::TypeTag;
use move_core_types::transaction_argument::TransactionArgument;
use crate::memory;

#[repr(C)]
#[derive(PartialEq, Debug)]
pub enum ETypeTag {
    Bool = 1,
    U8 = 2,
    U64 = 3,
    U128 = 4,
    Address = 5,
    Signer = 6,
    Vector = 7,
    Struct = 8,
    Error = 9
}

#[no_mangle]
pub extern fn parse_type_tag(input: *const c_char) -> ETypeTag {
    let s = unsafe { CStr::from_ptr(input).to_str().unwrap() };
    let transaction_argument = match parser::parse_type_tag(s) {
        Ok(v) => v,
        Err(_) => return ETypeTag::Error
    };
    return match transaction_argument {
        TypeTag::Bool => ETypeTag::Bool,
        TypeTag::U8 => ETypeTag::U8,
        TypeTag::U64 => ETypeTag::U64,
        TypeTag::U128 => ETypeTag::U128,
        TypeTag::Address => ETypeTag::Address,
        TypeTag::Signer => ETypeTag::Signer,
        TypeTag::Vector(_) => ETypeTag::Vector,
        TypeTag::Struct(_) => ETypeTag::Struct
    }
}

#[no_mangle]
pub extern fn parse_function_argument_to_bcs(input: *const c_char) -> *const c_char {
    let s = unsafe { CStr::from_ptr(input).to_str().unwrap() };
    let transaction_argument = match parser::parse_transaction_argument(s) {
        Ok(v) => v,
        Err(_) => return "\0".as_ptr() as *const c_char
    };
    let v = match transaction_argument {
        TransactionArgument::U8(v) => hex::encode(bcs::to_bytes(&v).unwrap()),
        TransactionArgument::U64(v) => hex::encode(bcs::to_bytes(&v).unwrap()),
        TransactionArgument::U128(v) => hex::encode(bcs::to_bytes(&v).unwrap()),
        TransactionArgument::Address(v) => hex::encode(bcs::to_bytes(&v).unwrap()),
        TransactionArgument::U8Vector(v) => hex::encode(bcs::to_bytes(&v).unwrap()),
        TransactionArgument::Bool(v) => hex::encode(bcs::to_bytes(&v).unwrap()),
    };
    memory::c_string_standalone(v)
}

#[cfg(test)]
mod tests {
    use crate::move_parser::{ETypeTag, parse_function_argument_to_bcs, parse_type_tag};
    use std::ffi::CStr;
    use std::ffi::c_char;

    #[test]
    fn tests_type_tag() {
        let tag = parse_type_tag("0x1::aptos_coin::AptosCoin\0".as_ptr() as *const c_char);
        assert_eq!(tag, ETypeTag::Struct);
    }

    #[test]
    fn tests_function_argument_to_bcs() {
        let str = unsafe { CStr::from_ptr(parse_function_argument_to_bcs("10000000\0".as_ptr() as *const c_char)).to_str().unwrap() };
        assert_eq!(str, "8096980000000000");

        let str = unsafe { CStr::from_ptr(parse_function_argument_to_bcs("5047445908\0".as_ptr() as *const c_char)).to_str().unwrap() };
        assert_eq!(str, "94e9d92c01000000");
    }
}
