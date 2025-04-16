// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#![allow(clippy::missing_safety_doc)]

use crate::crypto_hmac::hmac_sha256;
use tw_macros::tw_ffi;
use tw_memory::ffi::c_byte_array::CByteArray;
use tw_memory::ffi::c_byte_array_ref::CByteArrayRef;

/// The PBKDF2 key derivation function.
///
/// \param password *nullable* byte array.
/// \param password_len the length of the `password` array.
/// \param salt *nullable* byte array.
/// \param salt_len the length of the `salt` array.
/// \param iterations PBKDF2 parameter `iterations`.
/// \param desired_len PBKDF2 parameter `desired_len`.
/// \return C-compatible byte array.
#[tw_ffi(ty = static_function, class = TWCrypto, name = PBKDF2)]
#[no_mangle]
pub unsafe extern "C" fn crypto_pbkdf2(
    password: *const u8,
    password_len: usize,
    salt: *const u8,
    salt_len: usize,
    iterations: u32,
    desired_len: u32,
) -> CByteArray {
    let password_ref = CByteArrayRef::new(password, password_len);
    let password = password_ref.as_slice().unwrap_or_default();

    let salt_ref = CByteArrayRef::new(salt, salt_len);
    let salt = salt_ref.as_slice().unwrap_or_default();

    let output = hmac_sha256(password, salt, iterations, desired_len);

    CByteArray::from(output)
}
