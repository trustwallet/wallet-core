// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::ecdsa::{nist256p1, secp256k1};
use crate::schnorr;
use crate::traits::VerifyingKeyTrait;
use crate::tw::PublicKeyType;
use crate::{ed25519, starkex, KeyPairError, KeyPairResult};
use tw_misc::traits::ToBytesVec;
use tw_misc::try_or_false;

/// Represents a public key that can be used to verify signatures and messages.
#[derive(Clone)]
#[non_exhaustive]
pub enum PublicKey {
    Secp256k1(secp256k1::PublicKey),
    Secp256k1Extended(secp256k1::PublicKey),
    Nist256p1(nist256p1::PublicKey),
    Nist256p1Extended(nist256p1::PublicKey),
    Ed25519(ed25519::sha512::PublicKey),
    Ed25519Blake2b(ed25519::blake2b::PublicKey),
    Curve25519Waves(ed25519::waves::PublicKey),
    Ed25519ExtendedCardano(Box<ed25519::cardano::ExtendedPublicKey>),
    Starkex(starkex::PublicKey),
    Schnorr(schnorr::PublicKey),
}

impl PublicKey {
    /// Validates the given `bytes` using the `ty` public key type and creates a public key from it.
    pub fn new(bytes: Vec<u8>, ty: PublicKeyType) -> KeyPairResult<PublicKey> {
        match ty {
            PublicKeyType::Secp256k1 if secp256k1::PublicKey::COMPRESSED == bytes.len() => {
                let pubkey = secp256k1::PublicKey::try_from(bytes.as_slice())?;
                Ok(PublicKey::Secp256k1(pubkey))
            },
            PublicKeyType::Secp256k1Extended
                if secp256k1::PublicKey::UNCOMPRESSED == bytes.len() =>
            {
                let pubkey = secp256k1::PublicKey::try_from(bytes.as_slice())?;
                Ok(PublicKey::Secp256k1Extended(pubkey))
            },
            PublicKeyType::Nist256p1 if nist256p1::PublicKey::COMPRESSED == bytes.len() => {
                let pubkey = nist256p1::PublicKey::try_from(bytes.as_slice())?;
                Ok(PublicKey::Nist256p1(pubkey))
            },
            PublicKeyType::Nist256p1Extended
                if nist256p1::PublicKey::UNCOMPRESSED == bytes.len() =>
            {
                let pubkey = nist256p1::PublicKey::try_from(bytes.as_slice())?;
                Ok(PublicKey::Nist256p1Extended(pubkey))
            },
            PublicKeyType::Ed25519 if ed25519::sha512::PublicKey::LEN == bytes.len() => {
                let pubkey = ed25519::sha512::PublicKey::try_from(bytes.as_slice())?;
                Ok(PublicKey::Ed25519(pubkey))
            },
            PublicKeyType::Ed25519Blake2b if ed25519::blake2b::PublicKey::LEN == bytes.len() => {
                let pubkey = ed25519::blake2b::PublicKey::try_from(bytes.as_slice())?;
                Ok(PublicKey::Ed25519Blake2b(pubkey))
            },
            PublicKeyType::Curve25519Waves if ed25519::waves::PublicKey::LEN == bytes.len() => {
                let pubkey = ed25519::waves::PublicKey::try_from(bytes.as_slice())?;
                Ok(PublicKey::Curve25519Waves(pubkey))
            },
            PublicKeyType::Ed25519ExtendedCardano
                if ed25519::cardano::ExtendedPublicKey::LEN == bytes.len() =>
            {
                let pubkey = ed25519::cardano::ExtendedPublicKey::try_from(bytes.as_slice())?;
                Ok(PublicKey::Ed25519ExtendedCardano(Box::new(pubkey)))
            },
            PublicKeyType::Starkex => {
                let pubkey = starkex::PublicKey::try_from(bytes.as_slice())?;
                Ok(PublicKey::Starkex(pubkey))
            },
            PublicKeyType::Schnorr => {
                let pubkey = schnorr::PublicKey::try_from(bytes.as_slice())?;
                Ok(PublicKey::Schnorr(pubkey))
            },
            _ => Err(KeyPairError::InvalidPublicKey),
        }
    }

    /// Checks if the given `bytes` is valid using `ty` public key type.
    pub fn is_valid(bytes: Vec<u8>, ty: PublicKeyType) -> bool {
        PublicKey::new(bytes, ty).is_ok()
    }

