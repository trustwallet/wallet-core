// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::ed25519::modifications::cardano::extended_public::{
    ExtendedPublicKey, ExtendedPublicPart,
};
use crate::ed25519::public::PublicKey;
use crate::ed25519::secret::ExpandedSecretKey;
use crate::ed25519::signature::Signature;
use crate::ed25519::Hasher512;
use crate::traits::SigningKeyTrait;
use crate::{KeyPairError, KeyPairResult};
use std::ops::Range;
use tw_encoding::hex;
use tw_hash::H256;
use tw_misc::traits::ToBytesZeroizing;
use zeroize::{ZeroizeOnDrop, Zeroizing};

/// Represents an `ed25519` extended private key that is used in Cardano blockchain.
pub struct ExtendedPrivateKey<H: Hasher512> {
    /// The first half (96 bytes) of the extended secret.
    key: ExtendedSecretPart<H>,
    /// The second half (96 bytes) of the extended secret.
    second_key: ExtendedSecretPart<H>,
}

/// cbindgen:ignore
impl<H: Hasher512> ExtendedPrivateKey<H> {
    /// The number of bytes in a serialized private key (192 bytes).
    pub const LEN: usize = ExtendedSecretPart::<H>::LEN * 2;
    const KEY_RANGE: Range<usize> = 0..ExtendedSecretPart::<H>::LEN;
    const SECOND_KEY_RANGE: Range<usize> = ExtendedSecretPart::<H>::LEN..Self::LEN;

    /// Returns an associated Cardano extended `ed25519` public key.
    pub fn public(&self) -> ExtendedPublicKey<H> {
        let key_public = PublicKey::with_expanded_secret_no_mangle(&self.key.expanded_key);
        let second_key_public =
            PublicKey::with_expanded_secret_no_mangle(&self.second_key.expanded_key);

        let key = ExtendedPublicPart::new(key_public, self.key.chain_code);
        let second_key = ExtendedPublicPart::new(second_key_public, self.second_key.chain_code);

        ExtendedPublicKey::new(key, second_key)
    }

    /// `ed25519` signing uses a public key associated with the private key.
    pub(crate) fn sign_with_public_key(
        &self,
        public: &ExtendedPublicKey<H>,
        message: &[u8],
    ) -> KeyPairResult<Signature> {
        self.key
            .expanded_key
            .sign_with_pubkey(public.key_for_signing(), message)
    }
}

impl<H: Hasher512> SigningKeyTrait for ExtendedPrivateKey<H> {
    type SigningMessage = Vec<u8>;
    type Signature = Signature;

    fn sign(&self, message: Self::SigningMessage) -> KeyPairResult<Self::Signature> {
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
    type Error = KeyPairError;

    fn try_from(bytes: &'a [u8]) -> Result<Self, Self::Error> {
        if bytes.len() != Self::LEN {
            return Err(KeyPairError::InvalidSecretKey);
        }
        let key = ExtendedSecretPart::try_from(&bytes[Self::KEY_RANGE])?;
        let second_key = ExtendedSecretPart::try_from(&bytes[Self::SECOND_KEY_RANGE])?;

        Ok(ExtendedPrivateKey { key, second_key })
    }
}

impl<'a, H: Hasher512> TryFrom<&'a str> for ExtendedPrivateKey<H> {
    type Error = KeyPairError;

    fn try_from(hex: &'a str) -> Result<Self, Self::Error> {
        let bytes = Zeroizing::new(hex::decode(hex).map_err(|_| KeyPairError::InvalidSecretKey)?);
        Self::try_from(bytes.as_slice())
    }
}

#[derive(ZeroizeOnDrop)]
struct ExtendedSecretPart<H: Hasher512> {
    secret: H256,
    extension: H256,
    chain_code: H256,
    /// An expanded secret key obtained from [`ExtendedSecretPart::secret`]
    /// and [`ExtendedSecretPart::extension`].
    /// It's used to generate a public key and sign messages.
    expanded_key: ExpandedSecretKey<H>,
}

/// cbindgen:ignore
impl<H: Hasher512> ExtendedSecretPart<H> {
    const LEN: usize = 96;
    const SECRET_RANGE: Range<usize> = 0..32;
    const EXTENSION_RANGE: Range<usize> = 32..64;
    const CHAIN_CODE_RANGE: Range<usize> = 64..96;
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
    type Error = KeyPairError;

    fn try_from(bytes: &'a [u8]) -> Result<Self, Self::Error> {
        if bytes.len() != Self::LEN {
            return Err(KeyPairError::InvalidSecretKey);
        }
        let secret = H256::try_from(&bytes[Self::SECRET_RANGE])
            .map_err(|_| KeyPairError::InvalidSecretKey)?;
        let extension = H256::try_from(&bytes[Self::EXTENSION_RANGE])
            .map_err(|_| KeyPairError::InvalidSecretKey)?;
        let chain_code = H256::try_from(&bytes[Self::CHAIN_CODE_RANGE])
            .map_err(|_| KeyPairError::InvalidSecretKey)?;

        let expanded_key = ExpandedSecretKey::with_extended_secret(secret, extension);
        Ok(ExtendedSecretPart {
            secret,
            extension,
            chain_code,
            expanded_key,
        })
    }
}
