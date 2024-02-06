// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::ecdsa::{nist256p1, secp256k1};
use crate::traits::SigningKeyTrait;
use crate::tw::{Curve, PublicKey, PublicKeyType};
use crate::{ed25519, starkex, KeyPairError, KeyPairResult};
use std::ops::Range;
use tw_hash::H256;
use tw_misc::traits::ToBytesVec;
use zeroize::ZeroizeOnDrop;

/// Represents a private key that can be used to sign messages with different elliptic curves.
///
/// TODO add `secp256k1: Once<each_curve::PrivateKey>` for each curve.
#[derive(ZeroizeOnDrop)]
pub struct PrivateKey {
    bytes: Vec<u8>,
}

/// cbindgen:ignore
impl PrivateKey {
    /// The number of bytes in a private key.
    const SIZE: usize = 32;
    const CARDANO_SIZE: usize = ed25519::cardano::ExtendedPrivateKey::LEN;

    const KEY_RANGE: Range<usize> = 0..Self::SIZE;
    const EXTENDED_CARDANO_RANGE: Range<usize> = 0..Self::CARDANO_SIZE;

    /// Validates the given `bytes` secret and creates a private key.
    pub fn new(bytes: Vec<u8>) -> KeyPairResult<PrivateKey> {
        if !Self::is_valid_general(&bytes) {
            return Err(KeyPairError::InvalidSecretKey);
        }
        Ok(PrivateKey { bytes })
    }

    /// Returns the 32 byte array - the essential private key data.
    pub fn key(&self) -> H256 {
        assert!(
            self.bytes.len() >= Self::SIZE,
            "'PrivateKey::bytes' has an unexpected length"
        );
        H256::try_from(&self.bytes[Self::KEY_RANGE])
            .expect("H256 and KEY_RANGE must be 32 byte length")
    }

    /// Returns the 192 byte array - the essential cardano extended private key data.
    pub fn extended_cardano_key(&self) -> KeyPairResult<&[u8]> {
        if self.bytes.len() != Self::CARDANO_SIZE {
            return Err(KeyPairError::InvalidSecretKey);
        }
        Ok(&self.bytes[Self::EXTENDED_CARDANO_RANGE])
    }

    /// Checks if the given `bytes` secret is valid in general (without a concrete curve).
    pub fn is_valid_general(bytes: &[u8]) -> bool {
        if bytes.len() != Self::SIZE && bytes.len() != Self::CARDANO_SIZE {
            return false;
        }
        // Check for zero address.
        !bytes.iter().all(|byte| *byte == 0)
    }

    /// Checks if the given `bytes` secret is valid.
    pub fn is_valid(bytes: &[u8], curve: Curve) -> bool {
        if !Self::is_valid_general(bytes) {
            return false;
        }
        match curve {
            Curve::Secp256k1 => secp256k1::PrivateKey::try_from(&bytes[Self::KEY_RANGE]).is_ok(),
            Curve::Ed25519 => {
                ed25519::sha512::PrivateKey::try_from(&bytes[Self::KEY_RANGE]).is_ok()
            },
            Curve::Ed25519Blake2bNano => {
                ed25519::blake2b::PrivateKey::try_from(&bytes[Self::KEY_RANGE]).is_ok()
            },
            Curve::Curve25519Waves => {
                ed25519::waves::PrivateKey::try_from(&bytes[Self::KEY_RANGE]).is_ok()
            },
            Curve::Nist256p1 => nist256p1::PrivateKey::try_from(&bytes[Self::KEY_RANGE]).is_ok(),
            Curve::Ed25519ExtendedCardano => {
                ed25519::cardano::ExtendedPrivateKey::try_from(&bytes[Self::EXTENDED_CARDANO_RANGE])
                    .is_ok()
            },
            Curve::Starkex => starkex::PrivateKey::try_from(&bytes[Self::KEY_RANGE]).is_ok(),
        }
    }

    /// Signs a `message` with using the given elliptic curve.
    pub fn sign(&self, message: &[u8], curve: Curve) -> KeyPairResult<Vec<u8>> {
        fn sign_impl<Key>(signing_key: Key, message: &[u8]) -> KeyPairResult<Vec<u8>>
        where
            Key: SigningKeyTrait,
        {
            let hash_to_sign = <Key as SigningKeyTrait>::SigningMessage::try_from(message)
                .map_err(|_| KeyPairError::InvalidSignMessage)?;
            signing_key.sign(hash_to_sign).map(|sig| sig.to_vec())
        }

        match curve {
            Curve::Secp256k1 => sign_impl(self.to_secp256k1_privkey()?, message),
            Curve::Ed25519 => sign_impl(self.to_ed25519()?, message),
            Curve::Ed25519Blake2bNano => sign_impl(self.to_ed25519_blake2b()?, message),
            Curve::Curve25519Waves => sign_impl(self.to_curve25519_waves()?, message),
            Curve::Nist256p1 => sign_impl(self.to_nist256p1_privkey()?, message),
            Curve::Ed25519ExtendedCardano => {
                sign_impl(self.to_ed25519_extended_cardano()?, message)
            },
            Curve::Starkex => sign_impl(self.to_starkex_privkey()?, message),
        }
    }

