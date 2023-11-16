// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::signature::CosmosSignature;
use tw_hash::{H512, H520};
use tw_keypair::{KeyPairError, KeyPairResult};
use tw_memory::Data;
use tw_misc::traits::ToBytesVec;

pub struct Secp256k1Signature {
    signature: H512,
}

impl CosmosSignature for Secp256k1Signature {
    fn from_bytes(signature_bytes: &[u8]) -> KeyPairResult<Self> {
        let signature_slice = if signature_bytes.len() == H520::len() {
            // Discard the last `v` recovery byte.
            &signature_bytes[0..H512::len()]
        } else {
            signature_bytes
        };

        let signature =
            H512::try_from(signature_slice).map_err(|_| KeyPairError::InvalidSignature)?;
        Ok(Secp256k1Signature { signature })
    }

    fn to_bytes(&self) -> Data {
        self.signature.to_vec()
    }
}
