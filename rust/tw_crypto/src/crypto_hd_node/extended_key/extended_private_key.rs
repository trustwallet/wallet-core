// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use bip32::{
    ChildNumber, DerivationPath, ExtendedKey, ExtendedKeyAttrs, KeyFingerprint, Prefix, Version,
    KEY_SIZE,
};
use sha2::digest::Mac;
use tw_encoding::hex::{self, ToHex};
use tw_hash::hasher::Hasher;
use tw_hash::hasher::StatefulHasher;
use tw_hash::hmac::HmacSha512;
use tw_keypair::tw::Curve;
use tw_misc::traits::ToBytesZeroizing;
use zeroize::Zeroize;
use zeroize::Zeroizing;

use crate::crypto_hd_node::error::{Error, Result};
use crate::crypto_hd_node::extended_key::{
    bip32_private_key::BIP32PrivateKey, extended_public_key::ExtendedPublicKey,
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

            let result = (
                K::try_from(&digest[0..96]).map_err(|_| Error::InvalidKeyData)?,
                digest[64..96].try_into()?,
            );

            // Wipe the key parts (index 0..64) of the digest array
            digest[0..64].iter_mut().for_each(|byte| *byte = 0);

            result
        } else {
            let domain_separator = hex::decode(&K::bip32_name().to_hex())?;

            let mut hmac = HmacSha512::new_from_slice(&domain_separator).expect("Should not fail");
            hmac.update(seed.as_ref());

            let result = hmac.finalize().into_bytes();
            let (private_key, chain_code) = result.split_at(KEY_SIZE);
            (
                K::try_from(private_key).map_err(|_| Error::InvalidKeyData)?,
                chain_code.try_into()?,
            )
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
    pub fn derive_from_path(&self, path: &DerivationPath, hasher: Hasher) -> Result<Self> {
        path.iter().try_fold(self.clone(), |key, child_num| {
            key.derive_child(child_num, hasher)
        })
    }

    /// Derive a child key for a particular [`ChildNumber`].
    pub fn derive_child(&self, child_number: ChildNumber, hasher: Hasher) -> Result<Self> {
        let depth = self.attrs.depth.checked_add(1).ok_or(Error::InvalidDepth)?;
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
            parent_fingerprint: self.public_key().fingerprint(hasher),
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

    /// Serialize this key as an [`ExtendedKey`].
    pub fn to_extended_key(&self, prefix: Prefix) -> Result<ExtendedKey> {
        if K::curve() == Curve::Ed25519ExtendedCardano {
            return Err(Error::UnsupportedCurve(K::curve().to_raw()));
        }
        // Add leading `0` byte
        let mut key_bytes = [0u8; KEY_SIZE + 1];
        key_bytes[1..].copy_from_slice(&self.private_key.to_zeroizing_vec());

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

    pub fn from_base58(xprv: &str, hasher: Hasher) -> Result<Self> {
        let extended_key = decode_base58(xprv, hasher)?;
        extended_key.try_into()
    }
}

impl<K> TryFrom<ExtendedKey> for ExtendedPrivateKey<K>
where
    K: BIP32PrivateKey,
{
    type Error = Error;

    fn try_from(key: ExtendedKey) -> Result<Self> {
        let version = HDVersion::from_repr(key.prefix.version()).ok_or(Error::InvalidKeyData)?;
        if version.is_private() && key.key_bytes[0] == 0 {
            Ok(ExtendedPrivateKey {
                private_key: K::try_from(&key.key_bytes[1..]).map_err(|_| Error::InvalidKeyData)?,
                attrs: key.attrs.clone(),
            })
        } else {
            Err(Error::InvalidKeyData)
        }
    }
}

impl<K> ToBytesZeroizing for ExtendedPrivateKey<K>
where
    K: BIP32PrivateKey,
{
    fn to_zeroizing_vec(&self) -> Zeroizing<Vec<u8>> {
        self.private_key.to_zeroizing_vec()
    }
}

/// Size of an extended key when deserialized into bytes from Base58.
const BYTE_SIZE: usize = 78;

/// Size of the checksum in a Base58Check-encoded extended key.
const CHECKSUM_LEN: usize = 4;

/// Maximum size of a Base58Check-encoded extended key in bytes.
///
/// Note that extended keys can also be 111-bytes.
const MAX_BASE58_SIZE: usize = 112;

/// Write a Base58-encoded key to the provided buffer, returning a `String`
/// containing the serialized data.
pub fn encode_base58(extended_key: &ExtendedKey, hasher: Hasher) -> Result<String> {
    let mut buffer = [0u8; MAX_BASE58_SIZE];

    let mut bytes = [0u8; BYTE_SIZE]; // with 4-byte checksum
    bytes[..4].copy_from_slice(&extended_key.prefix.to_bytes());
    bytes[4] = extended_key.attrs.depth;
    bytes[5..9].copy_from_slice(&extended_key.attrs.parent_fingerprint);
    bytes[9..13].copy_from_slice(&extended_key.attrs.child_number.to_bytes());
    bytes[13..45].copy_from_slice(&extended_key.attrs.chain_code);
    bytes[45..78].copy_from_slice(&extended_key.key_bytes);

    let checksum = hasher.hash(&bytes);

    let mut bytes_with_checksum = [0u8; BYTE_SIZE + CHECKSUM_LEN];
    bytes_with_checksum[..BYTE_SIZE].copy_from_slice(&bytes);
    bytes_with_checksum[78..].copy_from_slice(&checksum[..CHECKSUM_LEN]);
    bytes.zeroize();

    let base58_len = bs58::encode(&bytes_with_checksum).onto(buffer.as_mut())?;
    bytes_with_checksum.zeroize();

    String::from_utf8(buffer[..base58_len].to_vec()).map_err(|_| Error::Base58)
}

pub fn decode_base58(base58: &str, hasher: Hasher) -> Result<ExtendedKey> {
    let mut bytes = [0u8; BYTE_SIZE + CHECKSUM_LEN]; // with 4-byte checksum
    let decoded_len = bs58::decode(base58).onto(&mut bytes)?;

    if decoded_len != BYTE_SIZE + CHECKSUM_LEN {
        return Err(Error::Decode);
    }

    let checksum_index = decoded_len - CHECKSUM_LEN;

    let expected_checksum = &bytes[checksum_index..decoded_len];

    let hash = hasher.hash(&bytes[0..checksum_index]);
    let (checksum, _) = hash.split_at(CHECKSUM_LEN);

    if checksum != expected_checksum {
        return Err(Error::InvalidChecksum);
    }

    let prefix = base58.get(..4).ok_or(Error::Decode).and_then(|chars| {
        validate_prefix(chars)?;
        let version = Version::from_be_bytes(bytes[..4].try_into()?);
        Ok(Prefix::from_parts_unchecked(chars, version))
    })?;

    let depth = bytes[4];
    let parent_fingerprint = bytes[5..9].try_into()?;
    let child_number = ChildNumber::from_bytes(bytes[9..13].try_into()?);
    let chain_code = bytes[13..45].try_into()?;
    let key_bytes = bytes[45..78].try_into()?;
    bytes.zeroize();

    let attrs = ExtendedKeyAttrs {
        depth,
        parent_fingerprint,
        child_number,
        chain_code,
    };

    Ok(ExtendedKey {
        prefix,
        attrs,
        key_bytes,
    })
}

const fn validate_prefix(s: &str) -> Result<&str> {
    if s.len() != Prefix::LENGTH {
        return Err(Error::Decode);
    }

    let mut i = 0;

    while i < Prefix::LENGTH {
        if s.as_bytes()[i].is_ascii_alphabetic() {
            i += 1;
        } else {
            return Err(Error::Decode);
        }
    }

    Ok(s)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_validate_prefix() {
        assert!(validate_prefix("eaab").is_ok());
        assert!(validate_prefix("eaab1").is_err());
    }
}
