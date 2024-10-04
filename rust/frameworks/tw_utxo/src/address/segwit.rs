// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use super::Bech32Prefix;
use crate::address::witness_program::{WitnessProgram, WITNESS_V0};
use crate::script::standard_script::conditions;
use crate::script::Script;
use core::fmt;
use std::str::FromStr;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::prelude::*;
use tw_hash::hasher::sha256_ripemd;
use tw_hash::sha2::sha256;
use tw_hash::{H160, H256};
use tw_keypair::tw;
use tw_memory::Data;

/// Witness program sizes valid for V0 (Segwit).
const WITNESS_V0_VALID_PROGRAM_SIZES: [usize; 2] = [H160::LEN, H256::LEN];

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct SegwitAddress {
    inner: WitnessProgram,
}

impl SegwitAddress {
    pub fn new(hrp: String, witness_program: Data) -> AddressResult<SegwitAddress> {
        // Specific segwit v0 check. These addresses can never spend funds sent to them.
        if !WITNESS_V0_VALID_PROGRAM_SIZES.contains(&witness_program.len()) {
            return Err(AddressError::InvalidInput);
        }

        let inner = WitnessProgram::new(hrp, WITNESS_V0, witness_program, bech32::Variant::Bech32)?;
        Ok(SegwitAddress { inner })
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

        Self::new(hrp, public_key_hash.to_vec())
    }

    pub fn p2wsh_with_hrp(redeem_script: &Script, hrp: String) -> AddressResult<SegwitAddress> {
        let script_hash = sha256(redeem_script.as_slice());
        Self::new(hrp, script_hash)
    }

    pub fn from_str_checked(s: &str, expected_hrp: &str) -> AddressResult<SegwitAddress> {
        let address = Self::from_str(s)?;
        if address.inner.hrp() != expected_hrp {
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

    pub fn witness_program(&self) -> &[u8] {
        self.inner.witness_program()
    }

    pub fn to_script_pubkey(&self) -> SigningResult<Script> {
        let witness_program = self.witness_program();
        match witness_program.len() {
            // P2WPKH
            H160::LEN => {
                let pubkey_hash = H160::try_from(witness_program)
                    .expect("'witness_program' length must be checked already");
                Ok(conditions::new_p2wpkh(&pubkey_hash))
            },
            // P2WSH
            H256::LEN => {
                let script_hash = H256::try_from(witness_program)
                    .expect("'witness_program' length must be checked already");
                Ok(conditions::new_p2wsh(&script_hash))
            },
            // Unknown
            _ => SigningError::err(SigningErrorType::Error_invalid_address).context(format!(
                "The given '{self}' segwit address should have 20 or 32 byte length witness program"
            )),
        }
    }
}

impl FromStr for SegwitAddress {
    type Err = AddressError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let inner = WitnessProgram::from_str_checked(
            s,
            WITNESS_V0,
            bech32::Variant::Bech32,
            &WITNESS_V0_VALID_PROGRAM_SIZES,
        )?;
        Ok(SegwitAddress { inner })
    }
}

impl fmt::Display for SegwitAddress {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.inner)
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

    #[track_caller]
    fn segwit_addr(hrp: &str, program: &str) -> SegwitAddress {
        SegwitAddress::new(hrp.to_string(), program.decode_hex().unwrap())
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

    #[track_caller]
    fn test_from_str_invalid(str: &str) {
        let _ = SegwitAddress::from_str(str).expect_err("Expected an invalid Segwit address");
    }

    #[test]
    fn test_segwit_address_to_from_str() {
        test_to_from_str_valid(TestInputValid {
            str: "BC1QW508D6QEJXTDG4Y5R3ZARVARY0C5XW7KV8F3T4",
            normalized: "bc1qw508d6qejxtdg4y5r3zarvary0c5xw7kv8f3t4",
            expected: segwit_addr("bc", "751e76e8199196d454941c45d1b3a323f1433bd6"),
        });

        test_to_from_str_valid(TestInputValid {
            str: "tb1qrp33g0q5c5txsp9arysrx4k6zdkfs4nce4xj0gdcccefvpysxf3q0sl5k7",
            normalized: "tb1qrp33g0q5c5txsp9arysrx4k6zdkfs4nce4xj0gdcccefvpysxf3q0sl5k7",
            expected: segwit_addr(
                "tb",
                "1863143c14c5166804bd19203356da136c985678cd4d27a1b8c6329604903262",
            ),
        });

        test_to_from_str_valid(TestInputValid {
            str: "tb1qqqqqp399et2xygdj5xreqhjjvcmzhxw4aywxecjdzew6hylgvsesrxh6hy",
            normalized: "tb1qqqqqp399et2xygdj5xreqhjjvcmzhxw4aywxecjdzew6hylgvsesrxh6hy",
            expected: segwit_addr(
                "tb",
                "000000c4a5cad46221b2a187905e5266362b99d5e91c6ce24d165dab93e86433",
            ),
        });

        test_to_from_str_valid(TestInputValid {
            str: "bc1qpjult34k9spjfym8hss2jrwjgf0xjf40ze0pp8",
            normalized: "bc1qpjult34k9spjfym8hss2jrwjgf0xjf40ze0pp8",
            expected: segwit_addr("bc", "0cb9f5c6b62c03249367bc20a90dd2425e6926af"),
        });

        test_to_from_str_valid(TestInputValid {
            str: "bc1qm9jzmujvdqjj6y28hptk859zs3yyv78hpqqjfj",
            normalized: "bc1qm9jzmujvdqjj6y28hptk859zs3yyv78hpqqjfj",
            expected: segwit_addr("bc", "d9642df24c68252d1147b85763d0a284484678f7"),
        });

        test_to_from_str_valid(TestInputValid {
            str: "tb1qqqqqp399et2xygdj5xreqhjjvcmzhxw4aywxecjdzew6hylgvsesrxh6hy",
            normalized: "tb1qqqqqp399et2xygdj5xreqhjjvcmzhxw4aywxecjdzew6hylgvsesrxh6hy",
            expected: segwit_addr(
                "tb",
                "000000c4a5cad46221b2a187905e5266362b99d5e91c6ce24d165dab93e86433",
            ),
        });
    }

    #[test]
    fn test_segwit_address_from_str_invalid() {
        // witness program size 38
        test_from_str_invalid(
            "bc1q0xlxvlhemja6c4dqv22uapctqupfhlxm0xlxvlhemja6c4dqv22uapctqupfkpvgusg",
        );

        // version 1
        test_from_str_invalid("bc1ptmsk7c2yut2xah4pgflpygh2s7fh0cpfkrza9cjj29awapv53mrslgd5cf");

        // version 1
        test_from_str_invalid(
            "bc1pw508d6qejxtdg4y5r3zarvary0c5xw7kw508d6qejxtdg4y5r3zarvary0c5xw7kt5nd6y",
        );

        // version 2
        test_from_str_invalid("bc1zw508d6qejxtdg4y5r3zarvaryvaxxpcs");

        // version 16
        test_from_str_invalid("BC1SW50QGDZ25J");
    }
}
