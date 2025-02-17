// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#![allow(clippy::missing_safety_doc)]

use tw_keypair::ffi::{
    privkey::{tw_private_key_create_with_data, TWPrivateKey},
    pubkey::{tw_public_key_create_with_data, TWPublicKey},
};
use tw_macros::tw_ffi;
use tw_memory::ffi::{
    tw_data::TWData, tw_string::TWString, Nonnull, Nullable, NullableMut, RawPtrTrait,
};
use tw_misc::try_or_else;

/// Sum two unsigned integers of 8 bits
#[tw_ffi(ty = static_function, class = TWFFITest, name = UnsignedSumU8)]
#[no_mangle]
pub unsafe extern "C" fn tw_ffi_test_unsigned_sum_u8(a: u8, b: u8) -> u8 {
    a.checked_add(b).unwrap_or(u8::MAX)
}

/// Sum two unsigned integers of 16 bits
#[tw_ffi(ty = static_function, class = TWFFITest, name = UnsignedSumU16)]
#[no_mangle]
pub unsafe extern "C" fn tw_ffi_test_unsigned_sum_u16(a: u16, b: u16) -> u16 {
    a.checked_add(b).unwrap_or(u16::MAX)
}

/// Sum two unsigned integers of 32 bits
#[tw_ffi(ty = static_function, class = TWFFITest, name = UnsignedSumU32)]
#[no_mangle]
pub unsafe extern "C" fn tw_ffi_test_unsigned_sum_u32(a: u32, b: u32) -> u32 {
    a.checked_add(b).unwrap_or(u32::MAX)
}

/// Sum two unsigned integers of 64 bits
#[tw_ffi(ty = static_function, class = TWFFITest, name = UnsignedSumU64)]
#[no_mangle]
pub unsafe extern "C" fn tw_ffi_test_unsigned_sum_u64(a: u64, b: u64) -> u64 {
    a.checked_add(b).unwrap_or(u64::MAX)
}

/// Sum two signed integers of 8 bits
#[tw_ffi(ty = static_function, class = TWFFITest, name = SignedSumI8)]
#[no_mangle]
pub unsafe extern "C" fn tw_ffi_test_signed_sum_i8(a: i8, b: i8) -> i8 {
    a.checked_add(b).unwrap_or(i8::MAX)
}

/// Sum two signed integers of 16 bits
#[tw_ffi(ty = static_function, class = TWFFITest, name = SignedSumI16)]
#[no_mangle]
pub unsafe extern "C" fn tw_ffi_test_signed_sum_i16(a: i16, b: i16) -> i16 {
    a.checked_add(b).unwrap_or(i16::MAX)
}

/// Sum two signed integers of 32 bits
#[tw_ffi(ty = static_function, class = TWFFITest, name = SignedSumI32)]
#[no_mangle]
pub unsafe extern "C" fn tw_ffi_test_signed_sum_i32(a: i32, b: i32) -> i32 {
    a.checked_add(b).unwrap_or(i32::MAX)
}

/// Sum two signed integers of 64 bits
#[tw_ffi(ty = static_function, class = TWFFITest, name = SignedSumI64)]
#[no_mangle]
pub unsafe extern "C" fn tw_ffi_test_signed_sum_i64(a: i64, b: i64) -> i64 {
    a.checked_add(b).unwrap_or(i64::MAX)
}

/// Concatenate a string with a character
#[tw_ffi(ty = static_function, class = TWFFITest, name = StringWithU8)]
#[no_mangle]
pub unsafe extern "C" fn tw_ffi_test_string_with_u8(
    a: Nullable<TWString>,
    b: u8,
) -> NullableMut<TWString> {
    let a = try_or_else!(TWString::from_ptr_as_ref(a), std::ptr::null_mut);
    let a_str = try_or_else!(a.as_str(), std::ptr::null_mut);
    let mut result = String::new();
    result.push_str(a_str);
    result.push(b as char);
    TWString::from(result).into_ptr()
}

#[tw_ffi(ty = static_function, class = TWFFITest, name = DataWithU8)]
#[no_mangle]
pub unsafe extern "C" fn tw_ffi_test_data_with_u8(
    a: Nonnull<TWData>,
    b: u8,
) -> NullableMut<TWData> {
    let a = try_or_else!(TWData::from_ptr_as_ref(a), std::ptr::null_mut);
    let mut data = Vec::new();
    data.extend_from_slice(a.clone().into_vec().as_slice());
    data.push(b);
    TWData::from(data).into_ptr()
}

#[tw_ffi(ty = static_function, class = TWFFITest, name = NullableDataWithU8)]
#[no_mangle]
pub unsafe extern "C" fn tw_ffi_test_nullable_data_with_u8(
    a: Nullable<TWData>,
    b: u8,
) -> NullableMut<TWData> {
    let a = try_or_else!(TWData::from_ptr_as_ref(a), std::ptr::null_mut);
    let mut data = Vec::new();
    data.extend_from_slice(a.clone().into_vec().as_slice());
    data.push(b);
    TWData::from(data).into_ptr()
}

#[tw_ffi(ty = static_function, class = TWFFITest, name = PrivateKey)]
#[no_mangle]
pub unsafe extern "C" fn tw_ffi_test_private_key(
    a: Nonnull<TWPrivateKey>,
) -> NullableMut<TWPrivateKey> {
    let c = try_or_else!(TWPrivateKey::from_ptr_as_ref(a), std::ptr::null_mut);
    tw_private_key_create_with_data(c.as_ref().bytes().as_ptr(), c.as_ref().bytes().len())
}

#[tw_ffi(ty = static_function, class = TWFFITest, name = NullablePrivateKey)]
#[no_mangle]
pub unsafe extern "C" fn tw_ffi_test_nullable_private_key(
    a: Nullable<TWPrivateKey>,
) -> NullableMut<TWPrivateKey> {
    let c = try_or_else!(TWPrivateKey::from_ptr_as_ref(a), std::ptr::null_mut);
    tw_private_key_create_with_data(c.as_ref().bytes().as_ptr(), c.as_ref().bytes().len())
}

#[tw_ffi(ty = static_function, class = TWFFITest, name = PublicKey)]
#[no_mangle]
pub unsafe extern "C" fn tw_ffi_test_public_key(
    a: Nonnull<TWPublicKey>,
) -> NullableMut<TWPublicKey> {
    let c = try_or_else!(TWPublicKey::from_ptr_as_ref(a), std::ptr::null_mut);
    tw_public_key_create_with_data(
        c.as_ref().to_bytes().as_ptr(),
        c.as_ref().to_bytes().len(),
        c.as_ref().public_key_type() as u32,
    )
}

#[tw_ffi(ty = static_function, class = TWFFITest, name = NullablePublicKey)]
#[no_mangle]
pub unsafe extern "C" fn tw_ffi_test_nullable_public_key(
    a: Nullable<TWPublicKey>,
) -> NullableMut<TWPublicKey> {
    let c = try_or_else!(TWPublicKey::from_ptr_as_ref(a), std::ptr::null_mut);
    tw_public_key_create_with_data(
        c.as_ref().to_bytes().as_ptr(),
        c.as_ref().to_bytes().len(),
        c.as_ref().public_key_type() as u32,
    )
}
