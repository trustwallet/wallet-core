// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#![allow(clippy::missing_safety_doc)]

use crate::modules::webauthn::{get_formatted_webauthn_signature, get_webauthn_message_hash};
use tw_macros::tw_ffi;
use tw_memory::ffi::tw_data::TWData;
use tw_memory::ffi::tw_string::TWString;
use tw_memory::ffi::{Nonnull, NullableMut, RawPtrTrait};
use tw_misc::try_or_else;

/// Computes WebAuthn message hash to be signed with secp256p1 private key.
///
/// \param authenticator_data The authenticator data in hex format.
/// \param client_data_json The client data JSON string with a challenge.
/// \return WebAuthn message hash.
#[tw_ffi(ty = static_function, class = TWWebAuthnSolidity, name = GetMessageHash)]
#[no_mangle]
pub unsafe extern "C" fn tw_webauthn_solidity_get_message_hash(
    authenticator_data: Nonnull<TWString>,
    client_data_json: Nonnull<TWString>,
) -> NullableMut<TWData> {
    let authenticator_data = try_or_else!(
        TWString::from_ptr_as_ref(authenticator_data),
        std::ptr::null_mut
    );
    let authenticator_data = try_or_else!(authenticator_data.as_str(), std::ptr::null_mut);

    let client_data_json = try_or_else!(
        TWString::from_ptr_as_ref(client_data_json),
        std::ptr::null_mut
    );
    let client_data_json = try_or_else!(client_data_json.as_str(), std::ptr::null_mut);

    let message_hash = try_or_else!(
        get_webauthn_message_hash(authenticator_data, client_data_json),
        std::ptr::null_mut
    );
    TWData::from(message_hash).into_ptr()
}

/// Converts the original ASN-encoded signature from webauthn to the format accepted by Barz
///
/// \param authenticator_data The authenticator data in hex format.
/// \param client_data_json The client data JSON string with a challenge.
/// \param der_signature original ASN-encoded signature from webauthn.
/// \return WebAuthn ABI-encoded data.
#[tw_ffi(ty = static_function, class = TWWebAuthnSolidity, name = GetFormattedSignature)]
#[no_mangle]
pub unsafe extern "C" fn tw_webauthn_solidity_get_formatted_signature(
    authenticator_data: Nonnull<TWString>,
    client_data_json: Nonnull<TWString>,
    der_signature: Nonnull<TWData>,
) -> NullableMut<TWData> {
    let authenticator_data = try_or_else!(
        TWString::from_ptr_as_ref(authenticator_data),
        std::ptr::null_mut
    );
    let authenticator_data = try_or_else!(authenticator_data.as_str(), std::ptr::null_mut);

    let client_data_json = try_or_else!(
        TWString::from_ptr_as_ref(client_data_json),
        std::ptr::null_mut
    );
    let client_data_json = try_or_else!(client_data_json.as_str(), std::ptr::null_mut);
    let der_signature = try_or_else!(TWData::from_ptr_as_ref(der_signature), std::ptr::null_mut);
    let der_signature = der_signature.as_slice();

    let encoded = try_or_else!(
        get_formatted_webauthn_signature(authenticator_data, client_data_json, der_signature),
        std::ptr::null_mut
    );
    TWData::from(encoded).into_ptr()
}
