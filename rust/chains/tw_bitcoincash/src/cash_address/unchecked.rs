// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::cash_address::checksum::{calculate_checksum, CHECKSUM_LEN};
use crate::cash_address::{cash_base32, CashAddress, CashAddressType};
use std::fmt;
use std::str::FromStr;
use tw_coin_entry::error::prelude::{AddressError, AddressResult};
use tw_encoding::bech32;
use tw_hash::H160;
use tw_memory::Data;

/// BitcoinCash address may or may not have a prefix.
struct CashAddressParts {
    prefix: Option<String>,
    payload: String,
}

impl FromStr for CashAddressParts {
    type Err = AddressError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let address_str = s.to_lowercase();
        let tokens: Vec<&str> = address_str.split(':').collect();

        if tokens.len() == 1 {
            Ok(CashAddressParts {
                prefix: None,
                payload: tokens[0].to_string(),
            })
        } else if tokens.len() == 2 {
            Ok(CashAddressParts {
                prefix: Some(tokens[0].to_string()),
                payload: tokens[1].to_string(),
            })
        } else {
            Err(AddressError::InvalidInput)
        }
    }
}

impl fmt::Display for CashAddressParts {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self.prefix {
            Some(ref prefix) => write!(f, "{prefix}:{}", self.payload),
            None => write!(f, "{}", self.payload),
        }
    }
}

/// BitcoinCash address with an unchecked checksum and prefix.
/// Consider using [`CashAddress`] directly instead.
pub struct UncheckedCashAddress {
    parts: CashAddressParts,
    ty: CashAddressType,
    key_hash: H160,
    /// Original 5-bit packed array decoded from base32.
    /// Used to validate the checksum along with a prefix.
    unchecked_payload_with_checksum: Data,
}

impl UncheckedCashAddress {
    /// Checks whether the address has an expected prefix, and verifies the address checksum.
    pub fn checked_with_prefix(self, expected_prefix: String) -> AddressResult<CashAddress> {
        // Validate the prefix if it was present.
        if let Some(ref prefix) = self.parts.prefix {
            if *prefix != expected_prefix {
                return Err(AddressError::InvalidHrp);
            }
        }

        // Ensure the checksum is zero when decoding.
        let checksum = calculate_checksum(&expected_prefix, &self.unchecked_payload_with_checksum);
        if checksum != 0 {
            return Err(AddressError::InvalidChecksum);
        }

        let address_str = format!("{expected_prefix}:{}", self.parts.payload);
        Ok(CashAddress {
            hrp: expected_prefix,
            ty: self.ty,
            key_hash: self.key_hash,
            address_str,
        })
    }

    /// Tries to verify the address checksum if it contains a prefix, otherwise do not verify at all.
    /// Please note this method doesn't validate if the prefix belongs to a network.
    ///
    /// Consider using [`UncheckedCashAddress::checked_with_prefix`] instead.
    pub fn partly_checked(self) -> AddressResult<CashAddress> {
        match self.parts.prefix.clone() {
            Some(prefix) => self.checked_with_prefix(prefix),
            // Do not check validity of the address and return as is.
            None => Ok(CashAddress {
                hrp: String::default(),
                ty: self.ty,
                key_hash: self.key_hash,
                address_str: self.parts.to_string(),
            }),
        }
    }
}

impl FromStr for UncheckedCashAddress {
    type Err = AddressError;

    fn from_str(address_str: &str) -> Result<Self, Self::Err> {
        let address_parts = CashAddressParts::from_str(address_str)?;

        let payload_with_checksum =
            cash_base32::decode(&address_parts.payload).map_err(|_| AddressError::InvalidInput)?;

        // Get the payload without the checksum.
        let payload_u5 = &payload_with_checksum[..payload_with_checksum.len() - CHECKSUM_LEN];

        let payload_data = {
            let from = 5;
            let to = 8;
            let pad = false;
            bech32::convert_bits(payload_u5, from, to, pad)
                .map_err(|_| AddressError::InvalidInput)?
        };

        let version_byte = payload_data[0];
        let ty = get_address_type(version_byte)?;
        let key_hash =
            H160::try_from(&payload_data[1..]).map_err(|_| AddressError::InvalidInput)?;

        // `encoded_payload` is checked already, and it contains the valid checksum at the end.
        Ok(UncheckedCashAddress {
            parts: address_parts,
            ty,
            key_hash,
            unchecked_payload_with_checksum: payload_with_checksum,
        })
    }
}

fn get_address_type(version_byte: u8) -> AddressResult<CashAddressType> {
    match version_byte & 8 {
        0 => Ok(CashAddressType::P2PKH),
        8 => Ok(CashAddressType::P2SH),
        _ => Err(AddressError::UnexpectedAddressPrefix),
    }
}
