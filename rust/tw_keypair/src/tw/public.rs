// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::traits::VerifyingKeyTrait;
use crate::tw::PublicKeyType;
use crate::{secp256k1, starkex, Error};
use tw_misc::traits::ToBytesVec;
use tw_misc::try_or_false;

/// Represents a public key that can be used to verify signatures and messages.
pub enum PublicKey {
    Secp256k1(secp256k1::PublicKey),
    Secp256k1Extended(secp256k1::PublicKey),
    Starkex(starkex::PublicKey),
}

impl PublicKey {
    /// Validates the given `bytes` using the `ty` public key type and creates a public key from it.
    pub fn new(bytes: Vec<u8>, ty: PublicKeyType) -> Result<PublicKey, Error> {
        match ty {
            PublicKeyType::Secp256k1 if secp256k1::PublicKey::COMPRESSED == bytes.len() => {
                let pubkey = secp256k1::PublicKey::try_from(bytes.as_slice())?;
                Ok(PublicKey::Secp256k1(pubkey))
            },
            PublicKeyType::Secp256k1Extended
                if secp256k1::PublicKey::UNCOMPRESSED == bytes.len() =>
            {
                let pubkey = secp256k1::PublicKey::try_from(bytes.as_slice())?;
                Ok(PublicKey::Secp256k1Extended(pubkey))
            },
            PublicKeyType::Starkex => {
                let pubkey = starkex::PublicKey::try_from(bytes.as_slice())?;
                Ok(PublicKey::Starkex(pubkey))
            },
            _ => Err(Error::InvalidPublicKey),
        }
    }

    /// Checks if the given `bytes` is valid using `ty` public key type.
    pub fn is_valid(bytes: Vec<u8>, ty: PublicKeyType) -> bool {
        PublicKey::new(bytes, ty).is_ok()
    }

    /// Verifies if the given `hash` was signed using a private key associated with the public key.
    pub fn verify(&self, sig: &[u8], hash: &[u8]) -> bool {
        fn verify_impl<Key>(verifying_key: &Key, sig: &[u8], hash: &[u8]) -> bool
        where
            Key: VerifyingKeyTrait,
        {
            let verify_sig =
                try_or_false!(<Key as VerifyingKeyTrait>::VerifySignature::try_from(sig));
            let hash = try_or_false!(<Key as VerifyingKeyTrait>::SigningHash::try_from(hash));
            verifying_key.verify(verify_sig, hash)
        }

        match self {
            PublicKey::Secp256k1(secp) | PublicKey::Secp256k1Extended(secp) => {
                verify_impl(secp, sig, hash)
            },
            PublicKey::Starkex(stark) => verify_impl(stark, sig, hash),
        }
    }

    /// Returns the raw data of the public key.
    pub fn to_bytes(&self) -> Vec<u8> {
        match self {
            PublicKey::Secp256k1(secp) => secp.compressed().into_vec(),
            PublicKey::Secp256k1Extended(secp) => secp.uncompressed().into_vec(),
            PublicKey::Starkex(stark) => stark.to_vec(),
        }
    }
}
