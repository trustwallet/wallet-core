use std::ffi::{c_char, CStr};
use starknet_crypto::{FieldElement, get_public_key};
use starknet_signers::SigningKey;
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
