use starknet_crypto::{get_public_key, Signature};
use starknet_ff::FieldElement;
use starknet_signers::{SigningKey, VerifyingKey};

pub fn starknet_pubkey_from_private(priv_key: &str) -> String {
    let private_key = field_element_from_be_hex(priv_key);
    format!("{:#02x}", get_public_key(&private_key))
}

pub fn starknet_sign(priv_key: &str, hash: &str) -> String {
    let private_key = field_element_from_be_hex(priv_key);
    let hash_field = field_element_from_be_hex(hash);
    let signing_key = SigningKey::from_secret_scalar(private_key);
    let signature = signing_key.sign(&hash_field).unwrap();
    signature.to_string()
}

pub fn starknet_verify(pub_key: &str, hash: &str, r: &str, s: &str) -> bool {
    let pub_key = field_element_from_be_hex(pub_key);
    let hash = field_element_from_be_hex(hash);
    let r = field_element_from_be_hex(r);
    let s = field_element_from_be_hex(s);
    let verifying_key = VerifyingKey::from_scalar(pub_key);
    verifying_key
        .verify(&hash, &Signature { r, s })
        .unwrap_or(false)
}

/// TODO return an error instead.
fn field_element_from_be_hex(hex: &str) -> FieldElement {
    // TODO use tw_encoding
    let decoded = hex::decode(hex.trim_start_matches("0x")).unwrap();
    if decoded.len() > 32 {
        panic!("hex string too long");
    }

    let mut buffer = [0u8; 32];
    buffer[(32 - decoded.len())..].copy_from_slice(&decoded[..]);

    FieldElement::from_bytes_be(&buffer).unwrap()
}
