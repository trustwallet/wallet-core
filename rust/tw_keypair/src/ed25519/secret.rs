// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::ed25519::public::PublicKey;
use crate::ed25519::signature::Signature;
use crate::ed25519::Hash512;
use crate::traits::SigningKeyTrait;
use crate::Error;
use curve25519_dalek::constants;
use curve25519_dalek::scalar::Scalar;
use std::marker::PhantomData;
use tw_hash::{H256, H512};

pub(crate) struct ExpandedSecretKey<Hash: Hash512> {
    pub key: Scalar,
    pub nonce: H256,
    _phantom: PhantomData<Hash>,
}

impl<Hash: Hash512> ExpandedSecretKey<Hash> {
    pub(crate) fn with_secret(secret: H256) -> Self {
        let mut h = Hash::new();
        let mut hash = H512::default();

        h.update(secret.as_slice());
        hash.copy_from_slice(h.finalize().as_slice());

        let (mut lower, upper): (H256, H256) = hash.split();

        lower[0] &= 248;
        lower[31] &= 63;
        lower[31] |= 64;

        ExpandedSecretKey {
            key: Scalar::from_bits(lower.take()),
            nonce: upper,
            _phantom: PhantomData::default(),
        }
    }

    pub(crate) fn with_extended_secret(secret: H256, extension: H256) -> Self {
        ExpandedSecretKey {
            key: Scalar::from_bits(secret.take()),
            nonce: extension,
            _phantom: PhantomData::default(),
        }
    }
}

impl<Hash: Hash512> SigningKeyTrait for ExpandedSecretKey<Hash> {
    type SigningHash = Vec<u8>;
    type Signature = Signature;

    #[allow(non_snake_case)]
    fn sign(&self, hash: Self::SigningHash) -> Result<Self::Signature, Error> {
        let public_key = PublicKey::with_expanded_secret(self);

        let mut h = Hash::new();

        h.update(self.nonce.as_slice());
        h.update(hash.as_slice());

        let r = Scalar::from_hash(h);
        let R = (&r * &constants::ED25519_BASEPOINT_TABLE).compress();

        h = Hash::new();
        h.update(R.as_bytes());
        h.update(public_key.as_slice());
        h.update(hash.as_slice());

        let k = Scalar::from_hash(h);
        let s = k * self.key + r;

        Ok(Signature { R, s })
    }
}
