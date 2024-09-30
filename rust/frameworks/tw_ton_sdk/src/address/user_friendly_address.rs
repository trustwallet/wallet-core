// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::address_data::AddressData;
use crate::crc::CRC_16_XMODEM;
use tw_coin_entry::error::prelude::{AddressError, AddressResult};
use tw_encoding::base64;
use tw_encoding::base64::{NO_PAD, URL_NO_PAD};
use tw_hash::{H256, H288};

const BASE64_ADDRESS_LEN: usize = 48;
const CHECKSUM_MASK: u16 = 0xff;

const BOUNCEABLE: u8 = 0x11;
const NON_BOUNCEABLE: u8 = 0x51;
const BOUNCEABLE_TESTNET: u8 = 0x91;
const NON_BOUNCEABLE_TESTNET: u8 = 0xD1;

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct UserFriendlyAddress {
    data: AddressData,
    bounceable: bool,
    testnet: bool,
}

impl UserFriendlyAddress {
    #[inline]
    pub const fn with_flags(data: AddressData, bounceable: bool, testnet: bool) -> Self {
        UserFriendlyAddress {
            data,
            bounceable,
            testnet,
        }
    }

    #[inline]
    pub fn into_data(self) -> AddressData {
        self.data
    }

    #[inline]
    pub fn bounceable(&self) -> bool {
        self.bounceable
    }

    #[inline]
    pub fn testnet(&self) -> bool {
        self.testnet
    }

    #[inline]
    pub fn set_bounceable(self, bounceable: bool) -> Self {
        UserFriendlyAddress { bounceable, ..self }
    }

    #[inline]
    pub fn set_testnet(self, testnet: bool) -> Self {
        UserFriendlyAddress { testnet, ..self }
    }

    /// Parses url-safe base64 representation of an address
    ///
    /// # Returns
    /// the address, non-bounceable flag, non-production flag.
    pub fn from_base64_url(s: &str) -> AddressResult<Self> {
        Self::from_base64_with_config(s, URL_NO_PAD)
    }

    /// Parses standard base64 representation of an address
    ///
    /// # Returns
    /// the address, bounceable flag, testnet flag.
    pub fn from_base64_std(s: &str) -> AddressResult<Self> {
        Self::from_base64_with_config(s, NO_PAD)
    }

    /// Parses base64 representation of an address with encoding config.
    ///
    /// # Returns
    /// the address, non-bounceable flag, non-production flag.
    fn from_base64_with_config(s: &str, config: base64::Config) -> AddressResult<Self> {
        if s.len() != BASE64_ADDRESS_LEN {
            return Err(AddressError::InvalidInput);
        }
        let bytes = base64::decode(s, config).map_err(|_| AddressError::FromBase64Error)?;
        // Address length has been checked already.
        let slice = H288::try_from(bytes.as_slice()).map_err(|_| AddressError::Internal)?;
        Self::from_base64_bytes(slice)
    }

    /// Parses decoded base64 representation of an address
    ///
    /// # Returns
    /// the address, bounceable flag, testnet flag.
    fn from_base64_bytes(bytes: H288) -> AddressResult<Self> {
        let (bounceable, testnet) = match bytes[0] {
            BOUNCEABLE => (true, false),
            NON_BOUNCEABLE => (false, false),
            BOUNCEABLE_TESTNET => (true, true),
            NON_BOUNCEABLE_TESTNET => (false, true),
            _ => return Err(AddressError::InvalidInput),
        };

        let workchain = bytes[1] as i8 as i32;

        let calc_crc = CRC_16_XMODEM.checksum(&bytes[0..34]);
        let addr_crc = ((bytes[34] as u16) << 8) | bytes[35] as u16;
        if calc_crc != addr_crc {
            return Err(AddressError::InvalidChecksum);
        }

        let hash_part = H256::try_from(&bytes[2..34]).expect("Expected exactly 32 bytes");
        let data = AddressData::new(workchain, hash_part);
        Ok(UserFriendlyAddress {
            data,
            bounceable,
            testnet,
        })
    }

    pub fn to_base64_url(&self) -> String {
        self.to_base64_with_config(URL_NO_PAD)
    }

    pub fn to_base64_std(&self) -> String {
        self.to_base64_with_config(NO_PAD)
    }

    fn to_base64_with_config(&self, config: base64::Config) -> String {
        let bytes = self.to_base64_bytes();
        base64::encode(bytes.as_slice(), config)
    }

    fn to_base64_bytes(&self) -> H288 {
        let mut bytes = H288::default();
        let tag: u8 = match (self.bounceable, self.testnet) {
            (false, false) => NON_BOUNCEABLE,
            (true, false) => BOUNCEABLE,
            (false, true) => NON_BOUNCEABLE_TESTNET,
            (true, true) => BOUNCEABLE_TESTNET,
        };
        bytes[0] = tag;
        bytes[1] = self.data.workchain_byte();
        bytes[2..34].clone_from_slice(self.data.hash_part.as_slice());
        let crc = CRC_16_XMODEM.checksum(&bytes[0..34]);
        bytes[34] = ((crc >> 8) & CHECKSUM_MASK) as u8;
        bytes[35] = (crc & CHECKSUM_MASK) as u8;

        bytes
    }
}

impl AsRef<AddressData> for UserFriendlyAddress {
    #[inline]
    fn as_ref(&self) -> &AddressData {
        &self.data
    }
}
