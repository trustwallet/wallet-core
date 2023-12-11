// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_hash::{concat, H512, H520};
use tw_keypair::ecdsa::secp256k1;
use tw_keypair::KeyPairError;
use tw_memory::Data;
use tw_misc::traits::ToBytesVec;

pub struct BinanceSignature {
    signature: H512,
}

impl BinanceSignature {
    pub fn to_bytes(&self) -> Data {
        self.signature.to_vec()
    }
}

impl From<secp256k1::Signature> for BinanceSignature {
    fn from(sign: secp256k1::Signature) -> Self {
        BinanceSignature {
            signature: concat(sign.r(), sign.s()),
        }
    }
}

impl<'a> TryFrom<&'a [u8]> for BinanceSignature {
    type Error = KeyPairError;

    fn try_from(signature_bytes: &'a [u8]) -> Result<Self, Self::Error> {
        let signature_slice = if signature_bytes.len() == H520::len() {
            // Discard the last `v` recovery byte.
            &signature_bytes[0..H512::len()]
        } else {
            signature_bytes
        };

        let signature =
            H512::try_from(signature_slice).map_err(|_| KeyPairError::InvalidSignature)?;
        Ok(BinanceSignature { signature })
    }
}
