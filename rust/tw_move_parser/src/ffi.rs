// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#![allow(clippy::missing_safety_doc)]

use move_core_types::language_storage::TypeTag;
use move_core_types::transaction_argument::TransactionArgument;
use move_core_types::*;
use std::{ffi::c_char, ffi::CStr};
use tw_memory::ffi::c_result::{CStrResult, ErrorCode};

#[repr(C)]
pub enum CMoveParserCode {
    Ok = 0,
    InvalidInput = 1,
    ErrorParsing = 2,
}

impl From<CMoveParserCode> for ErrorCode {
    fn from(code: CMoveParserCode) -> Self {
        code as ErrorCode
    }
}

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
    Error = 9,
}

/// Parses a Move type tag.
/// \param input *non-null* C-compatible, nul-terminated string.
/// \return `ETypeTag` enumeration.
#[no_mangle]
pub unsafe extern "C" fn parse_type_tag(input: *const c_char) -> ETypeTag {
    let s = CStr::from_ptr(input).to_str().unwrap();
    let transaction_argument = match parser::parse_type_tag(s) {
        Ok(v) => v,
        Err(_) => return ETypeTag::Error,
    };
    match transaction_argument {
        TypeTag::Bool => ETypeTag::Bool,
        TypeTag::U8 => ETypeTag::U8,
        TypeTag::U64 => ETypeTag::U64,
        TypeTag::U128 => ETypeTag::U128,
        TypeTag::Address => ETypeTag::Address,
        TypeTag::Signer => ETypeTag::Signer,
        TypeTag::Vector(_) => ETypeTag::Vector,
        TypeTag::Struct(_) => ETypeTag::Struct,
    }
}

/// Parses `input` as a Move function argument.
/// \param input *non-null* C-compatible, nul-terminated string.
/// \return *non-null* C-compatible, nul-terminated string, Binary Canonical Serialization (BCS).
#[no_mangle]
pub unsafe extern "C" fn parse_function_argument_to_bcs(input: *const c_char) -> CStrResult {
    let s = match CStr::from_ptr(input).to_str() {
        Ok(input) => input,
        Err(_) => return CStrResult::error(CMoveParserCode::InvalidInput),
    };
    let transaction_argument = match parser::parse_transaction_argument(s) {
        Ok(v) => v,
        Err(_) => return CStrResult::error(CMoveParserCode::ErrorParsing),
    };
    let v = match transaction_argument {
        TransactionArgument::U8(v) => hex::encode(bcs::to_bytes(&v).unwrap()),
        TransactionArgument::U64(v) => hex::encode(bcs::to_bytes(&v).unwrap()),
        TransactionArgument::U128(v) => hex::encode(bcs::to_bytes(&v).unwrap()),
        TransactionArgument::Address(v) => {
            hex::encode(bcs::to_bytes(&bcs::to_bytes(&v).unwrap()).unwrap())
        },
        TransactionArgument::U8Vector(v) => hex::encode(bcs::to_bytes(&v).unwrap()),
        TransactionArgument::Bool(v) => hex::encode(bcs::to_bytes(&v).unwrap()),
    };
    CStrResult::ok(tw_memory::c_string_standalone(v))
}
