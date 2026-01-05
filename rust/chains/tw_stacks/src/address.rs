// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::fmt;
use std::str::FromStr;
use tw_coin_entry::coin_entry::CoinAddress;
use tw_coin_entry::error::prelude::*;
use tw_encoding::base32;
use tw_hash::{hasher::StatefulHasher, ripemd::Sha256Ripemd, sha2, H160};
use tw_keypair::tw::PublicKey;
use tw_memory::Data;

const ALPHABET: &str = "0123456789ABCDEFGHJKMNPQRSTVWXYZ";

pub struct StacksAddress {
    version: u8,
    bytes: H160,
    check: [u8; 4],
}

impl StacksAddress {
    pub fn new(version: u8, public_key: &PublicKey) -> Self {
        let hasher = Sha256Ripemd;
        let hash_data = public_key.to_bytes();
        let mut hash_bytes = hasher.hash(&hash_data);
        let bytes = H160::try_from(hash_bytes.as_slice()).unwrap(); // Sha256Ripemd will always return correct number of bytes

        let mut check_data = Vec::new();
        check_data.push(version);
        check_data.append(&mut hash_bytes);

        let check_bytes = sha2::sha256(&sha2::sha256(&check_data))[0..4].to_vec();

        let check: [u8; 4] = check_bytes.try_into().unwrap();

        StacksAddress {
            version,
            bytes,
            check,
        }
    }
}

impl CoinAddress for StacksAddress {
    #[inline]
    fn data(&self) -> Data {
        self.bytes.to_vec()
    }
}

impl FromStr for StacksAddress {
    type Err = AddressError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        if &s[0..1] != "S" {
            return Err(AddressError::MissingPrefix);
        }

        let version = match &s[1..2] {
            "P" => 22,
            "M" => 20,
            "T" => 26,
            "N" => 21,
            _ => {
                return Err(AddressError::Unsupported);
            },
        };

        let payload = base32::decode(&s[2..], Some(ALPHABET.to_string()), false).unwrap();

        if payload[0] != version {
            return Err(AddressError::Unsupported);
        }

        let hash_bytes = payload[1..21].to_vec();
        let bytes = H160::try_from(hash_bytes.as_slice()).unwrap(); // Sha256Ripemd will always return correct number of bytes

        let check_data = &payload[0..21];

        let check_bytes = sha2::sha256(&sha2::sha256(check_data))[0..4].to_vec();

        if check_bytes != payload[21..].to_vec() {
            return Err(AddressError::InvalidChecksum);
        }

        let check: [u8; 4] = check_bytes.try_into().unwrap();

        Ok(StacksAddress {
            version,
            bytes,
            check,
        })
    }
}

impl fmt::Display for StacksAddress {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "S")?;
        match self.version {
            22 => write!(f, "P")?,
            20 => write!(f, "M")?,
            26 => write!(f, "T")?,
            21 => write!(f, "N")?,
            _ => panic!("bad version {}", self.version),
        }

        let mut bytes = Vec::new();
        bytes.push(self.version);
        bytes.append(&mut self.bytes.into_vec());
        bytes.append(&mut self.check.to_vec());

        let encoded = base32::encode(&bytes, Some(ALPHABET.to_string()), false).unwrap();

        write!(f, "{}", encoded)?;

        Ok(())
    }
}
