use std::str::FromStr;

use tw_keypair::ecdsa::secp256k1::PublicKey;

/// Principals are generic identifiers for canisters, users and possibly
/// other concepts in the future. As far as most uses of the IC are
/// concerned they are opaque binary blobs with a length between
/// 0 and 29 bytes.
#[derive(Debug, Clone)]
pub struct Principal(pub(crate) candid::Principal);

impl Principal {
    /// Take in a set of bytes and create a self-authenticating principal.
    ///
    /// These have the form H(public_key) · 0x02 (29 bytes).
    /// An external user can use these ids as the sender of a request if
    /// they own the corresponding private key.
    pub fn self_authenticating<P: AsRef<[u8]>>(public_key: P) -> Self {
        Self(candid::Principal::self_authenticating(public_key))
    }

    /// Takes a textual representation of a principal and returns a [Principal].
    /// See: https://internetcomputer.org/docs/current/references/ic-interface-spec#textual-ids
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

impl From<&PublicKey> for Principal {
    /// Takes a Secp256k1 public key, DER-encodes the public key,
    /// and creates a principal from the encoding.
    fn from(public_key: &PublicKey) -> Self {
        Self::self_authenticating(public_key.der_encoded())
    }
}
