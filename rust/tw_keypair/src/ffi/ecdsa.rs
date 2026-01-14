// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#![allow(clippy::missing_safety_doc)]

use crate::ecdsa::nist256p1::{
    PrivateKey as NISSecPrivateKey, PublicKey as NISTPublicKey, Signature as NISSignature,
};
use crate::ecdsa::secp256k1::{
    PrivateKey as SECPrivateKey, PublicKey as SECPublicKey, Signature as SECSignature,
};
use tw_hash::hasher::Hasher;
use tw_macros::tw_ffi;
use tw_memory::ffi::{tw_data::TWData, Nonnull, NullableMut, RawPtrTrait};
use tw_misc::try_or_else;

/// Converts an ECDSA signature to a DER-encoded signature.
///
/// \param sig *non-null* pointer to a block of data corresponding to the signature.
/// \param is_secp256k1 whether the signature is a secp256k1 signature.
///
/// \return DER-encoded signature.
#[tw_ffi(ty = static_function, class = TWECDSA, name = SigToDER)]
#[no_mangle]
pub unsafe extern "C" fn tw_ecdsa_sig_to_der(
    sig: Nonnull<TWData>,
    is_secp256k1: bool,
) -> NullableMut<TWData> {
    let sig = TWData::from_ptr_as_ref(sig)
        .map(|data| data.as_slice())
        .unwrap_or_default();
    if is_secp256k1 {
        let sig = try_or_else!(SECSignature::try_from(sig), std::ptr::null_mut);
        sig.to_der()
            .map(|output| TWData::from(output.der_bytes()).into_ptr())
            .unwrap_or_else(|_| std::ptr::null_mut())
    } else {
        let sig = try_or_else!(NISSignature::try_from(sig), std::ptr::null_mut);
        sig.to_der()
            .map(|output| TWData::from(output.der_bytes()).into_ptr())
            .unwrap_or_else(|_| std::ptr::null_mut())
    }
}

/// Computes the hash of a public key.
///
/// \param pubkey *non-null* pointer to a block of data corresponding to the public key.
/// \param is_secp256k1 whether the public key is a secp256k1 public key.
/// \param hasher hasher type.
/// \return hash of the public key.
#[tw_ffi(ty = static_function, class = TWECDSA, name = PubkeyHash)]
#[no_mangle]
pub unsafe extern "C" fn tw_ecdsa_pubkey_hash(
    pubkey_data: Nonnull<TWData>,
    is_secp256k1: bool,
    hasher: i32,
) -> NullableMut<TWData> {
    let pubkey_data = TWData::from_ptr_as_ref(pubkey_data)
        .map(|data| data.as_slice())
        .unwrap_or_default();
    let hasher_u32: u32 = try_or_else!(hasher.try_into(), std::ptr::null_mut);
    let hasher = try_or_else!(Hasher::from_repr(hasher_u32), std::ptr::null_mut);
    let hash = if is_secp256k1 {
        let pubkey = try_or_else!(SECPublicKey::try_from(pubkey_data), std::ptr::null_mut);
        pubkey.hash(hasher)
    } else {
        let pubkey = try_or_else!(NISTPublicKey::try_from(pubkey_data), std::ptr::null_mut);
        pubkey.hash(hasher)
    };
    TWData::from(hash.to_vec()).into_ptr()
}

/// Compares two ECDSA public keys.
///
/// \param pubkey_data1 *non-null* pointer to a block of data corresponding to the first public key.
/// \param pubkey_data2 *non-null* pointer to a block of data corresponding to the second public key.
/// \param is_secp256k1 whether the public keys are secp256k1 public keys.
/// \return -2 if the public keys are invalid, otherwise the comparison result.
#[tw_ffi(ty = static_function, class = TWECDSA, name = PubkeyCompare)]
#[no_mangle]
pub unsafe extern "C" fn tw_ecdsa_pubkey_compare(
    pubkey_data1: Nonnull<TWData>,
    pubkey_data2: Nonnull<TWData>,
    is_secp256k1: bool,
) -> i8 {
    let pubkey_data1 = TWData::from_ptr_as_ref(pubkey_data1)
        .map(|data| data.as_slice())
        .unwrap_or_default();
    let pubkey_data2 = TWData::from_ptr_as_ref(pubkey_data2)
        .map(|data| data.as_slice())
        .unwrap_or_default();
    if is_secp256k1 {
        let pubkey1 = try_or_else!(SECPublicKey::try_from(pubkey_data1), || -2);
        let pubkey2 = try_or_else!(SECPublicKey::try_from(pubkey_data2), || -2);
        pubkey1.compare(&pubkey2) as i8
    } else {
        let pubkey1 = try_or_else!(NISTPublicKey::try_from(pubkey_data1), || -2);
        let pubkey2 = try_or_else!(NISTPublicKey::try_from(pubkey_data2), || -2);
        pubkey1.compare(&pubkey2) as i8
    }
}

/// Computes the shared key.
///
/// \param private_key *non-null* pointer to a block of data corresponding to the private key.
/// \param pubkey_data *non-null* pointer to a block of data corresponding to the public key.
/// \param is_secp256k1 whether the private key is a secp256k1 private key.
/// \return the shared key.
#[tw_ffi(ty = static_function, class = TWECDSA, name = SharedKey)]
#[no_mangle]
pub unsafe extern "C" fn tw_ecdsa_shared_key(
    private_key: Nonnull<TWData>,
    pubkey_data: Nonnull<TWData>,
    is_secp256k1: bool,
) -> NullableMut<TWData> {
    let private_key = TWData::from_ptr_as_ref(private_key)
        .map(|data| data.as_slice())
        .unwrap_or_default();
    let pubkey_data = TWData::from_ptr_as_ref(pubkey_data)
        .map(|data| data.as_slice())
        .unwrap_or_default();
    let shared_key = if is_secp256k1 {
        let private_key = try_or_else!(SECPrivateKey::try_from(private_key), std::ptr::null_mut);
        let pubkey = try_or_else!(SECPublicKey::try_from(pubkey_data), std::ptr::null_mut);
        private_key.ecies_shared_key(&pubkey)
    } else {
        let private_key = try_or_else!(NISSecPrivateKey::try_from(private_key), std::ptr::null_mut);
        let pubkey = try_or_else!(NISTPublicKey::try_from(pubkey_data), std::ptr::null_mut);
        private_key.ecies_shared_key(&pubkey)
    };
    TWData::from(shared_key.to_vec()).into_ptr()
}
