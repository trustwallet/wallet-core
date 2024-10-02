// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use bech32::FromBase32;
use std::fmt;
use std::ops::RangeInclusive;
use tw_coin_entry::error::prelude::*;
use tw_memory::Data;

/// cbindgen:ignore
pub const WITNESS_V0: u8 = 0;
/// cbindgen:ignore
pub const MAX_WITNESS_VERSION: u8 = 16;
/// cbindgen:ignore
pub const WITNESS_VERSIONS: RangeInclusive<u8> = WITNESS_V0..=MAX_WITNESS_VERSION;
/// Witness program sizes valid for most of the witness versions.
/// Please note that V0 is more constraint.
/// cbindgen:ignore
pub const WITNESS_VALID_PROGRAM_SIZES: RangeInclusive<usize> = 2..=40;

/// A segwit address implementation that supports various program versions.
/// For example:
/// * witness V0 is Segwit address
/// * witness V1 is Taproot address
#[derive(Clone, Debug, Eq, PartialEq)]
pub struct WitnessProgram {
    hrp: String,
    witness_version: u8,
    witness_program: Data,
    /// An address string created from this `hrp`, `witness_version` and `witness_program`.
    address_str: String,
    bech32_variant: bech32::Variant,
}

impl WitnessProgram {
    pub fn new(
        hrp: String,
        witness_version: u8,
        witness_program: Data,
        bech32_variant: bech32::Variant,
    ) -> AddressResult<WitnessProgram> {
        if !WITNESS_VERSIONS.contains(&witness_version) {
            return Err(AddressError::Unsupported);
        }

        if !WITNESS_VALID_PROGRAM_SIZES.contains(&witness_program.len()) {
            return Err(AddressError::InvalidInput);
        }

        let address_str =
            Self::fmt_internal(&hrp, witness_version, &witness_program, bech32_variant)?;
        Ok(WitnessProgram {
            hrp,
            witness_version,
            witness_program,
            address_str,
            bech32_variant,
        })
    }

    pub fn witness_version(&self) -> u8 {
        self.witness_version
    }

    pub fn witness_program(&self) -> &[u8] {
        &self.witness_program
    }

    pub fn hrp(&self) -> &str {
        &self.hrp
    }

    pub fn from_str_checked(
        s: &str,
        expected_version: u8,
        expected_checksum_type: bech32::Variant,
        valid_program_sizes: &[usize],
    ) -> AddressResult<WitnessProgram> {
        let (hrp, payload_u5, checksum_variant) =
            bech32::decode(s).map_err(|_| AddressError::FromBech32Error)?;

        if payload_u5.is_empty() {
            return Err(AddressError::InvalidInput);
        }

        // Get the script version and program (converted from 5-bit to 8-bit)
        let (version, program) = payload_u5.split_at(1);
        let version = version[0].to_u8();
        let program = Data::from_base32(program).map_err(|_| AddressError::FromBech32Error)?;

        // Check witness version.
        if version != expected_version {
            return Err(AddressError::Unsupported);
        }

        // Check encoding.
        if checksum_variant != expected_checksum_type {
            return Err(AddressError::InvalidInput);
        }

        // Check witness program sizes.
        if !valid_program_sizes.contains(&program.len()) {
            return Err(AddressError::InvalidInput);
        }

        WitnessProgram::new(hrp, version, program, checksum_variant)
    }

    fn fmt_internal(
        hrp: &str,
        witness_version: u8,
        witness_program: &[u8],
        bech32_variant: bech32::Variant,
    ) -> AddressResult<String> {
        const STRING_CAPACITY: usize = 100;

        let mut result_addr = String::with_capacity(STRING_CAPACITY);

        let version_u5 =
            bech32::u5::try_from_u8(witness_version).expect("WitnessVersion must be 0..=16");

        {
            let mut bech32_writer =
                bech32::Bech32Writer::new(hrp, bech32_variant, &mut result_addr)
                    .map_err(|_| AddressError::FromBech32Error)?;
            bech32::WriteBase32::write_u5(&mut bech32_writer, version_u5)
                .map_err(|_| AddressError::FromBech32Error)?;
            bech32::ToBase32::write_base32(&witness_program, &mut bech32_writer)
                .map_err(|_| AddressError::FromBech32Error)?;
        }

        Ok(result_addr)
    }
}

impl fmt::Display for WitnessProgram {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.address_str)
    }
}
