// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

// Taken from https://github.com/typed-io/rust-ed25519-bip32/blob/master/src/key.rs

use std::convert::{TryFrom, TryInto};
use std::hash::{Hash, Hasher};

use cryptoxide::ed25519;

use super::derivation::{self, DerivationError, DerivationIndex, DerivationScheme};
use zeroize::ZeroizeOnDrop;

/// Extended Private key size in bytes
pub const XPRV_SIZE: usize = 96;
pub const EXTENDED_SECRET_KEY_SIZE: usize = 64;

/// Extended Public key size in bytes
pub const XPUB_SIZE: usize = 64;
pub const PUBLIC_KEY_SIZE: usize = 32;
pub const CHAIN_CODE_SIZE: usize = 32;

/// Possible errors during conversion from bytes
///
/// HighestBitsInvalid and LowestBitsInvalid are errors
/// reported linked to the shape of a normal extended ed25519 key.
///
#[derive(Debug, PartialEq, Eq)]
pub enum PrivateKeyError {
    HighestBitsInvalid,
    LowestBitsInvalid,
}

/// HDWallet extended private key
///
/// Effectively this is an ed25519 extended secret key (64 bytes) followed by a chain code (32 bytes).
///
#[derive(ZeroizeOnDrop)]
pub struct XPrv([u8; XPRV_SIZE]);
impl XPrv {
    // Create a XPrv from the given bytes.
    //
    // This function does not perform any validity check and should not be used outside
    // of this crate.
    pub(crate) fn from_bytes(bytes: [u8; XPRV_SIZE]) -> Self {
        XPrv(bytes)
    }

    /// Create a `XPrv` by taking ownership of the given array
    ///
    /// This function may returns an error if it does not have the expected
    /// format.
    ///
    /// This function allow the 3rd highest bit to not be clear (to handle potential derived valid xprv),
    /// but self.is_3rd_highest_bit_clear() can be called to check if the 3rd highest bit
    /// is assumed to be clear or not.
    pub fn from_bytes_verified(bytes: [u8; XPRV_SIZE]) -> Result<Self, PrivateKeyError> {
        let scalar = &bytes[0..32];
        let last = scalar[31];
        let first = scalar[0];

        if (last & 0b1100_0000) != 0b0100_0000 {
            return Err(PrivateKeyError::HighestBitsInvalid);
        }
        if (first & 0b0000_0111) != 0b0000_0000 {
            return Err(PrivateKeyError::LowestBitsInvalid);
        }

        Ok(XPrv(bytes))
    }

    pub fn derive(&self, scheme: DerivationScheme, index: DerivationIndex) -> Self {
        derivation::private(self, index, scheme)
    }

    pub fn extended_secret_key_bytes(&self) -> &[u8; 64] {
        self.0[0..EXTENDED_SECRET_KEY_SIZE].try_into().unwrap()
    }
}

impl AsRef<[u8]> for XPrv {
    fn as_ref(&self) -> &[u8] {
        &self.0
    }
}

/// Extended Public Key (Point + ChainCode)
#[derive(Clone, Copy)]
pub struct XPub([u8; XPUB_SIZE]);
impl XPub {
    /// create a `XPub` by taking ownership of the given array
    pub fn from_bytes(bytes: [u8; XPUB_SIZE]) -> Self {
        XPub(bytes)
    }

    pub fn derive(
        &self,
        scheme: DerivationScheme,
        index: DerivationIndex,
    ) -> Result<Self, DerivationError> {
        derivation::public(self, index, scheme)
    }

    pub fn get_without_chaincode(&self, out: &mut [u8; 32]) {
        out.clone_from_slice(&self.0[0..32])
    }

    pub fn public_key_bytes(&self) -> &[u8; 32] {
        <&[u8; 32]>::try_from(&self.0[0..32]).unwrap()
    }

    pub fn public_key_slice(&self) -> &[u8] {
        &self.0[0..32]
    }

    pub fn chain_code_slice(&self) -> &[u8] {
        &self.0[32..64]
    }

    pub fn public_key(&self) -> [u8; PUBLIC_KEY_SIZE] {
        let mut buf = [0u8; PUBLIC_KEY_SIZE];
        buf.copy_from_slice(self.public_key_slice());
        buf
    }

    pub fn chain_code(&self) -> &[u8; CHAIN_CODE_SIZE] {
        (&self.0[32..64]).try_into().unwrap()
    }
}

impl Hash for XPub {
    fn hash<H: Hasher>(&self, state: &mut H) {
        state.write(&self.0)
    }
}

impl AsRef<[u8]> for XPub {
    fn as_ref(&self) -> &[u8] {
        &self.0
    }
}
impl From<XPub> for [u8; XPUB_SIZE] {
    fn from(v: XPub) -> [u8; XPUB_SIZE] {
        v.0
    }
}

pub(crate) fn mk_xprv(out: &mut [u8; XPRV_SIZE], kl: &[u8], kr: &[u8], cc: &[u8]) {
    assert!(kl.len() == 32);
    assert!(kr.len() == 32);
    assert!(cc.len() == CHAIN_CODE_SIZE);

    out[0..32].clone_from_slice(kl);
    out[32..64].clone_from_slice(kr);
    out[64..96].clone_from_slice(cc);
}

pub(crate) fn mk_xpub(out: &mut [u8; XPUB_SIZE], pk: &[u8], cc: &[u8]) {
    assert!(pk.len() == 32);
    assert!(cc.len() == CHAIN_CODE_SIZE);

    out[0..32].clone_from_slice(pk);
    out[32..64].clone_from_slice(cc);
}

pub fn mk_public_key(extended_secret: &[u8; 64]) -> [u8; PUBLIC_KEY_SIZE] {
    ed25519::extended_to_public(extended_secret)
}
