// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#![allow(clippy::missing_safety_doc)]

use crate::ecdsa::der;
use tw_memory::ffi::c_byte_array::{CByteArray, CByteArrayResult};
use tw_memory::ffi::c_byte_array_ref::CByteArrayRef;
use tw_memory::ffi::c_result::ErrorCode;

#[repr(C)]
pub enum CKeyPairError {
    Ok = 0,
    InvalidSignature = 1,
}

impl From<CKeyPairError> for ErrorCode {
    fn from(e: CKeyPairError) -> Self {
        e as ErrorCode
    }
}

/// Parses the given ECDSA signature from ASN.1 DER encoded bytes.
///
/// \param encoded *non-null* byte array.
/// \param encoded_len the length of the `encoded` array.
/// \return C-compatible byte array.
#[no_mangle]
pub unsafe extern "C" fn ecdsa_signature_from_asn_der(
    encoded: *const u8,
    encoded_len: usize,
) -> CByteArrayResult {
    let encoded_ref = CByteArrayRef::new(encoded, encoded_len);
    let Some(encoded) = encoded_ref.to_vec() else {
        return CByteArrayResult::error(CKeyPairError::InvalidSignature);
    };

    der::Signature::from_bytes(encoded.as_slice())
        .map(|sign| CByteArray::from(sign.to_vec()))
        .map_err(|_| CKeyPairError::InvalidSignature)
        .into()
}
