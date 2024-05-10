// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::message::{EthMessage, MessageSigningResult};
use tw_encoding::hex;
use tw_hash::sha3::keccak256;
use tw_hash::H256;

/// cbindgen:ignore
pub const ETHEREUM_PREFIX: u8 = 0x19;
/// cbindgen:ignore
pub const ETHEREUM_MESSAGE_PREFIX: &str = "Ethereum Signed Message:\n";

pub struct Eip191Message {
    user_message: String,
}

impl Eip191Message {
    pub fn new<S: Into<String>>(user_message: S) -> Eip191Message {
        Eip191Message {
            user_message: user_message.into(),
        }
    }

    fn data_to_sign(&self) -> Vec<u8> {
        let mut data = Vec::with_capacity(self.user_message.len() * 2);

        data.push(ETHEREUM_PREFIX);
        data.extend_from_slice(ETHEREUM_MESSAGE_PREFIX.as_bytes());

        let mut do_extend = |bytes: &[u8]| {
            data.extend_from_slice(bytes.len().to_string().as_bytes());
            data.extend_from_slice(bytes);
        };

        match hex::decode(&self.user_message) {
            // Handle the message as hex-encoded.
            Ok(raw_msg) => do_extend(&raw_msg),
            // Handle as a regular string.
            Err(_) => do_extend(self.user_message.as_bytes()),
        }

        data
    }
}

impl EthMessage for Eip191Message {
    fn hash(&self) -> MessageSigningResult<H256> {
        let hash = keccak256(&self.data_to_sign());
        Ok(H256::try_from(hash.as_slice()).expect("Expected 32 byte hash"))
    }
}