    /// Returns the public key associated with the `self` private key and `ty` public key type.
    pub fn get_public_key_by_type(&self, ty: PublicKeyType) -> KeyPairResult<PublicKey> {
        match ty {
            PublicKeyType::Secp256k1 => {
                let privkey = self.to_secp256k1_privkey()?;
                Ok(PublicKey::Secp256k1(privkey.public()))
            },
            PublicKeyType::Secp256k1Extended => {
                let privkey = self.to_secp256k1_privkey()?;
                Ok(PublicKey::Secp256k1Extended(privkey.public()))
            },
            PublicKeyType::Nist256p1 => {
                let privkey = self.to_nist256p1_privkey()?;
                Ok(PublicKey::Nist256p1(privkey.public()))
            },
            PublicKeyType::Nist256p1Extended => {
                let privkey = self.to_nist256p1_privkey()?;
                Ok(PublicKey::Nist256p1Extended(privkey.public()))
            },
            PublicKeyType::Ed25519 => {
                let privkey = self.to_ed25519()?;
                Ok(PublicKey::Ed25519(privkey.public()))
            },
            PublicKeyType::Ed25519Blake2b => {
                let privkey = self.to_ed25519_blake2b()?;
                Ok(PublicKey::Ed25519Blake2b(privkey.public()))
            },
            PublicKeyType::Curve25519Waves => {
                let privkey = self.to_curve25519_waves()?;
                Ok(PublicKey::Curve25519Waves(privkey.public()))
            },
            PublicKeyType::Ed25519ExtendedCardano => {
                let privkey = self.to_ed25519_extended_cardano()?;
                Ok(PublicKey::Ed25519ExtendedCardano(Box::new(
                    privkey.public(),
                )))
            },
            PublicKeyType::Starkex => {
                let privkey = self.to_starkex_privkey()?;
                Ok(PublicKey::Starkex(privkey.public()))
            },
        }
    }

    /// Tries to convert [`PrivateKey::key`] to [`secp256k1::PrivateKey`].
    fn to_secp256k1_privkey(&self) -> KeyPairResult<secp256k1::PrivateKey> {
        secp256k1::PrivateKey::try_from(self.key().as_slice())
    }

    /// Tries to convert [`PrivateKey::key`] to [`nist256p1::PrivateKey`].
    fn to_nist256p1_privkey(&self) -> KeyPairResult<nist256p1::PrivateKey> {
        nist256p1::PrivateKey::try_from(self.key().as_slice())
    }

    /// Tries to convert [`PrivateKey::key`] to [`ed25519::sha512::PrivateKey`].
    fn to_ed25519(&self) -> KeyPairResult<ed25519::sha512::PrivateKey> {
        ed25519::sha512::PrivateKey::try_from(self.key().as_slice())
    }

    /// Tries to convert [`PrivateKey::key`] to [`ed25519::blake2b::PrivateKey`].
    fn to_ed25519_blake2b(&self) -> KeyPairResult<ed25519::blake2b::PrivateKey> {
        ed25519::blake2b::PrivateKey::try_from(self.key().as_slice())
    }

    /// Tries to convert [`PrivateKey::key`] to [`ed25519::waves::PrivateKey`].
    fn to_curve25519_waves(&self) -> KeyPairResult<ed25519::waves::PrivateKey> {
        ed25519::waves::PrivateKey::try_from(self.key().as_slice())
    }

    /// Tries to convert [`PrivateKey::extended_cardano_key`] to [`ed25519::cardano::ExtendedPrivateKey`].
    fn to_ed25519_extended_cardano(&self) -> KeyPairResult<ed25519::cardano::ExtendedPrivateKey> {
        ed25519::cardano::ExtendedPrivateKey::try_from(self.extended_cardano_key()?)
    }

    /// Tries to convert [`PrivateKey::key`] to [`starkex::PrivateKey`].
    fn to_starkex_privkey(&self) -> KeyPairResult<starkex::PrivateKey> {
        starkex::PrivateKey::try_from(self.key().as_slice())
    }
}
