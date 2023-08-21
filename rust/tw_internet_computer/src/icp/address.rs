use candid::Principal;
use ic_ledger_types::{AccountIdentifier, DEFAULT_SUBACCOUNT};
use tw_keypair::tw::PublicKey;

/// Checks if the provided string is a valid ICP account identifier.
pub fn is_valid(address: &str) -> bool {
    if address.len() != 64 {
        return false;
    }

    AccountIdentifier::from_hex(address).is_ok()
}

#[derive(Debug, PartialEq)]
pub enum AddressFromPublicKeyError {
    InvalidSecp256k1,
}

/// Creates an ICP account identifier from the provided public key.
pub fn from_public_key(public_key: &PublicKey) -> Result<String, AddressFromPublicKeyError> {
    let key = public_key
        .to_secp256k1()
        .ok_or(AddressFromPublicKeyError::InvalidSecp256k1)?;
    let der_encoded_bytes = key.der_encoded();
    let principal = Principal::self_authenticating(der_encoded_bytes);
    let account_id = AccountIdentifier::new(&principal, &DEFAULT_SUBACCOUNT);
    Ok(account_id.to_hex())
}
