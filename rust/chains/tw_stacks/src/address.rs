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
        println!("from_str {s}");

        // first, normalize the following characters:
        // O, o => 0
        // I, i => 1
        // L, l => 1
        let s = s.replace("O", "0");
        println!("from_str {s}");
        let s = s.replace("o", "0");
        println!("from_str {s}");
        let s = s.replace("I", "1");
        println!("from_str {s}");
        let s = s.replace("i", "1");
        println!("from_str {s}");
        let s = s.replace("L", "1");
        println!("from_str {s}");
        let s = s.replace("l", "1");
        println!("from_str {s}");
        let s = s.as_str();
        println!("from_str {s}");

        if s.len() < 2 {
            println!("from_str s.len() < 2");
            return Err(AddressError::MissingPrefix);
        }

        if &s[0..1] != "S" {
            println!("from_str &s[0..1] != \"S\" ");
            return Err(AddressError::MissingPrefix);
        }

        println!("from_str {s} check version");
        let version = match &s[1..2] {
            "P" => 22,
            "M" => 20,
            "T" => 26,
            "N" => 21,
            _ => {
                println!("from_str UnexpectedAddressPrefix");
                return Err(AddressError::UnexpectedAddressPrefix);
            },
        };

        println!("from_str {s} decode payload");
        let payload = base32::decode(&s[2..], Some(ALPHABET.to_string()), false)
            .map_err(|_| AddressError::FromBech32Error)?;
        println!(
            "from_str base32::decode succeeded payload.len {}",
            payload.len()
        );

        println!("from_str {s} check payload len");
        if payload.len() < 24 {
            println!("from_str Unsupported payload.len < 24");
            return Err(AddressError::Unsupported);
        }

        println!("from_str {s} get H160");
        let hash_bytes = payload[0..20].to_vec();
        let bytes = H160::try_from(hash_bytes.as_slice()).unwrap(); // Sha256Ripemd will always return correct number of bytes

        println!("from_str {s} create check data");
        let mut check_data = Vec::new();
        check_data.push(version);
        check_data.extend_from_slice(&payload[0..20]);

        println!("from_str {s} compute check bytes");
        let check_bytes = sha2::sha256(&sha2::sha256(&check_data))[0..4].to_vec(); // we can always grab 4 bytes from the beginning of a SHA256 hash

        println!("from_str {s} compare check bytes to payload");
        if check_bytes != payload[20..].to_vec() {
            println!("from_str InvalidChecksum");
            return Err(AddressError::InvalidChecksum);
        }

        println!("from_str {s} store check bytes");
        let check: [u8; 4] = check_bytes.try_into().unwrap(); // we grabbed exactly 4 bytes above

        println!(
            "from_str succeeded version {version} bytes.len {} check",
            bytes.len()
        );

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

        let encoded = base32::encode(&bytes, Some(ALPHABET.to_string()), false).unwrap();

        write!(f, "{}", encoded)?;

        Ok(())
    }
}
