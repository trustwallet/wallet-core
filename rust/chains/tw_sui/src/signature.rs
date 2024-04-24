// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_encoding::base64;
use tw_hash::{H256, H512};
use tw_keypair::ed25519;
use tw_memory::Data;

#[derive(Clone, Copy)]
#[repr(u8)]
pub enum SignatureScheme {
    ED25519 = 0,
}

pub struct SuiSignatureInfo {
    scheme: SignatureScheme,
    signature: H512,
    public_key: H256,
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

    pub fn to_vec(&self) -> Data {
        let mut scheme: Data = Vec::with_capacity(H512::LEN + H256::LEN + 1);
        scheme.push(self.scheme as u8);
        scheme.extend_from_slice(self.signature.as_slice());
        scheme.extend_from_slice(self.public_key.as_slice());
        scheme
    }

    pub fn to_base64(&self) -> String {
        let is_url = false;
        base64::encode(&self.to_vec(), is_url)
    }
}
