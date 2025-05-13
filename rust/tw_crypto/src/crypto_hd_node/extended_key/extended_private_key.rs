// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use bip32::{
    ChildNumber, DerivationPath, Error, ExtendedKey, ExtendedKeyAttrs, KeyFingerprint, Prefix,
    Result, KEY_SIZE,
};
use core::str::FromStr;
use sha2::digest::Mac;
use tw_encoding::hex::{self, ToHex};
use tw_hash::hmac::HmacSha512;
use tw_keypair::tw::Curve;
use zeroize::Zeroizing;

use crate::crypto_hd_node::extended_key::{
    bip32_private_key::BIP32PrivateKey, bip32_public_key::BIP32PublicKey,
    extended_public_key::ExtendedPublicKey,
};

use super::hd_version::HDVersion;

/// Extended private keys derived using BIP32.
///
/// Generic around a [`PrivateKey`] type. When the `secp256k1` feature of this
/// crate is enabled, the [`XPrv`] type provides a convenient alias for
/// extended ECDSA/secp256k1 private keys.
#[derive(Clone)]
pub struct ExtendedPrivateKey<K: BIP32PrivateKey> {
    /// Derived private key
    private_key: K,

    /// Extended key attributes.
    attrs: ExtendedKeyAttrs,
}

impl<K> ExtendedPrivateKey<K>
where
    K: BIP32PrivateKey,
{
    /// Create the root extended key for the given seed value.
    pub fn new<S>(seed: S) -> Result<Self>
    where
        S: AsRef<[u8]>,
    {
        let (private_key, chain_code) = if K::curve() == Curve::Ed25519ExtendedCardano {
            let mut digest = [0u8; 128];
            pbkdf2::pbkdf2_hmac::<sha2::Sha512>(&[], seed.as_ref(), 4096, &mut digest);

            let secret = &mut digest[0..96];
            secret[0] &= 0xf8;
            secret[31] &= 0x1f;
            secret[31] |= 0x40;

            (K::from_bytes(&digest[0..96])?, digest[64..96].try_into()?)
        } else {
            let domain_separator =
                hex::decode(&K::bip32_name().to_hex()).map_err(|_| Error::Crypto)?;

            let mut hmac = HmacSha512::new_from_slice(&domain_separator)?;
            hmac.update(seed.as_ref());

            let result = hmac.finalize().into_bytes();
            let (private_key, chain_code) = result.split_at(KEY_SIZE);
            (K::from_bytes(private_key)?, chain_code.try_into()?)
        };

        let attrs = ExtendedKeyAttrs {
            depth: 0,
            parent_fingerprint: KeyFingerprint::default(),
            child_number: ChildNumber::default(),
            chain_code,
        };

        Ok(ExtendedPrivateKey { private_key, attrs })
    }

    /// Derive a child key from the given [`DerivationPath`].
    pub fn derive_from_path(&self, path: &DerivationPath) -> Result<Self> {
        path.iter().fold(Ok(self.clone()), |maybe_key, child_num| {
            maybe_key.and_then(|key| key.derive_child(child_num))
        })
    }

    /// Derive a child key for a particular [`ChildNumber`].
    pub fn derive_child(&self, child_number: ChildNumber) -> Result<Self> {
        let depth = self.attrs.depth.checked_add(1).ok_or(Error::Depth)?;
        let (tweak, chain_code) = self
            .private_key
            .derive_tweak(&self.attrs.chain_code, child_number)?;

        // We should technically loop here if the tweak is zero or overflows
        // the order of the underlying elliptic curve group, incrementing the
        // index, however per "Child key derivation (CKD) functions":
        // https://github.com/bitcoin/bips/blob/master/bip-0032.mediawiki#child-key-derivation-ckd-functions
        //
        // > "Note: this has probability lower than 1 in 2^127."
        //
        // ...so instead, we simply return an error if this were ever to happen,
        // as the chances of it happening are vanishingly small.
        let private_key = self.private_key.derive_child(&tweak, child_number)?;

        let attrs = ExtendedKeyAttrs {
            parent_fingerprint: self.private_key.public_key().fingerprint(),
            child_number,
            chain_code,
            depth,
        };

        Ok(ExtendedPrivateKey { private_key, attrs })
    }

    /// Borrow the derived private key value.
    pub fn private_key(&self) -> &K {
        &self.private_key
    }

    /// Serialize the derived public key as bytes.
    pub fn public_key(&self) -> ExtendedPublicKey<K::BIP32PublicKey> {
        self.into()
    }

    /// Get attributes for this key such as depth, parent fingerprint,
    /// child number, and chain code.
    pub fn attrs(&self) -> &ExtendedKeyAttrs {
        &self.attrs
    }

    /// Serialize the raw private key as a byte array.
    pub fn to_bytes(&self) -> Vec<u8> {
        self.private_key.to_bytes()
    }

    /// Serialize the raw private key extension as a byte array.
    pub fn private_key_extension_bytes(&self) -> Vec<u8> {
        if K::curve() == Curve::Ed25519ExtendedCardano {
            return self.private_key.to_bytes()[32..64].to_vec();
        } else {
            vec![]
        }
    }

    /// Serialize this key as an [`ExtendedKey`].
    pub fn to_extended_key(&self, prefix: Prefix) -> Result<ExtendedKey> {
        if K::curve() == Curve::Ed25519ExtendedCardano {
            return Err(Error::Crypto);
        }
        // Add leading `0` byte
        let mut key_bytes = [0u8; KEY_SIZE + 1];
        key_bytes[1..].copy_from_slice(&self.to_bytes());

        Ok(ExtendedKey {
            prefix,
            attrs: self.attrs.clone(),
            key_bytes,
        })
    }

    /// Serialize this key as a self-[`Zeroizing`] `String`.
    pub fn to_string(&self, prefix: Prefix) -> Result<Zeroizing<String>> {
        Ok(Zeroizing::new(self.to_extended_key(prefix)?.to_string()))
    }
}

impl<K> FromStr for ExtendedPrivateKey<K>
where
    K: BIP32PrivateKey,
{
    type Err = Error;

    fn from_str(xpub: &str) -> Result<Self> {
        ExtendedKey::from_str(xpub)?.try_into()
    }
}

impl<K> TryFrom<ExtendedKey> for ExtendedPrivateKey<K>
where
    K: BIP32PrivateKey,
{
    type Error = Error;

    fn try_from(key: ExtendedKey) -> Result<Self> {
        let version: HDVersion = key.prefix.version().into();
        if version.is_private() && key.key_bytes[0] == 0 {
            Ok(ExtendedPrivateKey {
                private_key: K::from_bytes(&key.key_bytes[1..])?,
                attrs: key.attrs.clone(),
            })
        } else {
            Err(Error::Crypto)
        }
    }
}
