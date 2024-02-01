// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_cosmos_sdk::signature::CosmosSignature;
use tw_evm::message::signature::{MessageSignature, SignatureType};
use tw_keypair::ecdsa::secp256k1;
use tw_keypair::KeyPairError;
use tw_misc::traits::ToBytesVec;

/// secp256k1 signature with the legacy ETH replay protection.
#[derive(Clone)]
pub struct GreenfieldSignature(MessageSignature);

impl TryFrom<secp256k1::Signature> for GreenfieldSignature {
    type Error = KeyPairError;

    fn try_from(sign: secp256k1::Signature) -> Result<Self, Self::Error> {
        MessageSignature::prepared(sign, SignatureType::Legacy).map(GreenfieldSignature)
    }
}

impl CosmosSignature for GreenfieldSignature {}

/// [`GreenfieldSignature::try_from`] tries to parse a standard secp256k1 signature from the given bytes,
/// and applies the legacy ETH replay protection.
impl<'a> TryFrom<&'a [u8]> for GreenfieldSignature {
    type Error = KeyPairError;

    fn try_from(bytes: &'a [u8]) -> Result<Self, Self::Error> {
        let standard_sign = secp256k1::Signature::try_from(bytes)?;
        GreenfieldSignature::try_from(standard_sign)
    }
}

impl ToBytesVec for GreenfieldSignature {
    fn to_vec(&self) -> Vec<u8> {
        self.0.to_bytes().to_vec()
    }
}
