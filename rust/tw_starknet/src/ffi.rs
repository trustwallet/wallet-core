// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#![allow(clippy::missing_safety_doc)]

use crate::key_pair;
use std::ffi::{c_char, CStr};
use tw_memory::ffi::c_result::{CBoolResult, CStrResult, ErrorCode};

#[repr(C)]
pub enum CStarknetCode {
    Ok = 0,
    InvalidInput = 1,
    PrivKeyError = 2,
}

impl From<CStarknetCode> for ErrorCode {
    fn from(code: CStarknetCode) -> Self {
        code as ErrorCode
    }
}

impl From<key_pair::StarknetKeyPairError> for CStarknetCode {
    fn from(_: key_pair::StarknetKeyPairError) -> Self {
        CStarknetCode::PrivKeyError
    }
}

/// Returns a StarkNet pubkey corresponding to the given `priv_key`.
/// \param priv_key - *non-null* C-compatible, nul-terminated string.
/// \return *non-null* C-compatible, nul-terminated string.
#[no_mangle]
pub unsafe extern "C" fn starknet_pubkey_from_private(priv_key: *const c_char) -> CStrResult {
    let priv_key = match CStr::from_ptr(priv_key).to_str() {
        Ok(priv_key) => priv_key,
        Err(_) => return CStrResult::error(CStarknetCode::InvalidInput),
    };
    key_pair::starknet_pubkey_from_private(priv_key)
        .map(tw_memory::c_string_standalone)
        .map_err(CStarknetCode::from)
        .into()
}

/// Signs the input `hash` with the given `priv_key` and returns a signature compatible with StarkNet.
/// \param priv_key *non-null* C-compatible, nul-terminated string.
/// \param hash *non-null* C-compatible, nul-terminated string.
/// \return *non-null* C-compatible, nul-terminated string.
#[no_mangle]
pub unsafe extern "C" fn starknet_sign(priv_key: *const c_char, hash: *const c_char) -> CStrResult {
    let priv_key = match CStr::from_ptr(priv_key).to_str() {
        Ok(priv_key) => priv_key,
        Err(_) => return CStrResult::error(CStarknetCode::InvalidInput),
    };
    let hash = match CStr::from_ptr(hash).to_str() {
        Ok(hash) => hash,
        Err(_) => return CStrResult::error(CStarknetCode::InvalidInput),
    };
    key_pair::starknet_sign(priv_key, hash)
        .map(tw_memory::c_string_standalone)
        .map_err(CStarknetCode::from)
        .into()
}

/// Verifies if the given signature (`r` and `s`) is valid over a message `hash` given a StarkNet `pub_key`.
/// \param pub_key *non-null* C-compatible, nul-terminated string.
/// \param hash *non-null* C-compatible, nul-terminated string.
/// \param r *non-null* C-compatible, nul-terminated string.
/// \param s *non-null* C-compatible, nul-terminated string.
/// \return true if the signature is valid.
#[no_mangle]
pub unsafe extern "C" fn starknet_verify(
    pub_key: *const c_char,
    hash: *const c_char,
    r: *const c_char,
    s: *const c_char,
) -> CBoolResult {
    macro_rules! parse_c_str {
        ($s:expr) => {
            match CStr::from_ptr($s).to_str() {
                Ok(s) => s,
                Err(_) => return CBoolResult::error(CStarknetCode::InvalidInput),
            }
        };
    }

    let pub_key = parse_c_str!(pub_key);
    let hash = parse_c_str!(hash);
    let r = parse_c_str!(r);
    let s = parse_c_str!(s);

    key_pair::starknet_verify(pub_key, hash, r, s)
        .map_err(CStarknetCode::from)
        .into()
}
