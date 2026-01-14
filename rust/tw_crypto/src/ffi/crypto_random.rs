// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#![allow(clippy::missing_safety_doc)]

use rand_core::RngCore;
use tw_memory::ffi::c_byte_array_mut::CByteArrayMut;
use tw_memory::ffi::c_result::ErrorCode;
use tw_misc::try_or_else;

#[repr(C)]
pub enum CRandomCode {
    Ok = 0,
    NullBuffer = 1,
    NotAvailable = 2,
}

impl From<CRandomCode> for ErrorCode {
    fn from(error: CRandomCode) -> Self {
        error as ErrorCode
    }
}

/// Fills the provided buffer with cryptographically secure random bytes.
///
/// \param data *non-null* byte array.
/// \param size the length of the `data` array.
/// \return `CRandomCode::Ok` on success, or an error code otherwise.
#[no_mangle]
pub unsafe extern "C" fn crypto_random_buffer(data: *mut u8, size: usize) -> ErrorCode {
    if data.is_null() || size == 0 {
        return ErrorCode::from(CRandomCode::NullBuffer);
    }

    let mut bytes_mut = CByteArrayMut::new(data, size);
    try_or_else!(
        rand::thread_rng().try_fill_bytes(bytes_mut.as_mut()),
        || ErrorCode::from(CRandomCode::NotAvailable)
    );
    ErrorCode::from(CRandomCode::Ok)
}
