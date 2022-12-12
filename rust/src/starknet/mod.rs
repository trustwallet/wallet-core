// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use std::ffi::{c_char, CStr};
use starknet_crypto::{FieldElement, get_public_key, Signature};
use starknet_signers::{SigningKey, VerifyingKey};
use crate::memory;

pub fn field_element_from_be_hex(hex: &str) -> FieldElement {
    let decoded = hex::decode(hex.trim_start_matches("0x")).unwrap();

    if decoded.len() > 32 {
        panic!("hex string too long");
    }

    let mut buffer = [0u8; 32];
    buffer[(32 - decoded.len())..].copy_from_slice(&decoded[..]);

    FieldElement::from_bytes_be(&buffer).unwrap()
}

#[no_mangle]
pub extern fn starknet_pubkey_from_private(priv_key: *const c_char) -> *const c_char {
    let s = unsafe { CStr::from_ptr(priv_key).to_str().unwrap() };
    let private_key = field_element_from_be_hex(s);
    let hx = format!("{:#02x}", get_public_key(&private_key));
    memory::c_string_standalone(hx)
}

#[no_mangle]
pub extern fn starknet_sign(priv_key: *const c_char, hash: *const c_char) -> *const c_char {
    let s = unsafe { CStr::from_ptr(priv_key).to_str().unwrap() };
    let private_key = field_element_from_be_hex(s);
    let h = unsafe { CStr::from_ptr(hash).to_str().unwrap() };
    let hash_field = field_element_from_be_hex(h);
    let signing_key = SigningKey::from_secret_scalar(private_key);
    let signature = signing_key.sign(&hash_field).unwrap();
    let hx = format!("{}", signature);
    memory::c_string_standalone(hx)
}

#[no_mangle]
pub extern fn starknet_verify(pub_key: *const c_char, hash: *const c_char, r: *const c_char,  s: *const c_char) -> bool {
    let pub_key = unsafe { field_element_from_be_hex(CStr::from_ptr(pub_key).to_str().unwrap()) };
    let hash = unsafe { field_element_from_be_hex(CStr::from_ptr(hash).to_str().unwrap()) };
    let r = unsafe { field_element_from_be_hex(CStr::from_ptr(r).to_str().unwrap()) };
    let s = unsafe { field_element_from_be_hex(CStr::from_ptr(s).to_str().unwrap()) };
    let verifying_key = VerifyingKey::from_scalar(pub_key);
    verifying_key.verify(&hash, &Signature { r, s }).unwrap_or(false)
}
