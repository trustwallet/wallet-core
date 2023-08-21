use candid::Principal;

use ic_ledger_types::{AccountIdentifier, DEFAULT_SUBACCOUNT};
use pkcs8::EncodePublicKey;

#[derive(Debug, PartialEq)]
pub enum EncodePrincipalError {
    InvalidPublicKey,
    FailedDerEncode,
}

fn encode_public_key_to_der(
    public_key_bytes: &[u8],
) -> Result<der::Document, EncodePrincipalError> {
    let t = k256::PublicKey::from_sec1_bytes(public_key_bytes)
        .map_err(|_| EncodePrincipalError::InvalidPublicKey)?;
    t.to_public_key_der()
        .map_err(|_| EncodePrincipalError::FailedDerEncode)
}

pub fn encode_public_key_to_principal(
    public_key_bytes: &[u8],
) -> Result<Vec<u8>, EncodePrincipalError> {
    let der_encoded_public_key = encode_public_key_to_der(public_key_bytes)?;
    let principal = Principal::self_authenticating(der_encoded_public_key.as_bytes());
    Ok(principal.as_slice().to_vec())
}

pub fn encode_textual_principal(principal_bytes: &[u8]) -> String {
    Principal::from_slice(principal_bytes).to_text()
}

pub fn principal_to_account_identifier(principal_bytes: &[u8]) -> String {
    let principal = Principal::from_slice(principal_bytes);
    let account_id = AccountIdentifier::new(&principal, &DEFAULT_SUBACCOUNT);
    account_id.to_hex()
}
