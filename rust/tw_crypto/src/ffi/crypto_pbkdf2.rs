// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#![allow(clippy::missing_safety_doc)]

use crate::crypto_pbkdf2::{pbkdf2_hmac_512, pbkdf2_hmac_sha256};
use tw_macros::tw_ffi;
use tw_memory::ffi::{tw_data::TWData, Nonnull, NullableMut, RawPtrTrait};

/// The PBKDF2 key derivation function.
///
/// \param password data.
/// \param salt data.
/// \param iterations PBKDF2 parameter `iterations`.
/// \param desired_len PBKDF2 parameter `desired_len`.
/// \return *nullable* data.
#[tw_ffi(ty = static_function, class = TWPBKDF2, name = HmacSha256)]
#[no_mangle]
pub unsafe extern "C" fn tw_pbkdf2_hmac_sha256(
    password: Nonnull<TWData>,
    salt: Nonnull<TWData>,
    iterations: u32,
    desired_len: usize,
) -> NullableMut<TWData> {
    let password = TWData::from_ptr_as_ref(password)
        .map(|data| data.as_slice())
        .unwrap_or_default();
    let salt = TWData::from_ptr_as_ref(salt)
        .map(|data| data.as_slice())
        .unwrap_or_default();

    let output = pbkdf2_hmac_sha256(password, salt, iterations, desired_len);
    TWData::from(output).into_ptr()
}

/// The PBKDF2 key derivation function.
///
/// \param password data.
/// \param salt data.
/// \param iterations PBKDF2 parameter `iterations`.
/// \param desired_len PBKDF2 parameter `desired_len`.
/// \return *nullable* data.
#[tw_ffi(ty = static_function, class = TWPBKDF2, name = HmacSha512)]
#[no_mangle]
pub unsafe extern "C" fn tw_pbkdf2_hmac_sha512(
    password: Nonnull<TWData>,
    salt: Nonnull<TWData>,
    iterations: u32,
    desired_len: usize,
) -> NullableMut<TWData> {
    let password = TWData::from_ptr_as_ref(password)
        .map(|data| data.as_slice())
        .unwrap_or_default();
    let salt = TWData::from_ptr_as_ref(salt)
        .map(|data| data.as_slice())
        .unwrap_or_default();

    let output = pbkdf2_hmac_512(password, salt, iterations, desired_len);
    TWData::from(output).into_ptr()
}
