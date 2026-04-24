// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::KeyPairError;
use curve25519_dalek::MontgomeryPoint;

const PUBLIC_KEY_LEN: usize = 32;
/// Use an arbitrary sign bit 0 – group order is unaffected by the sign.
/// Please note it's only used to check if the public key is of small order,
/// and not to determine the sign of the point.
const EDWARDS_POINT_SIGN: u8 = 0;

pub struct PublicKey {
    public: crypto_box::PublicKey,
}

impl PublicKey {
    pub(crate) fn with_inner(public: crypto_box::PublicKey) -> PublicKey {
        PublicKey { public }
    }

    pub(crate) fn inner(&self) -> &crypto_box::PublicKey {
        &self.public
    }

    pub fn as_slice(&self) -> &[u8] {
        self.public.as_bytes()
    }
}

impl<'a> TryFrom<&'a [u8]> for PublicKey {
    type Error = KeyPairError;

    fn try_from(value: &'a [u8]) -> Result<Self, Self::Error> {
        // Reject low-order X25519 public keys to prevent small-subgroup attacks.
        // Convert the Montgomery-encoded key to an EdwardsPoint and use
        // curve25519_dalek's built-in is_small_order() check.
        let arr: [u8; PUBLIC_KEY_LEN] = value
            .try_into()
            .map_err(|_| KeyPairError::InvalidPublicKey)?;
        let mont = MontgomeryPoint(arr);

        // to_edwards() returns None for the Montgomery point-at-infinity and
        // for u-coordinates that have no Edwards image; both warrant rejection.
        let edwards = mont
            .to_edwards(EDWARDS_POINT_SIGN)
            .ok_or(KeyPairError::InvalidPublicKey)?;
        if edwards.is_small_order() {
            return Err(KeyPairError::InvalidPublicKey);
        }

        let public = crypto_box::PublicKey::from(mont);
        Ok(PublicKey { public })
    }
}
