// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::error::prelude::*;
use tw_encoding::base64::{self, STANDARD};
use tw_hash::{H256, H512};
use tw_keypair::{ed25519, KeyPairError};
use tw_memory::Data;

#[derive(Clone, Copy)]
#[repr(u8)]
pub enum SignatureScheme {
    ED25519 = 0,
}

pub struct SuiSignatureInfo {
    scheme: SignatureScheme,
    pub signature: H512,
    pub public_key: H256,
}

impl SuiSignatureInfo {
    pub fn ed25519(
        signature: &ed25519::Signature,
        public_key: &ed25519::sha512::PublicKey,
    ) -> SuiSignatureInfo {
        SuiSignatureInfo {
            scheme: SignatureScheme::ED25519,
            signature: signature.to_bytes(),
            public_key: public_key.to_bytes(),
        }
    }

    pub fn from_base64(encoded: &str) -> SigningResult<SuiSignatureInfo> {
        let data = base64::decode(encoded, STANDARD)?;
        if data.len() != H512::LEN + H256::LEN + 1 {
            return SigningError::err(tw_proto::Common::Proto::SigningError::Error_invalid_params)
                .context("Invalid signature length, expected exactly 97 bytes");
        }
        let scheme = match data[0] {
            0 => SignatureScheme::ED25519,
            _ => {
                return SigningError::err(
                    tw_proto::Common::Proto::SigningError::Error_not_supported,
                )
                .context("Unsupported signature scheme")
            },
        };
        let signature =
            H512::try_from(&data[1..H512::LEN + 1]).map_err(|_| KeyPairError::InvalidSignature)?;
        let public_key =
            H256::try_from(&data[H512::LEN + 1..]).map_err(|_| KeyPairError::InvalidPublicKey)?;

        Ok(SuiSignatureInfo {
            scheme,
            signature,
            public_key,
        })
    }

    pub fn to_vec(&self) -> Data {
        let mut scheme: Data = Vec::with_capacity(H512::LEN + H256::LEN + 1);
        scheme.push(self.scheme as u8);
        scheme.extend_from_slice(self.signature.as_slice());
        scheme.extend_from_slice(self.public_key.as_slice());
        scheme
    }

    pub fn to_base64(&self) -> String {
        base64::encode(&self.to_vec(), STANDARD)
    }
}
