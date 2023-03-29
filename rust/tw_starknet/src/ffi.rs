// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::key_pair;
use std::ffi::{c_char, CStr};

/// Returns a StarkNet pubkey corresponding to the given `priv_key`.
/// \param priv_key - *non-null* C-compatible, nul-terminated string.
/// \return *non-null* C-compatible, nul-terminated string.
#[no_mangle]
pub unsafe extern "C" fn starknet_pubkey_from_private(priv_key: *const c_char) -> *const c_char {
    let priv_key = CStr::from_ptr(priv_key).to_str().unwrap();
    let hex = key_pair::starknet_pubkey_from_private(priv_key).unwrap_or_default();
    tw_memory::c_string_standalone(hex)
}

/// Signs the input `hash` with the given `priv_key` and returns a signature compatible with StarkNet.
/// \param priv_key *non-null* C-compatible, nul-terminated string.
/// \param hash *non-null* C-compatible, nul-terminated string.
/// \return *non-null* C-compatible, nul-terminated string.
#[no_mangle]
pub unsafe extern "C" fn starknet_sign(
    priv_key: *const c_char,
    hash: *const c_char,
) -> *const c_char {
    let priv_key = CStr::from_ptr(priv_key).to_str().unwrap();
    let hash = CStr::from_ptr(hash).to_str().unwrap();
    let hex = key_pair::starknet_sign(priv_key, hash).unwrap_or_default();
    tw_memory::c_string_standalone(hex)
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
) -> bool {
    let pub_key = CStr::from_ptr(pub_key).to_str().unwrap();
    let hash = CStr::from_ptr(hash).to_str().unwrap();
    let r = CStr::from_ptr(r).to_str().unwrap();
    let s = CStr::from_ptr(s).to_str().unwrap();

    key_pair::starknet_verify(pub_key, hash, r, s).unwrap_or_default()
}
