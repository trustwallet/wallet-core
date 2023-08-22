use std::str::FromStr;

use tw_keypair::ecdsa::secp256k1::PublicKey;

#[derive(Debug, Clone)]
pub struct Principal(pub(crate) candid::Principal);

impl Principal {
    pub fn new() -> Self {
        Self(candid::Principal::anonymous())
    }

    pub fn self_authenticating<P: AsRef<[u8]>>(public_key: P) -> Self {
        Self(candid::Principal::self_authenticating(public_key))
    }

    pub fn from_text<S: AsRef<str>>(
        text: S,
    ) -> Result<Self, candid::types::principal::PrincipalError> {
        let inner = candid::Principal::from_text(text)?;
        Ok(Self(inner))
    }
}

impl FromStr for Principal {
    type Err = candid::types::principal::PrincipalError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        Self::from_text(s)
    }
}

impl From<PublicKey> for Principal {
    fn from(public_key: PublicKey) -> Self {
        Self::self_authenticating(public_key.der_encoded())
    }
}

impl From<&PublicKey> for Principal {
    fn from(public_key: &PublicKey) -> Self {
        Self::self_authenticating(public_key.der_encoded())
    }
}
