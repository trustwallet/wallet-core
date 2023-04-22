// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::traits::VerifyingKeyTrait;
use crate::tw::TWPublicKeyType;
use crate::{secp256k1, starkex, Error};
use tw_memory::ffi::RawPtrTrait;
use tw_utils::traits::ToBytesVec;
use tw_utils::try_or_false;

pub enum TWPublicKey {
    Secp256k1(secp256k1::PublicKey),
    Secp256k1Extended(secp256k1::PublicKey),
    Starkex(starkex::PublicKey),
}

impl RawPtrTrait for TWPublicKey {}

impl TWPublicKey {
    pub fn new(bytes: Vec<u8>, ty: TWPublicKeyType) -> Result<TWPublicKey, Error> {
        match ty {
            TWPublicKeyType::Secp256k1 if secp256k1::PublicKey::COMPRESSED == bytes.len() => {
                let pubkey = secp256k1::PublicKey::try_from(bytes.as_slice())?;
                Ok(TWPublicKey::Secp256k1(pubkey))
            },
            TWPublicKeyType::Secp256k1Extended
                if secp256k1::PublicKey::UNCOMPRESSED == bytes.len() =>
            {
                let pubkey = secp256k1::PublicKey::try_from(bytes.as_slice())?;
                Ok(TWPublicKey::Secp256k1Extended(pubkey))
            },
            TWPublicKeyType::Starkex => {
                let pubkey = starkex::PublicKey::try_from(bytes.as_slice())?;
                Ok(TWPublicKey::Starkex(pubkey))
            },
            _ => Err(Error::InvalidPublicKey),
        }
    }

    pub fn is_valid(bytes: Vec<u8>, ty: TWPublicKeyType) -> bool {
        TWPublicKey::new(bytes, ty).is_ok()
    }

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
            TWPublicKey::Secp256k1(secp) | TWPublicKey::Secp256k1Extended(secp) => {
                verify_impl(secp, sig, hash)
            },
            TWPublicKey::Starkex(stark) => verify_impl(stark, sig, hash),
        }
    }

    pub fn to_bytes(&self) -> Vec<u8> {
        match self {
            TWPublicKey::Secp256k1(secp) => secp.compressed().into_vec(),
            TWPublicKey::Secp256k1Extended(secp) => secp.uncompressed().into_vec(),
            TWPublicKey::Starkex(stark) => stark.to_vec(),
        }
    }
}
