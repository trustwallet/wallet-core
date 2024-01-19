// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use bech32::FromBase32;
use core::fmt;
use std::ops::RangeInclusive;
use std::str::FromStr;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::{AddressError, AddressResult};
use tw_hash::hasher::sha256_ripemd;
use tw_hash::{H160, H256};
use tw_keypair::tw;
use tw_memory::Data;

const WITNESS_V0: u8 = 0;
const MAX_WITNESS_VERSION: u8 = 16;
const WITNESS_VERSIONS: RangeInclusive<u8> = WITNESS_V0..=MAX_WITNESS_VERSION;
/// Witness program sizes valid for V0.
const WITNESS_V0_VALID_PROGRAM_SIZES: [usize; 2] = [H160::LEN, H256::LEN];
/// Witness program sizes valid for most of the witness versions.
/// Please note that V0 is more constraint.
const WITNESS_VALID_PROGRAM_SIZES: RangeInclusive<usize> = 2..=40;

pub type Bech32Prefix = tw_bech32_address::bech32_prefix::Bech32Prefix;

#[derive(Debug, Eq, PartialEq)]
pub struct SegwitAddress {
    hrp: String,
    witness_version: u8,
    witness_program: Data,
    /// An address string created from this `hrp`, `witness_version` and `witness_program`.
    address_str: String,
}

