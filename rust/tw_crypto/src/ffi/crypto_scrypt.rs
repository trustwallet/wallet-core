// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#![allow(clippy::missing_safety_doc)]

use crate::crypto_scrypt::params::Params;
use crate::crypto_scrypt::scrypt;
use tw_macros::tw_ffi;
use tw_memory::ffi::c_result::ErrorCode;
use tw_memory::ffi::{tw_data::TWData, Nullable, NullableMut, RawPtrTrait};

#[repr(C)]
pub enum ScryptError {
    Ok = 0,
    InvalidParams = 1,
}

impl From<ScryptError> for ErrorCode {
    fn from(error: ScryptError) -> Self {
        error as ErrorCode
    }
}

/// The scrypt key derivation function.
///
/// \param password *nullable* byte array.
/// \param password_len the length of the `password` array.
/// \param salt *nullable* byte array.
/// \param salt_len the length of the `salt` array.
/// \param n scrypt parameter `N`: CPU/memory cost.
/// \param r scrypt parameter `r`: block size.
/// \param p scrypt parameter `p`: parallelism.
/// \param desired_len scrypt parameter `Key length`.
/// \return C-compatible byte array.
#[tw_ffi(ty = static_function, class = TWCrypto, name = Scrypt)]
#[no_mangle]
pub unsafe extern "C" fn crypto_scrypt(
    password: Nullable<TWData>,
    salt: Nullable<TWData>,
    n: u32,
    r: u32,
    p: u32,
    desired_len: usize,
) -> NullableMut<TWData> {
    let password = TWData::from_ptr_as_ref(password)
        .map(|data| data.as_slice())
        .unwrap_or_default();
    let salt = TWData::from_ptr_as_ref(salt)
        .map(|data| data.as_slice())
        .unwrap_or_default();

    let params = Params {
        n,
        r,
        p,
        desired_len,
    };
    scrypt(password, salt, &params)
        .map(|output| TWData::from(output).into_ptr())
        .unwrap_or_else(|_| std::ptr::null_mut())
}
