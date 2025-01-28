// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use byteorder::{LittleEndian, ReadBytesExt};
use std::fmt;
use std::ops::Range;
use std::str::FromStr;
use tw_base58_address::Base58Address;
use tw_coin_entry::coin_entry::CoinAddress;
use tw_coin_entry::error::prelude::{AddressError, AddressResult};
use tw_encoding::base58;
use tw_encoding::base58::Alphabet;
use tw_hash::hasher::Hasher;
use tw_hash::H160;
use tw_memory::Data;

const X_ADDRESS_LEN: usize = 31;
const X_ADDRESS_CHECKSUM_LEN: usize = 4;

const MAINNET_PREFIX: [u8; 2] = [0x05, 0x44];
const PREFIX_RANGE: Range<usize> = 0..MAINNET_PREFIX.len();

const TAG_FLAG_POSITION: usize = 22;

const TAG_STARTS_AT: usize = 23;
const TAG_LEN: usize = size_of::<u32>();
const TAG_RANGE: Range<usize> = TAG_STARTS_AT..(TAG_STARTS_AT + TAG_LEN);

const KEY_HASH_RANGE: Range<usize> = 2..H160::LEN;

#[derive(strum_macros::FromRepr)]
#[repr(u8)]
pub enum TagFlag {
    None = 0x00,
    Classic = 0x01,
}

pub struct XAddress {
    /// Destination tag.
    tag: u32,
    inner: Base58Address<X_ADDRESS_LEN, X_ADDRESS_CHECKSUM_LEN>,
    /// Destination tag flag, none/32/64bit.
    tag_flag: TagFlag,
}

impl XAddress {
    pub fn public_key_hash(&self) -> H160 {
        H160::try_from(&self.inner.bytes[KEY_HASH_RANGE])
            .expect("'KEY_HASH_RANGE' must be 20 bytes length")
    }

    fn decode_tag_flag(bytes: &[u8; 31]) -> AddressResult<TagFlag> {
        TagFlag::from_repr(bytes[TAG_FLAG_POSITION]).ok_or(AddressError::InvalidInput)
    }

    fn read_tag(bytes: &[u8; 31]) -> AddressResult<u32> {
        (&bytes[TAG_RANGE])
            .read_u32::<LittleEndian>()
            .map_err(|_| AddressError::InvalidInput)
    }
}

impl FromStr for XAddress {
    type Err = AddressError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let inner = Base58Address::from_str_with_alphabet(s, Alphabet::Ripple, Hasher::Sha256d)?;

        // Check prefix.
        if inner.bytes[PREFIX_RANGE] != MAINNET_PREFIX {
            return Err(AddressError::UnexpectedAddressPrefix);
        }

        let tag = Self::read_tag(&inner.bytes)?;
        let tag_flag = Self::decode_tag_flag(&inner.bytes)?;
        Ok(XAddress {
            tag,
            inner,
            tag_flag,
        })
    }
}

impl fmt::Display for XAddress {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.inner)
    }
}

impl CoinAddress for XAddress {
    fn data(&self) -> Data {
        // TODO consider using another data to return.
        self.public_key_hash().to_vec()
    }
}
