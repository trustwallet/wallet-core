// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#![allow(clippy::missing_safety_doc)]

use crate::modules::biz_passkey_session::{
    encode_execute_with_passkey_session_call, encode_passkey_nonce,
    encode_register_passkey_session_call, encode_remove_passkey_session_call,
    sign_execute_with_signature_call,
};
use tw_keypair::ffi::pubkey::TWPublicKey;
use tw_macros::tw_ffi;
use tw_memory::ffi::tw_data::TWData;
use tw_memory::ffi::{Nonnull, NullableMut, RawPtrTrait};
use tw_misc::try_or_else;
use tw_proto::deserialize;
use tw_proto::BizPasskeySession::Proto::{
    ExecuteWithPasskeySessionInput, ExecuteWithSignatureInput,
};

/// Encodes `BizPasskeySession.registerSession` function call to register a session passkey public key.
///
/// \param session_passkey_public_key The nist256p1 (aka secp256p1) public key of the session passkey.
/// \param valid_until_timestamp The timestamp until which the session is valid. Big endian uint64.
/// \return ABI-encoded function call.
#[tw_ffi(ty = static_function, class = TWBizPasskeySession, name = EncodeRegisterSessionCall)]
#[no_mangle]
pub unsafe extern "C" fn tw_biz_passkey_session_encode_register_session_call(
    session_passkey_public_key: Nonnull<TWPublicKey>,
    valid_until_timestamp: Nonnull<TWData>,
) -> NullableMut<TWData> {
    let session_passkey_public_key = try_or_else!(
        TWPublicKey::from_ptr_as_ref(session_passkey_public_key),
        std::ptr::null_mut
    );
    let valid_until_timestamp = try_or_else!(
        TWData::from_ptr_as_ref(valid_until_timestamp),
        std::ptr::null_mut
    );
    let encoded = try_or_else!(
        encode_register_passkey_session_call(
            session_passkey_public_key.as_ref(),
            valid_until_timestamp.as_slice()
        ),
        std::ptr::null_mut
    );
    TWData::from(encoded).into_ptr()
}

/// Encodes `BizPasskeySession.removeSession` function call to deregister a session passkey public key.
///
/// \param session_passkey_public_key The nist256p1 (aka secp256p1) public key of the session passkey.
/// \return ABI-encoded function call.
#[tw_ffi(ty = static_function, class = TWBizPasskeySession, name = EncodeRemoveSessionCall)]
#[no_mangle]
pub unsafe extern "C" fn tw_biz_passkey_session_encode_remove_session_call(
    session_passkey_public_key: Nonnull<TWPublicKey>,
) -> NullableMut<TWData> {
    let session_passkey_public_key = try_or_else!(
        TWPublicKey::from_ptr_as_ref(session_passkey_public_key),
        std::ptr::null_mut
    );
    let encoded = try_or_else!(
        encode_remove_passkey_session_call(session_passkey_public_key.as_ref(),),
        std::ptr::null_mut
    );
    TWData::from(encoded).into_ptr()
}

/// Encodes `BizPasskeySession` nonce.
///
/// \param nonce The nonce of the Biz Passkey Session account.
/// \return uint256 represented as [passkey_nonce_key_192, nonce_64].
#[tw_ffi(ty = static_function, class = TWBizPasskeySession, name = EncodePasskeySessionNonce)]
#[no_mangle]
pub unsafe extern "C" fn tw_biz_passkey_session_encode_passkey_session_nonce(
    nonce: Nonnull<TWData>,
) -> NullableMut<TWData> {
    let nonce = try_or_else!(TWData::from_ptr_as_ref(nonce), std::ptr::null_mut);
    let encoded = try_or_else!(encode_passkey_nonce(nonce.as_slice()), std::ptr::null_mut);
    TWData::from(encoded).into_ptr()
}

/// Encodes `BizPasskeySession.executeWithPasskeySession` function call to execute a batch of transactions.
///
/// \param input The serialized data of `BizPasskeySession.ExecuteWithPasskeySessionInput` protobuf message.
/// \return ABI-encoded function call.
#[tw_ffi(ty = static_function, class = TWBizPasskeySession, name = EncodeExecuteWithPasskeySessionCall)]
#[no_mangle]
pub unsafe extern "C" fn tw_biz_passkey_session_encode_execute_with_passkey_session_call(
    input: Nonnull<TWData>,
) -> NullableMut<TWData> {
    let input = try_or_else!(TWData::from_ptr_as_ref(input), std::ptr::null_mut);
    let input: ExecuteWithPasskeySessionInput =
        try_or_else!(deserialize(input.as_slice()), std::ptr::null_mut);
    let encoded = try_or_else!(
        encode_execute_with_passkey_session_call(&input),
        std::ptr::null_mut
    );
    TWData::from(encoded).into_ptr()
}

/// Signs and encodes `BizPasskeySession.executeWithPasskeySession` function call to execute a batch of transactions.
///
/// \param input The serialized data of `BizPasskeySession.ExecuteWithSignatureInput` protobuf message.
/// \return ABI-encoded function call.
#[tw_ffi(ty = static_function, class = TWBizPasskeySession, name = SignExecuteWithSignatureCall)]
#[no_mangle]
pub unsafe extern "C" fn tw_biz_passkey_session_sign_execute_with_signature_call(
    input: Nonnull<TWData>,
) -> NullableMut<TWData> {
    let input = try_or_else!(TWData::from_ptr_as_ref(input), std::ptr::null_mut);
    let input: ExecuteWithSignatureInput =
        try_or_else!(deserialize(input.as_slice()), std::ptr::null_mut);
    let encoded = try_or_else!(sign_execute_with_signature_call(&input), std::ptr::null_mut);
    TWData::from(encoded).into_ptr()
}
