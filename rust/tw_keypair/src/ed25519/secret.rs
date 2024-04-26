// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::ed25519::mangle::mangle_scalar;
use crate::ed25519::signature::Signature;
use crate::ed25519::Hasher512;
use crate::KeyPairResult;
use curve25519_dalek::constants;
use curve25519_dalek::scalar::Scalar;
use std::marker::PhantomData;
use std::ops::DerefMut;
use tw_hash::{H256, H512};
use zeroize::ZeroizeOnDrop;

/// Represents an "expanded" secret key produced by using a hash function
/// with 512-bits output to digest a `PrivateKey`.
///
/// Represents [ed25519_extsk](https://github.com/trustwallet/wallet-core/blob/423f0e34725f69c0a9d535e1a32534c99682edea/trezor-crypto/crypto/ed25519-donna/ed25519.c#L23-L32).
#[derive(ZeroizeOnDrop)]
pub(crate) struct ExpandedSecretKey<H: Hasher512> {
    /// 32 byte scalar. Represents `extsk[0..32]`.
    pub key: Scalar,
    /// 32 byte nonce. Represents `extsk[32..64]`.
    pub nonce: H256,
    _phantom: PhantomData<H>,
}

impl<H: Hasher512> ExpandedSecretKey<H> {
    /// Source: https://github.com/dalek-cryptography/ed25519-dalek/blob/1.0.1/src/secret.rs#L246-L282
    /// Ported: https://github.com/trustwallet/wallet-core/blob/423f0e34725f69c0a9d535e1a32534c99682edea/trezor-crypto/crypto/ed25519-donna/ed25519.c#L23-L32
    pub(crate) fn with_secret(secret: H256) -> Self {
        let mut h = H::new();
        let mut hash = H512::default();

        h.update(secret.as_slice());
        hash.copy_from_slice(h.finalize().as_slice());

        let (mut lower, upper): (H256, H256) = hash.split();
        mangle_scalar(lower.deref_mut());

        #[allow(deprecated)]
        let key = Scalar::from_bits(lower.take());
        ExpandedSecretKey {
            key,
            nonce: upper,
            _phantom: PhantomData,
        }
    }

    /// Ported: https://github.com/trustwallet/wallet-core/blob/423f0e34725f69c0a9d535e1a32534c99682edea/trezor-crypto/crypto/ed25519-donna/ed25519.c#L93-L94C30
    ///
    /// Here we use `Scalar::from_bytes_mod_order` instead of `Scalar::from_bits`
    /// because `Scalar::from_bits` modifies the last 32th byte in some cases.
    /// Although, `Scalar::from_bytes_mod_order` changes the given `secret` significantly,
    /// but the result signature seems to be correct.
    /// Unfortunately, there are no public functions to create `Scalar` without mangling the secret.
    ///
    /// TODO make sure if this is the right way to create an extended secret key (extsk).
    pub(crate) fn with_extended_secret(secret: H256, extension: H256) -> Self {
        let key = Scalar::from_bytes_mod_order(secret.take());
        ExpandedSecretKey {
            key,
            nonce: extension,
            _phantom: PhantomData,
        }
    }

    /// Signs a message with this `ExpandedSecretKey`.
    /// Source: https://github.com/dalek-cryptography/ed25519-dalek/blob/1.0.1/src/secret.rs#L389-L412
    /// Ported: https://github.com/trustwallet/wallet-core/blob/423f0e34725f69c0a9d535e1a32534c99682edea/trezor-crypto/crypto/ed25519-donna/ed25519.c#L97-L130
    #[allow(non_snake_case)]
    pub(crate) fn sign_with_pubkey(
        &self,
        pubkey: H256,
        message: &[u8],
    ) -> KeyPairResult<Signature> {
        let mut h = H::new();

        h.update(self.nonce.as_slice());
        h.update(message);

        let r = Scalar::from_hash(h);
        let R = (&r * constants::ED25519_BASEPOINT_TABLE).compress();

        h = H::new();
        h.update(R.as_bytes());
        h.update(pubkey);
        h.update(message);

        let k = Scalar::from_hash(h);
        let s = k * self.key + r;

        Ok(Signature { R, s })
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use sha2::Sha512;
    use tw_encoding::hex;

    #[test]
    fn test_ed25519_expanded_secret_from_extended_key() {
        let secret = H256::from("b52a1a9f4ae3bff2d16a06e144bcdd4147a35aa9843cfd0edf458afdf5ba1a3b");
        let nonce = H256::from("b33b86344897745b35bb3ef8ca8fe8a3758bd31a537280a6b8c60e42a1f3a00d");

        let secret_key: ExpandedSecretKey<Sha512> =
            ExpandedSecretKey::with_extended_secret(secret, nonce);

        // In `trezor-crypto` implementation, `a = mod(secret)` has the following:
        // https://github.com/trustwallet/wallet-core/blob/423f0e34725f69c0a9d535e1a32534c99682edea/trezor-crypto/crypto/ed25519-donna/ed25519.c#L109-L111
        let expected_mod_a =
            H256::from("eeae3808eee7222aee44f9013eaa33100347a31aa512f234eff05d24627fbd2e");
        // On the other hand, [`ExpandedSecretKey::key`] represents the same `a` value.
        // But it differs from that - `eeae3888fbb988ea4e941ff8a8ce400347a35aa9843cfd0edf458afdf5ba1a0b`.
        // TODO probably, `secret_key.key.to_bytes()` should be the same as `expected_mod_a.take()`.
        assert_ne!(secret_key.key.to_bytes(), expected_mod_a.take());

        let public = H256::from("7950119e049a53a9eaa6ecfbfe354337287056ba0ea054130c1b0c97f1b69697");
        let message = hex::decode("f0").unwrap();

        // Anyway, the result signature has an expected value.
        let sign = secret_key.sign_with_pubkey(public, &message).unwrap();
        let expected = H512::from("ed55bce14a845a275e7a3a7242420ed1eeaba79dc3141bebf42ca0d12169e209a6e56b6981a336f711ae3aaea8d063b72b0e79a8808311d08cb42cabfdd0450d");
        assert_eq!(sign.to_bytes(), expected);
    }
}
