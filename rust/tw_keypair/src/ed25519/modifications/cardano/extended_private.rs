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
use crate::ed25519::Hash512;
use crate::traits::SigningKeyTrait;
use crate::Error;
use std::marker::PhantomData;
use std::ops::Range;
use tw_encoding::hex;
use tw_hash::H256;
use tw_misc::traits::ToBytesVec;

pub struct ExtendedPrivateKey<Hash: Hash512> {
    key: ExtendedSecretPart<Hash>,
    second_key: ExtendedSecretPart<Hash>,
}

/// cbindgen:ignore
impl<Hash: Hash512> ExtendedPrivateKey<Hash> {
    const LEN: usize = ExtendedSecretPart::<Hash>::LEN * 2;
    const KEY_RANGE: Range<usize> = 0..ExtendedSecretPart::<Hash>::LEN;
    const SECOND_KEY_RANGE: Range<usize> = ExtendedSecretPart::<Hash>::LEN..Self::LEN;

    pub fn public(&self) -> ExtendedPublicKey<Hash> {
        let key_public = PublicKey::with_expanded_secret(&self.key.to_expanded_secret());
        let second_key_public =
            PublicKey::with_expanded_secret(&self.second_key.to_expanded_secret());

        let key = ExtendedPublicPart::new(key_public, self.key.chain_code);
        let second_key = ExtendedPublicPart::new(second_key_public, self.second_key.chain_code);

        ExtendedPublicKey::new(key, second_key)
    }
}

impl<Hash: Hash512> SigningKeyTrait for ExtendedPrivateKey<Hash> {
    type SigningHash = Vec<u8>;
    type Signature = Signature;

    fn sign(&self, message: Self::SigningHash) -> Result<Self::Signature, Error> {
        let expanded =
            ExpandedSecretKey::<Hash>::with_extended_secret(self.key.secret, self.key.extension);
        expanded.sign(message)
    }
}

impl<Hash: Hash512> ToBytesVec for ExtendedPrivateKey<Hash> {
    fn to_vec(&self) -> Vec<u8> {
        let mut res = self.key.to_vec();
        res.extend_from_slice(self.second_key.to_vec().as_slice());
        res
    }
}

impl<'a, Hash: Hash512> TryFrom<&'a [u8]> for ExtendedPrivateKey<Hash> {
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

impl<Hash: Hash512> From<&'static str> for ExtendedPrivateKey<Hash> {
    fn from(hex: &'static str) -> Self {
        // There is no need to zeroize the `data` as it has a static lifetime (so most likely included in the binary).
        let data = hex::decode(hex).expect("Expected a valid Secret Key hex");
        ExtendedPrivateKey::try_from(data.as_slice()).expect("Expected a valid Secret Key")
    }
}

struct ExtendedSecretPart<Hash: Hash512> {
    secret: H256,
    extension: H256,
    chain_code: H256,
    _phantom: PhantomData<Hash>,
}

/// cbindgen:ignore
impl<Hash: Hash512> ExtendedSecretPart<Hash> {
    const LEN: usize = 96;
    const SECRET_RANGE: Range<usize> = 0..32;
    const EXTENSION_RANGE: Range<usize> = 32..64;
    const CHAIN_CODE_RANGE: Range<usize> = 64..96;

    fn to_expanded_secret(&self) -> ExpandedSecretKey<Hash> {
        ExpandedSecretKey::with_extended_secret(self.secret, self.extension)
    }
}

impl<Hash: Hash512> ToBytesVec for ExtendedSecretPart<Hash> {
    fn to_vec(&self) -> Vec<u8> {
        let mut res = Vec::with_capacity(H256::len() * 3);
        res.extend_from_slice(self.secret.as_slice());
        res.extend_from_slice(self.extension.as_slice());
        res.extend_from_slice(self.chain_code.as_slice());
        res
    }
}

impl<'a, Hash: Hash512> TryFrom<&'a [u8]> for ExtendedSecretPart<Hash> {
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
