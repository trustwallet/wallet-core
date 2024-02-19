// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::SOLANA_ALPHABET;
use std::fmt;
use std::str::FromStr;
use tw_encoding::{base58, EncodingError};
use tw_hash::H256;

#[derive(Clone, Copy)]
pub struct Blockhash {
    bytes: H256,
}

impl Blockhash {
    pub fn to_bytes(&self) -> H256 {
        self.bytes
    }
}

impl fmt::Display for Blockhash {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(
            f,
            "{}",
            base58::encode(self.bytes.as_slice(), &SOLANA_ALPHABET)
        )
    }
}

impl FromStr for Blockhash {
    type Err = EncodingError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let bytes = base58::decode(s, &SOLANA_ALPHABET)?;
        let bytes = H256::try_from(bytes.as_slice()).map_err(|_| EncodingError::InvalidInput)?;
        Ok(Blockhash { bytes })
    }
}
