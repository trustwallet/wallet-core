// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::SOLANA_ALPHABET;
use serde::de::Error as DeError;
use serde::{Deserialize, Deserializer, Serialize, Serializer};
use std::fmt;
use std::str::FromStr;
use tw_coin_entry::coin_entry::CoinAddress;
use tw_coin_entry::error::{AddressError, AddressResult};
use tw_encoding::base58;
use tw_hash::H256;
use tw_keypair::tw;
use tw_memory::Data;

#[derive(Clone, Copy)]
pub struct SolanaAddress {
    bytes: H256,
}

impl SolanaAddress {
    pub fn with_public_key(public_key: &tw::PublicKey) -> AddressResult<SolanaAddress> {
        let bytes = public_key
            .to_ed25519()
            .ok_or(AddressError::PublicKeyTypeMismatch)?
            .to_bytes();
        Ok(SolanaAddress { bytes })
    }

    pub fn with_public_key_bytes(bytes: H256) -> SolanaAddress {
        SolanaAddress { bytes }
    }

    pub fn bytes(&self) -> H256 {
        self.bytes
    }
}

impl CoinAddress for SolanaAddress {
    #[inline]
    fn data(&self) -> Data {
        self.bytes.to_vec()
    }
}

impl FromStr for SolanaAddress {
    type Err = AddressError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let bytes =
            base58::decode(s, &SOLANA_ALPHABET).map_err(|_| AddressError::FromBase58Error)?;
        let bytes = H256::try_from(bytes.as_slice()).map_err(|_| AddressError::InvalidInput)?;
        Ok(SolanaAddress { bytes })
    }
}

impl fmt::Display for SolanaAddress {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        let encoded = base58::encode(self.bytes.as_slice(), &SOLANA_ALPHABET);
        write!(f, "{}", encoded)
    }
}

impl Serialize for SolanaAddress {
    fn serialize<S>(&self, serializer: S) -> Result<S::Ok, S::Error>
    where
        S: Serializer,
    {
        self.to_string().serialize(serializer)
    }
}

impl<'de> Deserialize<'de> for SolanaAddress {
    fn deserialize<D>(deserializer: D) -> Result<Self, D::Error>
    where
        D: Deserializer<'de>,
    {
        let addr_str = String::deserialize(deserializer)?;
        SolanaAddress::from_str(&addr_str).map_err(|e| DeError::custom(format!("{e:?}")))
    }
}
