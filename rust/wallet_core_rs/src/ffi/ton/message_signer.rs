// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#![allow(clippy::missing_safety_doc)]

use tw_encoding::hex::ToHex;
use tw_keypair::ed25519;
use tw_keypair::ffi::privkey::TWPrivateKey;
use tw_memory::ffi::tw_string::TWString;
use tw_memory::ffi::RawPtrTrait;
use tw_misc::try_or_else;
use tw_ton::modules::personal_message_signer::PersonalMessageSigner;

/// Signs an arbitrary message to prove ownership of an address for off-chain services.
/// https://github.com/ton-foundation/specs/blob/main/specs/wtf-0002.md
///
/// \param private_key: the private key used for signing
/// \param message: A custom message which is input to the signing.
/// \returns the signature, Hex-encoded. On invalid input null is returned. Returned object needs to be deleted after use.
#[no_mangle]
pub unsafe extern "C" fn tw_ton_message_signer_sign_message(
    private_key: *const TWPrivateKey,
    message: *const TWString,
) -> *mut TWString {
    let private_key = try_or_else!(
        TWPrivateKey::from_ptr_as_ref(private_key),
        std::ptr::null_mut
    );
    let private_key_bytes = private_key.as_ref().key();
    let ed25519_private_key = try_or_else!(
        ed25519::sha512::PrivateKey::try_from(private_key_bytes.as_slice()),
        std::ptr::null_mut
    );

    let message = try_or_else!(TWString::from_ptr_as_ref(message), std::ptr::null_mut);
    let message_str = try_or_else!(message.as_str(), std::ptr::null_mut);

    let signature = try_or_else!(
        PersonalMessageSigner::sign(&ed25519_private_key, message_str),
        std::ptr::null_mut
    );
    TWString::from(signature.to_bytes().to_hex()).into_ptr()
}
