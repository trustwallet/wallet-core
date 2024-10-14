use super::Bech32Prefix;
use crate::address::witness_program::WitnessProgram;
use crate::script::standard_script::conditions;
use crate::script::Script;
use bitcoin::key::TapTweak;
use core::fmt;
use std::str::FromStr;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::prelude::*;
use tw_hash::{H256, H264};
use tw_keypair::tw;
use tw_memory::Data;
use tw_misc::traits::ToBytesVec;

/// cbindgen:ignore
pub const WITNESS_V1: u8 = 1;
/// Witness program sizes valid for V1 (Taproot).
/// cbindgen:ignore
pub const WITNESS_V1_VALID_PROGRAM_SIZES: [usize; 1] = [H256::LEN];

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct TaprootAddress {
    inner: WitnessProgram,
}

impl TaprootAddress {
    pub fn new(hrp: String, witness_program: Data) -> AddressResult<TaprootAddress> {
        // Specific Taproot V1 check. These addresses can never spend funds sent to them.
        if !WITNESS_V1_VALID_PROGRAM_SIZES.contains(&witness_program.len()) {
            return Err(AddressError::InvalidInput);
        }

        let inner =
            WitnessProgram::new(hrp, WITNESS_V1, witness_program, bech32::Variant::Bech32m)?;
        Ok(TaprootAddress { inner })
    }

    /// Create a Taproot address from a public key and an optional merkle root.
    /// Taproot transactions come in two variants:
    ///
    /// * P2TR key-path: which is used for "normal" balance transfers and is
    ///   internally _tweaked_ with an empty (None) merkle root.
    /// * P2TR script-path: which is used for complex scripts, such as
    ///   Ordinals/BRC20, and is internally _tweaked_ with a merkle root of all
    ///   possible spending conditions.
    pub fn p2tr_with_public_key(
        hrp: String,
        internal_pubkey: &H264,
        merkle_root: Option<&H256>,
    ) -> AddressResult<TaprootAddress> {
        // We're relying on the `bitcoin` crate to generate anything Taproot related.

        // Convert the native `H256` to `TapNodeHash` from the `bitcoin` crate.
        let merkle_root = merkle_root.map(|hash| {
            let tap_hash =
                <bitcoin::hashes::sha256t::Hash<_> as bitcoin::hashes::Hash>::from_slice(
                    hash.as_slice(),
                )
                .expect("merkle_root length is 32 bytes");

            bitcoin::taproot::TapNodeHash::from_raw_hash(tap_hash)
        });

        // Tweak the public key with the (empty) merkle root.
        let pubkey = bitcoin::PublicKey::from_slice(internal_pubkey.as_slice()).unwrap();
        let internal_key = bitcoin::secp256k1::XOnlyPublicKey::from(pubkey.inner);
        let (output_key, _parity) =
            internal_key.tap_tweak(&bitcoin::secp256k1::Secp256k1::new(), merkle_root);

        Self::new(hrp, output_key.serialize().to_vec())
    }

    /// Create a Taproot address from a public key and an optional merkle root.
    /// Taproot transactions come in two variants:
    ///
    /// * P2TR key-path: which is used for "normal" balance transfers and is
    ///   internally _tweaked_ with an empty (None) merkle root.
    /// * P2TR script-path: which is used for complex scripts, such as
    ///   Ordinals/BRC20, and is internally _tweaked_ with a merkle root of all
    ///   possible spending conditions.
    pub fn p2tr_with_coin_and_prefix(
        coin: &dyn CoinContext,
        public_key: &tw::PublicKey,
        prefix: Option<Bech32Prefix>,
        merkle_root: Option<&H256>,
    ) -> AddressResult<TaprootAddress> {
        let hrp = match prefix {
            Some(Bech32Prefix { hrp }) => hrp,
            None => coin.hrp().ok_or(AddressError::InvalidRegistry)?,
        };

        let public_key_bytes = public_key
            .to_secp256k1()
            .ok_or(AddressError::PublicKeyTypeMismatch)?
            .compressed();

        Self::p2tr_with_public_key(hrp, &public_key_bytes, merkle_root)
    }

    pub fn from_str_checked(s: &str, expected_hrp: &str) -> AddressResult<TaprootAddress> {
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
    ) -> AddressResult<TaprootAddress> {
        let hrp = match prefix {
            Some(Bech32Prefix { hrp }) => hrp,
            None => coin.hrp().ok_or(AddressError::InvalidRegistry)?,
        };
        TaprootAddress::from_str_checked(s, &hrp)
    }

    pub fn witness_program(&self) -> &[u8] {
        self.inner.witness_program()
    }

    pub fn to_script_pubkey(&self) -> SigningResult<Script> {
        let tweaked_pubkey_hash = H256::try_from(self.witness_program())
            .tw_err(|_| SigningErrorType::Error_invalid_address)
            .context("P2TR 'witness_program' should be 32-bytes array")?;
        Ok(conditions::new_p2tr_dangerous_assume_tweaked(
            &tweaked_pubkey_hash,
        ))
    }
}

