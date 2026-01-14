// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#![allow(clippy::missing_safety_doc)]

use crate::crypto_scrypt::params::Params;
use crate::crypto_scrypt::scrypt;
use tw_memory::ffi::c_result::ErrorCode;
use tw_memory::ffi::{tw_data::TWData, Nonnull, NullableMut, RawPtrTrait};

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
/// \param password data.
/// \param salt data.
/// \param n scrypt parameter `N`: CPU/memory cost.
/// \param r scrypt parameter `r`: block size.
/// \param p scrypt parameter `p`: parallelism.
/// \param desired_len scrypt parameter `Key length`.
/// \return C-compatible byte array.
#[no_mangle]
pub unsafe extern "C" fn crypto_scrypt(
    password: Nonnull<TWData>,
    salt: Nonnull<TWData>,
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
