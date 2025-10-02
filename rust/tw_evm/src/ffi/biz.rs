// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#![allow(clippy::missing_safety_doc)]

use crate::modules::biz::{
    encode_execute_with_passkey_session_call, encode_passkey_nonce,
    encode_register_passkey_session_call, encode_remove_passkey_session_call, get_encoded_hash,
    sign_user_op_hash,
};
use tw_keypair::ffi::pubkey::TWPublicKey;
use tw_macros::tw_ffi;
use tw_memory::ffi::tw_data::TWData;
use tw_memory::ffi::tw_string::TWString;
use tw_memory::ffi::{Nonnull, NullableMut, RawPtrTrait};
use tw_misc::try_or_else;
use tw_proto::deserialize;
use tw_proto::Biz::Proto::ExecuteWithPasskeySessionInput;

/// Returns the encoded hash of the user operation
///
/// \param chain_id The chain ID of the user.
/// \param code_address The address of the smart contract wallet.
/// \param code_name The name of the smart contract wallet.
/// \param code_version The version of the smart contract wallet.
/// \param type_hash The type hash of the smart contract wallet.
/// \param domain_separator_hash The domain separator hash of the smart contract wallet.
/// \param sender The sender of the smart contract wallet.
/// \param user_op_hash The user operation hash of the smart contract wallet.
/// \return The encoded hash.
#[tw_ffi(ty = static_function, class = TWBiz, name = GetEncodedHash)]
#[no_mangle]
pub unsafe extern "C" fn tw_biz_get_encoded_hash(
    chain_id: Nonnull<TWData>,
    code_address: Nonnull<TWString>,
    code_name: Nonnull<TWString>,
    code_version: Nonnull<TWString>,
    type_hash: Nonnull<TWString>,
    domain_separator_hash: Nonnull<TWString>,
    sender: Nonnull<TWString>,
    user_op_hash: Nonnull<TWString>,
) -> NullableMut<TWData> {
    let chain_id = try_or_else!(TWData::from_ptr_as_ref(chain_id), std::ptr::null_mut);
    let code_address = try_or_else!(TWString::from_ptr_as_ref(code_address), std::ptr::null_mut);
    let code_address = try_or_else!(code_address.as_str(), std::ptr::null_mut);
    let code_name = try_or_else!(TWString::from_ptr_as_ref(code_name), std::ptr::null_mut);
    let code_name = try_or_else!(code_name.as_str(), std::ptr::null_mut);
    let code_version = try_or_else!(TWString::from_ptr_as_ref(code_version), std::ptr::null_mut);
    let code_version = try_or_else!(code_version.as_str(), std::ptr::null_mut);
    let type_hash = try_or_else!(TWString::from_ptr_as_ref(type_hash), std::ptr::null_mut);
    let type_hash = try_or_else!(type_hash.as_str(), std::ptr::null_mut);
    let domain_separator_hash = try_or_else!(
        TWString::from_ptr_as_ref(domain_separator_hash),
        std::ptr::null_mut
    );
    let domain_separator_hash = try_or_else!(domain_separator_hash.as_str(), std::ptr::null_mut);
    let sender = try_or_else!(TWString::from_ptr_as_ref(sender), std::ptr::null_mut);
    let sender = try_or_else!(sender.as_str(), std::ptr::null_mut);
    let user_op_hash = try_or_else!(TWString::from_ptr_as_ref(user_op_hash), std::ptr::null_mut);
    let user_op_hash = try_or_else!(user_op_hash.as_str(), std::ptr::null_mut);
    let encoded_hash = try_or_else!(
        get_encoded_hash(
            chain_id.as_slice(),
            code_address,
            code_name,
            code_version,
            type_hash,
            domain_separator_hash,
            sender,
            user_op_hash
        ),
        std::ptr::null_mut
    );
    TWData::from(encoded_hash).into_ptr()
}

/// Signs a message using the private key
///
/// \param hash The hash of the user.
/// \param private_key The private key of the user.
/// \return The signed hash.
#[tw_ffi(ty = static_function, class = TWBiz, name = GetSignedHash)]
#[no_mangle]
pub unsafe extern "C" fn tw_biz_get_signed_hash(
    hash: Nonnull<TWString>,
    private_key: Nonnull<TWString>,
) -> NullableMut<TWData> {
    let hash = try_or_else!(TWString::from_ptr_as_ref(hash), std::ptr::null_mut);
    let hash = try_or_else!(hash.as_str(), std::ptr::null_mut);
    let private_key = try_or_else!(TWString::from_ptr_as_ref(private_key), std::ptr::null_mut);
    let private_key = try_or_else!(private_key.as_str(), std::ptr::null_mut);
    let signed_hash = try_or_else!(sign_user_op_hash(hash, private_key), std::ptr::null_mut);
    TWData::from(signed_hash).into_ptr()
}

/// Encodes `Biz.registerSession` function call to register a session passkey public key.
///
/// \param session_passkey_public_key The nist256p1 (aka secp256p1) public key of the session passkey.
/// \param valid_until_timestamp The timestamp until which the session is valid. Big endian uint64.
/// \return ABI-encoded function call.
#[tw_ffi(ty = static_function, class = TWBiz, name = EncodeRegisterSessionCall)]
#[no_mangle]
pub unsafe extern "C" fn tw_biz_encode_register_session_call(
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

/// Encodes `Biz.removeSession` function call to deregister a session passkey public key.
///
/// \param session_passkey_public_key The nist256p1 (aka secp256p1) public key of the session passkey.
/// \return ABI-encoded function call.
#[tw_ffi(ty = static_function, class = TWBiz, name = EncodeRemoveSessionCall)]
#[no_mangle]
pub unsafe extern "C" fn tw_biz_encode_remove_session_call(
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

/// Encodes Biz Passkey Session nonce.
///
/// \param nonce The nonce of the Biz Passkey Session account.
/// \return uint256 represented as [passkey_nonce_key_192, nonce_64].
#[tw_ffi(ty = static_function, class = TWBiz, name = EncodePasskeySessionNonce)]
#[no_mangle]
pub unsafe extern "C" fn tw_biz_encode_passkey_session_nonce(
    nonce: Nonnull<TWData>,
) -> NullableMut<TWData> {
    let nonce = try_or_else!(TWData::from_ptr_as_ref(nonce), std::ptr::null_mut);
    let encoded = try_or_else!(encode_passkey_nonce(nonce.as_slice()), std::ptr::null_mut);
    TWData::from(encoded).into_ptr()
}

/// Encodes `Biz.executeWithPasskeySession` function call to execute a batch of transactions.
///
/// \param input The serialized data of `Biz.ExecuteWithPasskeySessionInput` protobuf message.
/// \return ABI-encoded function call.
#[tw_ffi(ty = static_function, class = TWBiz, name = EncodeExecuteWithPasskeySessionCall)]
#[no_mangle]
pub unsafe extern "C" fn tw_biz_encode_execute_with_passkey_session_call(
    input: Nonnull<TWData>,
) -> NullableMut<TWData> {
    let input = try_or_else!(TWData::from_ptr_as_ref(input), std::ptr::null_mut);
    let input: ExecuteWithPasskeySessionInput = deserialize(input.as_slice()).unwrap();
    let encoded = try_or_else!(
        encode_execute_with_passkey_session_call(&input),
        std::ptr::null_mut
    );
    TWData::from(encoded).into_ptr()
}