    /// Verifies if the given `message` was signed using a private key associated with the public key.
    pub fn verify(&self, sig: &[u8], message: &[u8]) -> bool {
        fn verify_impl<Key>(verifying_key: &Key, sig: &[u8], message: &[u8]) -> bool
        where
            Key: VerifyingKeyTrait,
        {
            let verify_sig =
                try_or_false!(<Key as VerifyingKeyTrait>::VerifySignature::try_from(sig));
            let message = try_or_false!(<Key as VerifyingKeyTrait>::SigningMessage::try_from(
                message
            ));
            verifying_key.verify(verify_sig, message)
        }

        match self {
            PublicKey::Secp256k1(secp) | PublicKey::Secp256k1Extended(secp) => {
                verify_impl(secp, sig, message)
            },
            PublicKey::Nist256p1(nist) | PublicKey::Nist256p1Extended(nist) => {
                verify_impl(nist, sig, message)
            },
            PublicKey::Ed25519(ed) => verify_impl(ed, sig, message),
            PublicKey::Ed25519Blake2b(blake) => verify_impl(blake, sig, message),
            PublicKey::Curve25519Waves(waves) => verify_impl(waves, sig, message),
            PublicKey::Ed25519ExtendedCardano(cardano) => {
                verify_impl(cardano.as_ref(), sig, message)
            },
            PublicKey::Starkex(stark) => verify_impl(stark, sig, message),
            PublicKey::Schnorr(schnorr) => verify_impl(schnorr, sig, message),
        }
    }

    /// Returns the raw data of the public key.
    pub fn to_bytes(&self) -> Vec<u8> {
        match self {
            PublicKey::Secp256k1(secp) => secp.compressed().into_vec(),
            PublicKey::Secp256k1Extended(secp) => secp.uncompressed().into_vec(),
            PublicKey::Nist256p1(nist) => nist.compressed().into_vec(),
            PublicKey::Nist256p1Extended(nist) => nist.uncompressed().into_vec(),
            PublicKey::Ed25519(ed) => ed.to_vec(),
            PublicKey::Ed25519Blake2b(blake) => blake.to_vec(),
            PublicKey::Curve25519Waves(waves) => waves.to_vec(),
            PublicKey::Ed25519ExtendedCardano(cardano) => cardano.to_vec(),
            PublicKey::Starkex(stark) => stark.to_vec(),
            PublicKey::Schnorr(schnorr) => schnorr.to_vec(),
        }
    }

    /// Returns a `secp256k1` public key if the key type is matched.
    pub fn to_secp256k1(&self) -> Option<&secp256k1::PublicKey> {
        match self {
            PublicKey::Secp256k1(secp256k1) | PublicKey::Secp256k1Extended(secp256k1) => {
                Some(secp256k1)
            },
            _ => None,
        }
    }

    pub fn to_ed25519(&self) -> Option<&ed25519::sha512::PublicKey> {
        match self {
            PublicKey::Ed25519(ed25519) => Some(ed25519),
            _ => None,
        }
    }

    /// Returns a `schnorr` public key if the key type is matched.
    pub fn to_schnorr(&self) -> Option<&schnorr::PublicKey> {
        match self {
            PublicKey::Schnorr(ref schnorr) => Some(schnorr),
            _ => None,
        }
    }

    /// Returns a public key type.
    pub fn public_key_type(&self) -> PublicKeyType {
        match self {
            PublicKey::Secp256k1(_) => PublicKeyType::Secp256k1,
            PublicKey::Secp256k1Extended(_) => PublicKeyType::Secp256k1Extended,
            PublicKey::Nist256p1(_) => PublicKeyType::Nist256p1,
            PublicKey::Nist256p1Extended(_) => PublicKeyType::Nist256p1Extended,
            PublicKey::Ed25519(_) => PublicKeyType::Ed25519,
            PublicKey::Ed25519Blake2b(_) => PublicKeyType::Ed25519Blake2b,
            PublicKey::Curve25519Waves(_) => PublicKeyType::Curve25519Waves,
            PublicKey::Ed25519ExtendedCardano(_) => PublicKeyType::Ed25519ExtendedCardano,
            PublicKey::Starkex(_) => PublicKeyType::Starkex,
            PublicKey::Schnorr(_) => PublicKeyType::Schnorr,
        }
    }
}
