// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::SOLANA_ALPHABET;
use serde::Deserialize;
use std::fmt;
use std::str::FromStr;
use tw_encoding::base58::{self, as_base58_bitcoin};
use tw_encoding::EncodingError;
use tw_hash::H256;

#[derive(Clone, Copy, Default, Deserialize)]
pub struct Blockhash(#[serde(with = "as_base58_bitcoin")] H256);

impl Blockhash {
    pub fn with_bytes(bytes: H256) -> Blockhash {
        Blockhash(bytes)
    }

    pub fn to_bytes(&self) -> H256 {
        self.0
    }
}

impl fmt::Display for Blockhash {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", base58::encode(self.0.as_slice(), &SOLANA_ALPHABET))
    }
}

impl FromStr for Blockhash {
    type Err = EncodingError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let bytes = base58::decode(s, &SOLANA_ALPHABET)?;
        let bytes = H256::try_from(bytes.as_slice()).map_err(|_| EncodingError::InvalidInput)?;
        Ok(Blockhash(bytes))
    }
}
