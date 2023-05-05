// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::ed25519::modifications::cardano::extended_public::{
    ExtendedPublicKey, ExtendedPublicPart,
};
use crate::ed25519::public::PublicKey;
use crate::ed25519::secret::ExpandedSecretKey;
use crate::ed25519::signature::Signature;
use crate::ed25519::Hasher512;
use crate::traits::SigningKeyTrait;
use crate::Error;
use std::marker::PhantomData;
use std::ops::Range;
use tw_encoding::hex;
use tw_hash::H256;
use tw_misc::traits::ToBytesZeroizing;
use zeroize::{ZeroizeOnDrop, Zeroizing};

/// Represents an `ed25519` extended private key that is used in Cardano blockchain.
pub struct ExtendedPrivateKey<H: Hasher512> {
    key: ExtendedSecretPart<H>,
    second_key: ExtendedSecretPart<H>,
}

/// cbindgen:ignore
impl<H: Hasher512> ExtendedPrivateKey<H> {
    /// The number of bytes in a serialized private key (192 bytes).
    const LEN: usize = ExtendedSecretPart::<H>::LEN * 2;
    const KEY_RANGE: Range<usize> = 0..ExtendedSecretPart::<H>::LEN;
    const SECOND_KEY_RANGE: Range<usize> = ExtendedSecretPart::<H>::LEN..Self::LEN;

    /// Returns an associated Cardano extended `ed25519` public key.
    pub fn public(&self) -> ExtendedPublicKey<H> {
        let key_public = PublicKey::with_expanded_secret_no_mangle(&self.key.to_expanded_secret());
        let second_key_public =
            PublicKey::with_expanded_secret_no_mangle(&self.second_key.to_expanded_secret());

        let key = ExtendedPublicPart::new(key_public, self.key.chain_code);
        let second_key = ExtendedPublicPart::new(second_key_public, self.second_key.chain_code);

        ExtendedPublicKey::new(key, second_key)
    }

    pub fn sign_with_public_key(
        &self,
        public: &ExtendedPublicKey<H>,
        message: &[u8],
    ) -> Result<Signature, Error> {
        let expanded = self.key.to_expanded_secret();
        expanded.sign_with_pubkey(public.key_for_signing(), &message)
    }
}

impl<H: Hasher512> SigningKeyTrait for ExtendedPrivateKey<H> {
    type SigningMessage = Vec<u8>;
    type Signature = Signature;

    fn sign(&self, message: Self::SigningMessage) -> Result<Self::Signature, Error> {
        self.sign_with_public_key(&self.public(), message.as_slice())
    }
}

impl<H: Hasher512> ToBytesZeroizing for ExtendedPrivateKey<H> {
    fn to_zeroizing_vec(&self) -> Zeroizing<Vec<u8>> {
        let mut res = self.key.to_zeroizing_vec();
        res.extend_from_slice(self.second_key.to_zeroizing_vec().as_slice());
        res
    }
}

impl<'a, H: Hasher512> TryFrom<&'a [u8]> for ExtendedPrivateKey<H> {
    type Error = Error;

    fn try_from(bytes: &'a [u8]) -> Result<Self, Self::Error> {
        if bytes.len() != Self::LEN {
            return Err(Error::InvalidSecretKey);
        }
        let key = ExtendedSecretPart::try_from(&bytes[Self::KEY_RANGE])?;
        let second_key = ExtendedSecretPart::try_from(&bytes[Self::SECOND_KEY_RANGE])?;

        Ok(ExtendedPrivateKey { key, second_key })
    }
}

/// Implement `str` -> `ExtendedPrivateKey<H>` conversion for test purposes.
impl<H: Hasher512> From<&'static str> for ExtendedPrivateKey<H> {
    fn from(hex: &'static str) -> Self {
        // There is no need to zeroize the `data` as it has a static lifetime (so most likely included in the binary).
        let data = hex::decode(hex).expect("Expected a valid Secret Key hex");
        ExtendedPrivateKey::try_from(data.as_slice()).expect("Expected a valid Secret Key")
    }
}

#[derive(ZeroizeOnDrop)]
struct ExtendedSecretPart<H: Hasher512> {
    secret: H256,
    extension: H256,
    chain_code: H256,
    _phantom: PhantomData<H>,
}

/// cbindgen:ignore
impl<H: Hasher512> ExtendedSecretPart<H> {
    const LEN: usize = 96;
    const SECRET_RANGE: Range<usize> = 0..32;
    const EXTENSION_RANGE: Range<usize> = 32..64;
    const CHAIN_CODE_RANGE: Range<usize> = 64..96;

    fn to_expanded_secret(&self) -> ExpandedSecretKey<H> {
        ExpandedSecretKey::with_extended_secret(self.secret, self.extension)
    }
}

impl<H: Hasher512> ToBytesZeroizing for ExtendedSecretPart<H> {
    fn to_zeroizing_vec(&self) -> Zeroizing<Vec<u8>> {
        let mut res = Vec::with_capacity(H256::len() * 3);
        res.extend_from_slice(self.secret.as_slice());
        res.extend_from_slice(self.extension.as_slice());
        res.extend_from_slice(self.chain_code.as_slice());
        Zeroizing::new(res)
    }
}

impl<'a, H: Hasher512> TryFrom<&'a [u8]> for ExtendedSecretPart<H> {
    type Error = Error;

    fn try_from(bytes: &'a [u8]) -> Result<Self, Self::Error> {
        if bytes.len() != Self::LEN {
            return Err(Error::InvalidSecretKey);
        }
        let secret =
            H256::try_from(&bytes[Self::SECRET_RANGE]).map_err(|_| Error::InvalidSecretKey)?;
        let extension =
            H256::try_from(&bytes[Self::EXTENSION_RANGE]).map_err(|_| Error::InvalidSecretKey)?;
        let chain_code =
            H256::try_from(&bytes[Self::CHAIN_CODE_RANGE]).map_err(|_| Error::InvalidSecretKey)?;

        Ok(ExtendedSecretPart {
            secret,
            extension,
            chain_code,
            _phantom: PhantomData::default(),
        })
    }
}
