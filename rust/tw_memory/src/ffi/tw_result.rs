// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#![allow(clippy::missing_safety_doc)]

use crate::ffi::tw_string::TWString;
use crate::ffi::{Nonnull, NonnullMut, NullableMut, RawPtrTrait};
use tw_macros::tw_ffi;

/// Represents either a success (no value) or an error with a string description.
pub struct TWResultVoid(Result<(), String>);

impl TWResultVoid {
    pub fn ok() -> Self {
        TWResultVoid(Ok(()))
    }

    pub fn err(msg: impl Into<String>) -> Self {
        TWResultVoid(Err(msg.into()))
    }

    fn is_success(&self) -> bool {
        self.0.is_ok()
    }

    fn error_message(&self) -> Option<&str> {
        self.0.as_ref().err().map(String::as_str)
    }
}

impl RawPtrTrait for TWResultVoid {}

/// Creates a `TWResultVoid` representing success.
/// Must be deleted with `tw_result_void_delete`.
///
/// \return Non-null pointer to a new `TWResultVoid`.
#[tw_ffi(ty = constructor, class = TWResultVoid, name = CreateOk)]
#[no_mangle]
pub unsafe extern "C" fn tw_result_void_create_ok() -> NonnullMut<TWResultVoid> {
    TWResultVoid::ok().into_ptr()
}

/// Creates a `TWResultVoid` representing an error with the given message.
/// Must be deleted with `tw_result_void_delete`.
///
/// \param error A non-null `TWString` with the error description.
/// \return Non-null pointer to a new `TWResultVoid`.
#[tw_ffi(ty = constructor, class = TWResultVoid, name = CreateError)]
#[no_mangle]
pub unsafe extern "C" fn tw_result_void_create_error(
    error: Nonnull<TWString>,
) -> NonnullMut<TWResultVoid> {
    let msg = TWString::from_ptr_as_ref(error)
        .and_then(TWString::as_str)
        .unwrap_or_default();
    TWResultVoid::err(msg).into_ptr()
}

/// Returns true if the result represents success.
///
/// \param result A non-null `TWResultVoid` pointer.
/// \return true on success, false on error.
#[tw_ffi(ty = property, class = TWResultVoid, name = IsSuccess)]
#[no_mangle]
pub unsafe extern "C" fn tw_result_void_is_success(result_ptr: Nonnull<TWResultVoid>) -> bool {
    TWResultVoid::from_ptr_as_ref(result_ptr)
        .map(TWResultVoid::is_success)
        .unwrap_or(false)
}

/// Returns true if the result represents an error.
///
/// \param result A non-null `TWResultVoid` pointer.
/// \return true on error, false on success.
#[tw_ffi(ty = property, class = TWResultVoid, name = IsErr)]
#[no_mangle]
pub unsafe extern "C" fn tw_result_void_is_err(result_ptr: Nonnull<TWResultVoid>) -> bool {
    TWResultVoid::from_ptr_as_ref(result_ptr)
        .map(|r| !r.is_success())
        .unwrap_or(false)
}

/// Deletes a `TWResultVoid` created with a `TWResultVoidCreate*` function and frees the memory.
///
/// \param result A non-null `TWResultVoid` pointer.
#[tw_ffi(ty = destructor, class = TWResultVoid, name = Delete)]
#[no_mangle]
pub unsafe extern "C" fn tw_result_void_delete(result_ptr: NonnullMut<TWResultVoid>) {
    let _ = TWResultVoid::from_ptr(result_ptr);
}

/// Returns the error description, or null if the result is a success.
/// The returned string must be deleted with `tw_string_delete`.
///
/// \param result A non-null `TWResultVoid` pointer.
/// \return Nullable pointer to a `TWString` with the error message.
#[tw_ffi(ty = property, class = TWResultVoid, name = GetErr)]
#[no_mangle]
pub unsafe extern "C" fn tw_result_void_get_err(
    result_ptr: Nonnull<TWResultVoid>,
) -> NullableMut<TWString> {
    let result_ref = match TWResultVoid::from_ptr_as_ref(result_ptr) {
        Some(r) => r,
        None => return std::ptr::null_mut(),
    };
    match result_ref.error_message() {
        Some(msg) => TWString::from(msg.to_owned()).into_ptr(),
        None => std::ptr::null_mut(),
    }
}
