use super::{
    Bech32Prefix, MAX_WITNESS_VERSION, WITNESS_V1, WITNESS_V1_VALID_PROGRAM_SIZES,
    WITNESS_VALID_PROGRAM_SIZES, WITNESS_VERSIONS,
};
use bech32::FromBase32;
use core::fmt;
use std::str::FromStr;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::{AddressError, AddressResult};
use tw_hash::hasher::sha256_ripemd;
use tw_keypair::tw;
use tw_memory::Data;

#[derive(Debug, Eq, PartialEq)]
pub struct TaprootAddress {
    hrp: String,
    witness_version: u8,
    witness_program: Data,
    /// An address string created from this `hrp`, `witness_version` and `witness_program`.
    address_str: String,
}

impl TaprootAddress {
    pub fn new(
        hrp: String,
        witness_version: u8,
        witness_program: Data,
    ) -> AddressResult<TaprootAddress> {
        if !WITNESS_VERSIONS.contains(&witness_version) {
            return Err(AddressError::Unsupported);
        }

        if !WITNESS_VALID_PROGRAM_SIZES.contains(&witness_program.len()) {
            return Err(AddressError::InvalidInput);
        }

        // Specific Taproot V1 check. These addresses can never spend funds sent to them.
        if witness_version == WITNESS_V1
            && !WITNESS_V1_VALID_PROGRAM_SIZES.contains(&witness_program.len())
        {
            return Err(AddressError::InvalidInput);
        }

        let address_str = Self::fmt_internal(&hrp, witness_version, &witness_program)?;
        Ok(TaprootAddress {
            hrp,
            witness_version,
            witness_program,
            address_str,
        })
    }

    pub fn p2wpkh_with_coin_and_prefix(
        coin: &dyn CoinContext,
        public_key: &tw::PublicKey,
        prefix: Option<Bech32Prefix>,
    ) -> AddressResult<TaprootAddress> {
        let hrp = match prefix {
            Some(Bech32Prefix { hrp }) => hrp,
            None => coin.hrp().ok_or(AddressError::InvalidRegistry)?,
        };

        let public_key_bytes = public_key
            .to_secp256k1()
            .ok_or(AddressError::PublicKeyTypeMismatch)?
            .compressed();
        let public_key_hash = sha256_ripemd(public_key_bytes.as_slice());

        Self::new(hrp, WITNESS_V1, public_key_hash.to_vec())
    }

    pub fn from_str_checked(s: &str, expected_hrp: &str) -> AddressResult<TaprootAddress> {
        let address = Self::from_str(s)?;
        if address.hrp != expected_hrp {
            return Err(AddressError::InvalidHrp);
        }
        Ok(address)
    }

    pub fn from_str_with_coin_and_prefix(
        coin: &dyn CoinContext,
        s: &str,
        prefix: Option<Bech32Prefix>,
    ) -> AddressResult<TaprootAddress> {
        let hrp = match prefix {
            Some(Bech32Prefix { hrp }) => hrp,
            None => coin.hrp().ok_or(AddressError::InvalidRegistry)?,
        };
        TaprootAddress::from_str_checked(s, &hrp)
    }

    pub fn witness_version(&self) -> u8 {
        self.witness_version
    }

    pub fn witness_program(&self) -> &[u8] {
        &self.witness_program
    }

    fn fmt_internal(
        hrp: &str,
        witness_version: u8,
        witness_program: &[u8],
    ) -> AddressResult<String> {
        const STRING_CAPACITY: usize = 100;

        let mut result_addr = String::with_capacity(STRING_CAPACITY);

        let bech32_variant = match witness_version {
            0 => bech32::Variant::Bech32,
            _ => bech32::Variant::Bech32m,
        };
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

impl FromStr for TaprootAddress {
    type Err = AddressError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let (hrp, payload_u5, checksum_variant) =
            bech32::decode(s).map_err(|_| AddressError::FromBech32Error)?;

        if payload_u5.is_empty() {
            return Err(AddressError::InvalidInput);
        }

        // Get the script version and program (converted from 5-bit to 8-bit)
        let (version, program) = payload_u5.split_at(1);
        let version = version[0].to_u8();
        let program = Data::from_base32(program).map_err(|_| AddressError::FromBech32Error)?;

        if version > MAX_WITNESS_VERSION {
            return Err(AddressError::Unsupported);
        }

        // Check encoding.
        match (version, checksum_variant) {
            (0, bech32::Variant::Bech32) => (),
            (_, bech32::Variant::Bech32m) => (),
            _ => return Err(AddressError::InvalidInput),
        }

        TaprootAddress::new(hrp, version, program)
    }
}

impl fmt::Display for TaprootAddress {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.address_str)
    }
}
