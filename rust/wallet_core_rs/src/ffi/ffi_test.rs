// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#![allow(clippy::missing_safety_doc)]

use tw_macros::tw_ffi;
use tw_memory::ffi::{tw_string::TWString, Nonnull, NullableMut, RawPtrTrait};
use tw_misc::try_or_else;

/// Sum two unsigned integers of 8 bits
#[tw_ffi(ty = static_function, class = TWFFITest, name = UnsignedSumU8)]
#[no_mangle]
pub unsafe extern "C" fn tw_ffi_test_unsigned_sum_u8(a: u8, b: u8) -> u8 {
    a + b
}

/// Sum two unsigned integers of 16 bits
#[tw_ffi(ty = static_function, class = TWFFITest, name = UnsignedSumU16)]
#[no_mangle]
pub unsafe extern "C" fn tw_ffi_test_unsigned_sum_u16(a: u16, b: u16) -> u16 {
    a + b
}

/// Sum two unsigned integers of 32 bits
#[tw_ffi(ty = static_function, class = TWFFITest, name = UnsignedSumU32)]
#[no_mangle]
pub unsafe extern "C" fn tw_ffi_test_unsigned_sum_u32(a: u32, b: u32) -> u32 {
    a + b
}

/// Sum two unsigned integers of 64 bits
#[tw_ffi(ty = static_function, class = TWFFITest, name = UnsignedSumU64)]
#[no_mangle]
pub unsafe extern "C" fn tw_ffi_test_unsigned_sum_u64(a: u64, b: u64) -> u64 {
    a + b
}

/// Sum two signed integers of 8 bits
#[tw_ffi(ty = static_function, class = TWFFITest, name = SignedSumI8)]
#[no_mangle]
pub unsafe extern "C" fn tw_ffi_test_signed_sum_i8(a: i8, b: i8) -> i8 {
    a + b
}

/// Sum two signed integers of 16 bits
#[tw_ffi(ty = static_function, class = TWFFITest, name = SignedSumI16)]
#[no_mangle]
pub unsafe extern "C" fn tw_ffi_test_signed_sum_i16(a: i16, b: i16) -> i16 {
    a + b
}

/// Sum two signed integers of 32 bits
#[tw_ffi(ty = static_function, class = TWFFITest, name = SignedSumI32)]
#[no_mangle]
pub unsafe extern "C" fn tw_ffi_test_signed_sum_i32(a: i32, b: i32) -> i32 {
    a + b
}

/// Sum two signed integers of 64 bits
#[tw_ffi(ty = static_function, class = TWFFITest, name = SignedSumI64)]
#[no_mangle]
pub unsafe extern "C" fn tw_ffi_test_signed_sum_i64(a: i64, b: i64) -> i64 {
    a + b
}

//
#[tw_ffi(ty = static_function, class = TWFFITest, name = StringWithU8)]
#[no_mangle]
pub unsafe extern "C" fn tw_ffi_test_string_with_u8(
    a: Nonnull<TWString>,
    b: u8,
) -> NullableMut<TWString> {
    let a = try_or_else!(TWString::from_ptr_as_ref(a), std::ptr::null_mut);
    let a_str = try_or_else!(a.as_str(), std::ptr::null_mut);
    let mut result = String::new();
    result.push_str(a_str);
    result.push(b as char);
    TWString::from(result).into_ptr()
}
