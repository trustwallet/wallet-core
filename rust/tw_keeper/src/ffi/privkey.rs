// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::ffi::pubkey::TWPublicKey;
use crate::ffi::{TWCurve, TWPublicKeyType};
use crate::{secp256k1, Error};
use tw_hash::H256;
use tw_memory::ffi::c_byte_array::CByteArray;
use tw_memory::ffi::c_byte_array_ref::CByteArrayRef;
use tw_memory::ffi::RawPtrTrait;
use tw_utils::try_or_else;

pub struct TWPrivateKey {
    bytes: Vec<u8>,
}

impl RawPtrTrait for TWPrivateKey {}

impl TWPrivateKey {
    /// The number of bytes in a private key.
    pub const SIZE: usize = 32;
    /// The number of bytes in a Cardano key (two extended ed25519 keys + chain code).
    pub const CARDANO_SIZE: usize = 2 * 3 * 32;

    pub fn new(bytes: Vec<u8>) -> Result<TWPrivateKey, Error> {
        if !Self::is_valid_general(&bytes) {
            return Err(Error::InvalidSecretKey);
        }
        Ok(TWPrivateKey { bytes })
    }

    /// Checks if the given bytes are valid in general (without a concrete curve).
    pub fn is_valid_general(bytes: &[u8]) -> bool {
        if bytes.len() != Self::SIZE && bytes.len() != Self::CARDANO_SIZE {
            return false;
        }
        // Check for zero address.
        !bytes.iter().all(|byte| *byte == 0)
    }

    pub fn is_valid(bytes: &[u8], curve: TWCurve) -> bool {
        if !Self::is_valid_general(bytes) {
            return false;
        }
        match curve {
            TWCurve::Secp256k1 => secp256k1::PrivateKey::try_from(bytes).is_ok(),
        }
    }

    pub fn sign(&self, hash: &[u8], curve: TWCurve) -> Result<Vec<u8>, Error> {
        let private = match curve {
            TWCurve::Secp256k1 => secp256k1::PrivateKey::try_from(self.bytes.as_slice())?,
        };
        let hash_to_sign = H256::try_from(hash).map_err(|_| Error::InvalidSignMessage)?;
        private
            .sign(hash_to_sign)
            .map(|sig| sig.to_bytes().to_vec())
    }

    pub fn get_public_key_by_type(&self, ty: TWPublicKeyType) -> Result<TWPublicKey, Error> {
        match ty {
            TWPublicKeyType::Secp256k1 => {
                let privkey = secp256k1::PrivateKey::try_from(self.bytes.as_slice())?;
                let pubkey_bytes = privkey.public().compressed();
                Ok(TWPublicKey::new_unchecked(
                    pubkey_bytes.to_vec(),
                    TWPublicKeyType::Secp256k1,
                ))
            },
            TWPublicKeyType::Secp256k1Extended => {
                let privkey = secp256k1::PrivateKey::try_from(self.bytes.as_slice())?;
                let pubkey_bytes = privkey.public().uncompressed();
                Ok(TWPublicKey::new_unchecked(
                    pubkey_bytes.to_vec(),
                    TWPublicKeyType::Secp256k1Extended,
                ))
            },
        }
    }
}

#[no_mangle]
pub unsafe extern "C" fn tw_private_key_create_with_data(
    input: *const u8,
    input_len: usize,
) -> *mut TWPrivateKey {
    let bytes_ref = CByteArrayRef::new(input, input_len);
    let bytes = try_or_else!(bytes_ref.to_vec(), std::ptr::null_mut);

    TWPrivateKey::new(bytes)
        .map(TWPrivateKey::into_ptr)
        // Return null if the private key is invalid.
        .unwrap_or_else(|_| std::ptr::null_mut())
}

#[no_mangle]
pub unsafe extern "C" fn tw_private_key_delete(key: *mut TWPrivateKey) {
    // Take the ownership back to rust and drop the owner.
    let _ = TWPrivateKey::from_ptr(key);
}

// #[no_mangle]
// pub unsafe fn tw_private_key_is_valid(key: *const u8, key_len: usize, curve: u32) -> bool {
//     let curve = match TWCurve::from_raw(curve) {
//         Some(curve) => curve,
//         None => return false,
//     };
//     let priv_key_slice = match CByteArrayRef::new(key, key_len).as_slice() {
//         Some(pk) => pk,
//         None => return false,
//     };
//     TWPrivateKey::is_valid(priv_key_slice, curve)
// }

#[no_mangle]
pub unsafe fn tw_private_key_sign(
    key: *mut TWPrivateKey,
    hash: *const u8,
    hash_len: usize,
    curve: u32,
) -> CByteArray {
    let curve = match TWCurve::from_raw(curve) {
        Some(curve) => curve,
        None => return CByteArray::empty(),
    };
    let private = &*key;
    let hash_to_sign = try_or_else!(
        CByteArrayRef::new(hash, hash_len).as_slice(),
        CByteArray::empty
    );

    // Return an empty signature if an error occurs.
    let sig = private.sign(hash_to_sign, curve).unwrap_or_default();
    CByteArray::new(sig)
}

#[no_mangle]
pub unsafe fn tw_private_key_get_public_key_by_type(
    key: *mut TWPrivateKey,
    pubkey_type: u32,
) -> *mut TWPublicKey {
    let ty = try_or_else!(TWPublicKeyType::from_raw(pubkey_type), std::ptr::null_mut);
    let private = try_or_else!(TWPrivateKey::from_ptr_as_ref(key), std::ptr::null_mut);
    private
        .get_public_key_by_type(ty)
        .map(TWPublicKey::into_ptr)
        .unwrap_or_else(|_| std::ptr::null_mut())
}

// #[no_mangle]
// pub unsafe fn tw_private_key_get_shared_key(
//     key: *mut TWPrivateKey,
//     hash: *const u8,
//     hash_len: usize,
//     curve: u32,
// ) -> CByteArray {
//     let curve = match TWCurve::from_raw(curve) {
//         Some(curve) => curve,
//         None => return CByteArray::empty(),
//     };
//     let private = &*key;
//
//     let hash = CByteArrayRef::new(hash, hash_len);
//     let hash_to_sign = match hash.as_slice() {
//         Some(hash) => hash,
//         None => return CByteArray::empty(),
//     };
//
//     // Return an empty signature if an error occurs.
//     let sig = private.sign(hash_to_sign, curve).unwrap_or_default();
//     CByteArray::new(sig)
// }