impl FromStr for TaprootAddress {
    type Err = AddressError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let inner = WitnessProgram::from_str_checked(
            s,
            WITNESS_V1,
            bech32::Variant::Bech32m,
            &WITNESS_V1_VALID_PROGRAM_SIZES,
        )?;
        Ok(TaprootAddress { inner })
    }
}

impl fmt::Display for TaprootAddress {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.inner)
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use tw_coin_entry::test_utils::test_context::TestCoinContext;
    use tw_encoding::hex::DecodeHex;

    struct TestInputValid {
        str: &'static str,
        normalized: &'static str,
        expected: TaprootAddress,
    }

    #[track_caller]
    fn taproot_addr(hrp: &str, program: &str) -> TaprootAddress {
        TaprootAddress::new(hrp.to_string(), program.decode_hex().unwrap())
            .expect("Cannot construct a TaprootAddress from the input")
    }

    /// Tests if the given `s` string representation is converted from and to `expected` segwit address.
    #[track_caller]
    fn test_to_from_str_valid(input: TestInputValid) {
        let actual = TaprootAddress::from_str(input.str).expect("Expected a valid address");
        assert_eq!(actual, input.expected, "String -> TaprootAddress");

        let actual_str = actual.to_string();
        assert_eq!(actual_str, input.normalized, "TaprootAddress -> String");
    }

    #[track_caller]
    fn test_from_str_invalid(str: &str) {
        let _ = TaprootAddress::from_str(str).expect_err("Expected an invalid Taproot address");
    }

    #[test]
    fn test_segwit_address_to_from_str() {
        test_to_from_str_valid(TestInputValid {
            str: "bc1ptmsk7c2yut2xah4pgflpygh2s7fh0cpfkrza9cjj29awapv53mrslgd5cf",
            normalized: "bc1ptmsk7c2yut2xah4pgflpygh2s7fh0cpfkrza9cjj29awapv53mrslgd5cf",
            expected: taproot_addr(
                "bc",
                "5ee16f6144e2d46edea1427e1222ea879377e029b0c5d2e252517aee85948ec7",
            ),
        });

        test_to_from_str_valid(TestInputValid {
            str: "tb1pqqqqp399et2xygdj5xreqhjjvcmzhxw4aywxecjdzew6hylgvsesf3hn0c",
            normalized: "tb1pqqqqp399et2xygdj5xreqhjjvcmzhxw4aywxecjdzew6hylgvsesf3hn0c",
            expected: taproot_addr(
                "tb",
                "000000c4a5cad46221b2a187905e5266362b99d5e91c6ce24d165dab93e86433",
            ),
        });

        test_to_from_str_valid(TestInputValid {
            str: "bc1p0xlxvlhemja6c4dqv22uapctqupfhlxm9h8z3k2e72q4k9hcz7vqzk5jj0",
            normalized: "bc1p0xlxvlhemja6c4dqv22uapctqupfhlxm9h8z3k2e72q4k9hcz7vqzk5jj0",
            expected: taproot_addr(
                "bc",
                "79be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798",
            ),
        });
    }

    #[test]
    fn test_taproot_address_from_str_invalid() {
        // version 0
        test_from_str_invalid("tb1qqqqqp399et2xygdj5xreqhjjvcmzhxw4aywxecjdzew6hylgvsesrxh6hy");

        // version 2
        test_from_str_invalid("bc1zw508d6qejxtdg4y5r3zarvaryvaxxpcs");

        // version 2
        test_from_str_invalid("bc1zw508d6qejxtdg4y5r3zarvaryvaxxpcs");

        // version 16
        test_from_str_invalid("BC1SW50QGDZ25J");

        // program size 40
        test_from_str_invalid(
            "bc1pw508d6qejxtdg4y5r3zarvary0c5xw7kw508d6qejxtdg4y5r3zarvary0c5xw7kt5nd6y",
        );
    }

    #[test]
    fn test_taproot_address_create_with_coin_and_prefix() {
        let coin = TestCoinContext::default();
        let hrp = "bc".to_string();
        let merkle_root = None;

        let public_key_bytes = "03cdf7e208a0146c3a35c181944a96a15b2a58256be69adad640a9a97d408b9b44"
            .decode_hex()
            .unwrap();
        let public_key =
            tw::PublicKey::new(public_key_bytes.clone(), tw::PublicKeyType::Secp256k1).unwrap();

        let addr = TaprootAddress::p2tr_with_coin_and_prefix(
            &coin,
            &public_key,
            Some(Bech32Prefix { hrp }),
            merkle_root,
        )
        .unwrap();
        assert_eq!(
            addr.to_string(),
            "bc1purekytqrqzfzdulufmll8a335jhvw9x4glhzp8fv76yxlsxeyptsfylq9h"
        );
    }
}