impl SegwitAddress {
    pub fn new(
        hrp: String,
        witness_version: u8,
        witness_program: Data,
    ) -> AddressResult<SegwitAddress> {
        if !WITNESS_VERSIONS.contains(&witness_version) {
            return Err(AddressError::Unsupported);
        }

        if !WITNESS_VALID_PROGRAM_SIZES.contains(&witness_program.len()) {
            return Err(AddressError::InvalidInput);
        }

        // Specific segwit v0 check. These addresses can never spend funds sent to them.
        if witness_version == WITNESS_V0
            && !WITNESS_V0_VALID_PROGRAM_SIZES.contains(&witness_program.len())
        {
            return Err(AddressError::InvalidInput);
        }

        let address_str = Self::fmt_internal(&hrp, witness_version, &witness_program)?;
        Ok(SegwitAddress {
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
    ) -> AddressResult<SegwitAddress> {
        let hrp = match prefix {
            Some(Bech32Prefix { hrp }) => hrp,
            None => coin.hrp().ok_or(AddressError::InvalidRegistry)?,
        };

        let public_key_bytes = public_key
            .to_secp256k1()
            .ok_or(AddressError::PublicKeyTypeMismatch)?
            .compressed();
        let public_key_hash = sha256_ripemd(public_key_bytes.as_slice());

        Self::new(hrp, WITNESS_V0, public_key_hash.to_vec())
    }

    pub fn from_str_checked(s: &str, expected_hrp: &str) -> AddressResult<SegwitAddress> {
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
    ) -> AddressResult<SegwitAddress> {
        let hrp = match prefix {
            Some(Bech32Prefix { hrp }) => hrp,
            None => coin.hrp().ok_or(AddressError::InvalidRegistry)?,
        };
        SegwitAddress::from_str_checked(s, &hrp)
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

impl FromStr for SegwitAddress {
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

        SegwitAddress::new(hrp, version, program)
    }
}

impl fmt::Display for SegwitAddress {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.address_str)
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use tw_encoding::hex::DecodeHex;

    struct TestInputValid {
        str: &'static str,
        normalized: &'static str,
        expected: SegwitAddress,
    }

    fn segwit_addr(hrp: &str, version: u8, program: &str) -> SegwitAddress {
        SegwitAddress::new(hrp.to_string(), version, program.decode_hex().unwrap())
            .expect("Cannot construct a SegwitAddress from the input")
    }

    /// Tests if the given `s` string representation is converted from and to `expected` segwit address.
    #[track_caller]
    fn test_to_from_str_valid(input: TestInputValid) {
        let actual = SegwitAddress::from_str(input.str).expect("Expected a valid address");
        assert_eq!(actual, input.expected, "String -> SegwitAddress");

        let actual_str = actual.to_string();
        assert_eq!(actual_str, input.normalized, "SegwitAddress -> String");
    }

    #[test]
    fn test_segwit_address_to_from_str() {
        test_to_from_str_valid(TestInputValid {
            str: "BC1QW508D6QEJXTDG4Y5R3ZARVARY0C5XW7KV8F3T4",
            normalized: "bc1qw508d6qejxtdg4y5r3zarvary0c5xw7kv8f3t4",
            expected: segwit_addr("bc", 0, "751e76e8199196d454941c45d1b3a323f1433bd6"),
        });

        test_to_from_str_valid(TestInputValid {
            str: "tb1qrp33g0q5c5txsp9arysrx4k6zdkfs4nce4xj0gdcccefvpysxf3q0sl5k7",
            normalized: "tb1qrp33g0q5c5txsp9arysrx4k6zdkfs4nce4xj0gdcccefvpysxf3q0sl5k7",
            expected: segwit_addr(
                "tb",
                0,
                "1863143c14c5166804bd19203356da136c985678cd4d27a1b8c6329604903262",
            ),
        });

        test_to_from_str_valid(TestInputValid {
            str: "tb1qqqqqp399et2xygdj5xreqhjjvcmzhxw4aywxecjdzew6hylgvsesrxh6hy",
            normalized: "tb1qqqqqp399et2xygdj5xreqhjjvcmzhxw4aywxecjdzew6hylgvsesrxh6hy",
            expected: segwit_addr(
                "tb",
                0,
                "000000c4a5cad46221b2a187905e5266362b99d5e91c6ce24d165dab93e86433",
            ),
        });

        test_to_from_str_valid(TestInputValid {
            str: "bc1qpjult34k9spjfym8hss2jrwjgf0xjf40ze0pp8",
            normalized: "bc1qpjult34k9spjfym8hss2jrwjgf0xjf40ze0pp8",
            expected: segwit_addr("bc", 0, "0cb9f5c6b62c03249367bc20a90dd2425e6926af"),
        });

        test_to_from_str_valid(TestInputValid {
            str: "bc1qm9jzmujvdqjj6y28hptk859zs3yyv78hpqqjfj",
            normalized: "bc1qm9jzmujvdqjj6y28hptk859zs3yyv78hpqqjfj",
            expected: segwit_addr("bc", 0, "d9642df24c68252d1147b85763d0a284484678f7"),
        });

        test_to_from_str_valid(TestInputValid {
            str: "bc1ptmsk7c2yut2xah4pgflpygh2s7fh0cpfkrza9cjj29awapv53mrslgd5cf",
            normalized: "bc1ptmsk7c2yut2xah4pgflpygh2s7fh0cpfkrza9cjj29awapv53mrslgd5cf",
            expected: segwit_addr(
                "bc",
                1,
                "5ee16f6144e2d46edea1427e1222ea879377e029b0c5d2e252517aee85948ec7",
            ),
        });

        test_to_from_str_valid(TestInputValid {
            str: "bc1pw508d6qejxtdg4y5r3zarvary0c5xw7kw508d6qejxtdg4y5r3zarvary0c5xw7kt5nd6y",
            normalized:
                "bc1pw508d6qejxtdg4y5r3zarvary0c5xw7kw508d6qejxtdg4y5r3zarvary0c5xw7kt5nd6y",
            expected: segwit_addr(
                "bc",
                1,
                "751e76e8199196d454941c45d1b3a323f1433bd6751e76e8199196d454941c45d1b3a323f1433bd6",
            ),
        });

        test_to_from_str_valid(TestInputValid {
            str: "BC1SW50QGDZ25J",
            normalized: "bc1sw50qgdz25j",
            expected: segwit_addr("bc", 16, "751e"),
        });

        test_to_from_str_valid(TestInputValid {
            str: "bc1zw508d6qejxtdg4y5r3zarvaryvaxxpcs",
            normalized: "bc1zw508d6qejxtdg4y5r3zarvaryvaxxpcs",
            expected: segwit_addr("bc", 2, "751e76e8199196d454941c45d1b3a323"),
        });

        test_to_from_str_valid(TestInputValid {
            str: "bc1zw508d6qejxtdg4y5r3zarvaryvaxxpcs",
            normalized: "bc1zw508d6qejxtdg4y5r3zarvaryvaxxpcs",
            expected: segwit_addr("bc", 2, "751e76e8199196d454941c45d1b3a323"),
        });

        test_to_from_str_valid(TestInputValid {
            str: "tb1qqqqqp399et2xygdj5xreqhjjvcmzhxw4aywxecjdzew6hylgvsesrxh6hy",
            normalized: "tb1qqqqqp399et2xygdj5xreqhjjvcmzhxw4aywxecjdzew6hylgvsesrxh6hy",
            expected: segwit_addr(
                "tb",
                0,
                "000000c4a5cad46221b2a187905e5266362b99d5e91c6ce24d165dab93e86433",
            ),
        });

        test_to_from_str_valid(TestInputValid {
            str: "tb1pqqqqp399et2xygdj5xreqhjjvcmzhxw4aywxecjdzew6hylgvsesf3hn0c",
            normalized: "tb1pqqqqp399et2xygdj5xreqhjjvcmzhxw4aywxecjdzew6hylgvsesf3hn0c",
            expected: segwit_addr(
                "tb",
                1,
                "000000c4a5cad46221b2a187905e5266362b99d5e91c6ce24d165dab93e86433",
            ),
        });

        test_to_from_str_valid(TestInputValid {
            str: "bc1p0xlxvlhemja6c4dqv22uapctqupfhlxm9h8z3k2e72q4k9hcz7vqzk5jj0",
            normalized: "bc1p0xlxvlhemja6c4dqv22uapctqupfhlxm9h8z3k2e72q4k9hcz7vqzk5jj0",
            expected: segwit_addr(
                "bc",
                1,
                "79be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798",
            ),
        });
    }
}
