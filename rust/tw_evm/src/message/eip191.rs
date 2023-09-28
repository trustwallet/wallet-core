// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::message::{EthMessage, MessageSigningResult};
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
        data.extend_from_slice(self.user_message.len().to_string().as_bytes());
        data.extend_from_slice(self.user_message.as_bytes());

        data
    }
}

impl EthMessage for Eip191Message {
    fn hash(&self) -> MessageSigningResult<H256> {
        let hash = keccak256(&self.data_to_sign());
        Ok(H256::try_from(hash.as_slice()).expect("Expected 32 byte hash"))
    }
}
