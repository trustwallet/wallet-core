// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::signature::{
    eip155_replay_protection, legacy_replay_protection, remove_replay_protection,
};
use std::str::FromStr;
use tw_encoding::hex::DecodeHex;
use tw_hash::{H256, H520};
use tw_keypair::ecdsa::secp256k1;
use tw_keypair::{KeyPairError, KeyPairResult};
use tw_number::U256;

pub enum SignatureType {
    Standard,
    Legacy,
    Eip155 { chain_id: U256 },
}

#[derive(Clone)]
pub struct MessageSignature {
    r: H256,
    s: H256,
    v: u8,
}

impl MessageSignature {
    /// The length of the binary representation.
    /// cbindgen:ignore
    pub const LEN: usize = secp256k1::Signature::LEN;

    pub fn prepared(sign: secp256k1::Signature, sign_type: SignatureType) -> KeyPairResult<Self> {
        let v = match sign_type {
            SignatureType::Standard => U256::from(sign.v()),
            SignatureType::Legacy => {
                legacy_replay_protection(sign.v()).map_err(|_| KeyPairError::InvalidSignature)?
            },
            SignatureType::Eip155 { chain_id } => eip155_replay_protection(chain_id, sign.v())
                .map_err(|_| KeyPairError::InvalidSignature)?,
        };
        Ok(MessageSignature {
            r: sign.r(),
            s: sign.s(),
            v: v.low_u8(),
        })
    }

    pub fn to_bytes(&self) -> H520 {
        let mut bytes = Vec::with_capacity(H520::LEN);
        bytes.extend_from_slice(self.r.as_slice());
        bytes.extend_from_slice(self.s.as_slice());
        bytes.push(self.v);

        H520::try_from(bytes.as_slice()).expect("'bytes' is 130 byte array")
    }

    pub fn to_secp256k1_signature(&self) -> KeyPairResult<secp256k1::Signature> {
        let v = remove_replay_protection(self.v);
        secp256k1::Signature::try_from_parts(self.r, self.s, v)
    }
}

impl FromStr for MessageSignature {
    type Err = KeyPairError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let bytes = s.decode_hex().map_err(|_| KeyPairError::InvalidSignature)?;
        Self::try_from(bytes.as_slice())
    }
}

impl<'a> TryFrom<&'a [u8]> for MessageSignature {
    type Error = KeyPairError;

    fn try_from(data: &'a [u8]) -> Result<Self, Self::Error> {
        if data.len() != Self::LEN {
            return Err(KeyPairError::InvalidSignature);
        }

        let r = H256::try_from(&data[secp256k1::Signature::R_RANGE])
            .expect("Expected 'r' 32 byte length array");
        let s = H256::try_from(&data[secp256k1::Signature::S_RANGE])
            .expect("Expected 's' 32 byte length array");
        let v = data[secp256k1::Signature::RECOVERY_LAST];
        Ok(MessageSignature { r, s, v })
    }
}
