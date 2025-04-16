// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#![allow(clippy::missing_safety_doc)]

use pbkdf2::pbkdf2_hmac;
use sha2::Sha256;
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

    let mut output = vec![0u8; desired_len as usize];
    pbkdf2_hmac::<Sha256>(password, salt, iterations, &mut output);

    CByteArray::from(output)
}
