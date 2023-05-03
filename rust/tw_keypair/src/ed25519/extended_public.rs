// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::ed25519::public::PublicKey;
use crate::ed25519::signature::Signature;
use crate::ed25519::Hash512;
use crate::traits::VerifyingKeyTrait;
use crate::Error;
use std::ops::Range;
use tw_encoding::hex;
use tw_hash::H256;
use tw_misc::traits::ToBytesVec;

pub struct ExtendedPublicKey<Hash: Hash512> {
    key: ExtendedPublicPart<Hash>,
    second_key: ExtendedPublicPart<Hash>,
}

/// cbindgen:ignore
impl<Hash: Hash512> ExtendedPublicKey<Hash> {
    pub const LEN: usize = ExtendedPublicPart::<Hash>::LEN * 2;
    const KEY_RANGE: Range<usize> = 0..ExtendedPublicPart::<Hash>::LEN;
    const SECOND_KEY_RANGE: Range<usize> = ExtendedPublicPart::<Hash>::LEN..Self::LEN;

    pub(crate) fn new(key: ExtendedPublicPart<Hash>, second_key: ExtendedPublicPart<Hash>) -> Self {
        ExtendedPublicKey { key, second_key }
    }
}

impl<Hash: Hash512> VerifyingKeyTrait for ExtendedPublicKey<Hash> {
    type SigningHash = Vec<u8>;
    type VerifySignature = Signature;

    fn verify(&self, signature: Self::VerifySignature, message: Self::SigningHash) -> bool {
        self.key.public.verify(signature, message)
    }
}

impl<Hash: Hash512> ToBytesVec for ExtendedPublicKey<Hash> {
    fn to_vec(&self) -> Vec<u8> {
        let mut res = self.key.to_vec();
        res.extend_from_slice(self.second_key.to_vec().as_slice());
        res
    }
}

impl<'a, Hash: Hash512> TryFrom<&'a [u8]> for ExtendedPublicKey<Hash> {
    type Error = Error;

    fn try_from(bytes: &'a [u8]) -> Result<Self, Self::Error> {
        if bytes.len() != Self::LEN {
            return Err(Error::InvalidPublicKey);
        }

        let key = ExtendedPublicPart::try_from(&bytes[Self::KEY_RANGE])?;
        let second_key = ExtendedPublicPart::try_from(&bytes[Self::SECOND_KEY_RANGE])?;

        Ok(ExtendedPublicKey { key, second_key })
    }
}

impl<Hash: Hash512> From<&'static str> for ExtendedPublicKey<Hash> {
    fn from(hex: &'static str) -> Self {
        let bytes = hex::decode(hex).expect("Expected a valid Public Key hex");
        ExtendedPublicKey::try_from(bytes.as_slice()).expect("Expected a valid Public Key")
    }
}

pub(crate) struct ExtendedPublicPart<Hash: Hash512> {
    public: PublicKey<Hash>,
    chain_code: H256,
}

/// cbindgen:ignore
impl<Hash: Hash512> ExtendedPublicPart<Hash> {
    const LEN: usize = PublicKey::<Hash>::LEN + H256::len();
    const PUBLIC_RANGE: Range<usize> = 0..32;
    const CHAIN_CODE_RANGE: Range<usize> = 32..64;

    pub(crate) fn new(public: PublicKey<Hash>, chain_code: H256) -> ExtendedPublicPart<Hash> {
        ExtendedPublicPart { public, chain_code }
    }
}

impl<'a, Hash: Hash512> TryFrom<&'a [u8]> for ExtendedPublicPart<Hash> {
    type Error = Error;

    fn try_from(bytes: &'a [u8]) -> Result<Self, Self::Error> {
        if bytes.len() != Self::LEN {
            return Err(Error::InvalidPublicKey);
        }

        let public = PublicKey::try_from(&bytes[Self::PUBLIC_RANGE])?;
        let chain_code =
            H256::try_from(&bytes[Self::CHAIN_CODE_RANGE]).map_err(|_| Error::InvalidPublicKey)?;

        Ok(ExtendedPublicPart { public, chain_code })
    }
}

impl<Hash: Hash512> ToBytesVec for ExtendedPublicPart<Hash> {
    fn to_vec(&self) -> Vec<u8> {
        let mut res = Vec::with_capacity(H256::len() * 2);
        res.extend_from_slice(self.public.as_slice());
        res.extend_from_slice(self.chain_code.as_slice());
        res
    }
}
