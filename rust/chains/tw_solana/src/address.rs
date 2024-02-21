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
use tw_hash::{as_byte_sequence, H256};
use tw_keypair::{ed25519, tw};
use tw_memory::Data;

#[derive(Clone, Copy, Default, Eq, Hash, Ord, PartialEq, PartialOrd)]
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

    pub fn with_public_key_ed25519(public_key: &ed25519::sha512::PublicKey) -> SolanaAddress {
        SolanaAddress {
            bytes: public_key.to_bytes(),
        }
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

impl From<&'static str> for SolanaAddress {
    fn from(s: &'static str) -> Self {
        SolanaAddress::from_str(s).unwrap()
    }
}

impl<'a> TryFrom<&'a [u8]> for SolanaAddress {
    type Error = AddressError;

    fn try_from(bytes: &'a [u8]) -> Result<Self, Self::Error> {
        let bytes = H256::try_from(bytes).map_err(|_| AddressError::InvalidInput)?;
        Ok(SolanaAddress::with_public_key_bytes(bytes))
    }
}

impl fmt::Debug for SolanaAddress {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self)
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
        if serializer.is_human_readable() {
            return self.to_string().serialize(serializer);
        }
        as_byte_sequence::serialize(&self.bytes(), serializer)
    }
}

impl<'de> Deserialize<'de> for SolanaAddress {
    fn deserialize<D>(deserializer: D) -> Result<Self, D::Error>
    where
        D: Deserializer<'de>,
    {
        if deserializer.is_human_readable() {
            let addr_str = String::deserialize(deserializer)?;
            return SolanaAddress::from_str(&addr_str)
                .map_err(|e| DeError::custom(format!("{e:?}")));
        }
        let bytes = as_byte_sequence::deserialize(deserializer)?;
        Ok(SolanaAddress { bytes })
    }
}
