// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use bip32::{
    ChildNumber, DerivationPath, ExtendedKey, ExtendedKeyAttrs, KeyFingerprint, Prefix, KEY_SIZE,
};
use tw_misc::traits::ToBytesVec;

use crate::crypto_hd_node::error::{Error, Result};
use crate::crypto_hd_node::extended_key::{
    bip32_private_key::BIP32PrivateKey, bip32_public_key::BIP32PublicKey,
    extended_private_key::ExtendedPrivateKey,
};

use super::{extended_private_key::decode_base58, hd_version::HDVersion};
use tw_hash::hasher::Hasher;

/// Extended public keys derived using BIP32.
///
/// Generic around a [`PublicKey`] type. When the `secp256k1` feature of this
/// crate is enabled, the [`XPub`] type provides a convenient alias for
/// extended ECDSA/secp256k1 public keys.
#[derive(Clone, Debug, Eq, PartialEq, PartialOrd, Ord)]
pub struct ExtendedPublicKey<K: BIP32PublicKey> {
    /// Derived public key
    public_key: K,

    /// Extended key attributes.
    attrs: ExtendedKeyAttrs,
}

impl<K> ExtendedPublicKey<K>
where
    K: BIP32PublicKey,
{
    /// Obtain the non-extended public key value `K`.
    pub fn public_key(&self) -> &K {
        &self.public_key
    }

    /// Compute a 4-byte key fingerprint for this extended public key.
    pub fn fingerprint(&self, hasher: Hasher) -> KeyFingerprint {
        self.public_key().fingerprint(hasher)
    }

    /// Serialize the raw public key as a byte array (e.g. SEC1-encoded).
    pub fn to_bytes(&self) -> Vec<u8> {
        self.public_key.to_vec()
    }

    /// Serialize this key as an [`ExtendedKey`].
    pub fn to_extended_key(&self, prefix: Prefix) -> ExtendedKey {
        let bytes = self.to_bytes();

        let mut key_bytes = [0u8; KEY_SIZE + 1];
        if bytes.len() == KEY_SIZE {
            // Add leading `0` byte
            key_bytes[1..].copy_from_slice(&self.to_bytes());
        } else {
            key_bytes.copy_from_slice(&self.to_bytes());
        }

        ExtendedKey {
            prefix,
            attrs: self.attrs.clone(),
            key_bytes,
        }
    }

    pub fn derive_from_path(&self, path: &DerivationPath, hasher: Hasher) -> Result<Self> {
        path.iter().try_fold(self.clone(), |key, child_num| {
            key.derive_child(child_num, hasher)
        })
    }

    /// Derive a child key for a particular [`ChildNumber`].
    pub fn derive_child(&self, child_number: ChildNumber, hasher: Hasher) -> Result<Self> {
        let depth = self.attrs.depth.checked_add(1).ok_or(Error::InvalidDepth)?;
        let (public_key, chain_code) = self
            .public_key
            .derive_child(&self.attrs.chain_code, child_number)?;

        let attrs = ExtendedKeyAttrs {
            parent_fingerprint: self.public_key.fingerprint(hasher),
            child_number,
            chain_code,
            depth,
        };

        Ok(ExtendedPublicKey { public_key, attrs })
    }

    pub fn from_base58(xpub: &str, hasher: Hasher) -> Result<Self> {
        let extended_key = decode_base58(xpub, hasher)?;
        extended_key.try_into()
    }
}

impl<K> From<&ExtendedPrivateKey<K>> for ExtendedPublicKey<K::BIP32PublicKey>
where
    K: BIP32PrivateKey,
{
    fn from(xprv: &ExtendedPrivateKey<K>) -> ExtendedPublicKey<K::BIP32PublicKey> {
        ExtendedPublicKey {
            public_key: xprv.private_key().public_key(),
            attrs: xprv.attrs().clone(),
        }
    }
}

impl<K> TryFrom<ExtendedKey> for ExtendedPublicKey<K>
where
    K: BIP32PublicKey,
{
    type Error = Error;

    fn try_from(key: ExtendedKey) -> Result<Self> {
        let version = HDVersion::from_repr(key.prefix.version()).ok_or(Error::InvalidKeyData)?;
        if version.is_public() {
            Ok(ExtendedPublicKey {
                public_key: K::try_from(&key.key_bytes).map_err(|_| Error::InvalidKeyData)?,
                attrs: key.attrs.clone(),
            })
        } else {
            Err(Error::InvalidKeyData)
        }
    }
}

impl<K> ToBytesVec for ExtendedPublicKey<K>
where
    K: BIP32PublicKey,
{
    fn to_vec(&self) -> Vec<u8> {
        self.public_key.to_vec()
    }
}
