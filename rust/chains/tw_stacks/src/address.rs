// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use stacks_core::c32;
use std::fmt;
use std::str::FromStr;
use tw_coin_entry::coin_entry::CoinAddress;
use tw_coin_entry::error::prelude::*;
use tw_hash::{hasher::StatefulHasher, ripemd::Sha256Ripemd, sha2, H160};
use tw_keypair::tw::PublicKey;
use tw_memory::Data;

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
        // first, normalize the following characters:
        // O, o => 0
        // I, i => 1
        // L, l => 1
        let s = s.replace("O", "0");
        let s = s.replace("o", "0");
        let s = s.replace("I", "1");
        let s = s.replace("i", "1");
        let s = s.replace("L", "1");
        let s = s.replace("l", "1");
        let s = s.as_str();

        if s.len() < 2 {
            return Err(AddressError::MissingPrefix);
        }

        if &s[0..1] != "S" {
            return Err(AddressError::MissingPrefix);
        }

        let version = match &s[1..2] {
            "P" => 22,
            "M" => 20,
            "T" => 26,
            "N" => 21,
            prefix => {
                eprintln!("unsupported address prefix {prefix}");
                return Err(AddressError::UnexpectedAddressPrefix);
            },
        };

        let payload = c32::decode(&s[2..]).map_err(|e| {
            eprintln!("c32::decode error {e:?} on str {}", &s[2..]);
            AddressError::Unsupported
        })?;

        if payload.len() < 24 {
            eprintln!("from_str Unsupported payload.len < 24");
            return Err(AddressError::Unsupported);
        }

        let hash_bytes = payload[0..20].to_vec();
        let bytes = H160::try_from(hash_bytes.as_slice()).unwrap(); // Sha256Ripemd will always return correct number of bytes

        let mut check_data = Vec::new();
        check_data.push(version);
        check_data.extend_from_slice(&payload[0..20]);

        let check_bytes = sha2::sha256(&sha2::sha256(&check_data))[0..4].to_vec(); // we can always grab 4 bytes from the beginning of a SHA256 hash

        if check_bytes != payload[20..].to_vec() {
            return Err(AddressError::InvalidChecksum);
        }

        let check: [u8; 4] = check_bytes.try_into().unwrap(); // we grabbed exactly 4 bytes above

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
        bytes.append(&mut self.bytes.into_vec());
        bytes.append(&mut self.check.to_vec());

        let encoded = c32::encode(&bytes);

        write!(f, "{}", encoded)?;

        Ok(())
    }
}
