// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::bech32_prefix::Bech32Prefix;
use serde::{Deserialize, Deserializer, Serialize, Serializer};
use std::fmt;
use std::str::FromStr;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::CoinAddress;
use tw_coin_entry::error::{AddressError, AddressResult};
use tw_encoding::bech32;
use tw_hash::hasher::Hasher;
use tw_hash::H160;
use tw_keypair::tw::{PrivateKey, PublicKey, PublicKeyType};
use tw_memory::Data;

pub mod bech32_prefix;

#[derive(PartialEq)]
pub struct Bech32Address {
    hrp: String,
    key_hash: Data,
    /// An address string created from this `hrp` and `key_hash`.
    address_str: String,
}

impl Bech32Address {
    pub fn new(hrp: String, key_hash: Data) -> AddressResult<Bech32Address> {
        let address_str = bech32::encode(&hrp, &key_hash).map_err(|_| AddressError::InvalidHrp)?;
        Ok(Bech32Address {
            hrp,
            key_hash,
            address_str,
        })
    }

    pub fn with_public_key_hasher(
        hrp: String,
        public_key: &PublicKey,
        hasher: Hasher,
    ) -> AddressResult<Bech32Address> {
        let public_key_bytes = match public_key.public_key_type() {
            // If the public key is extended, skips the first byte (Evmos specific).
            // https://github.com/trustwallet/wallet-core/blob/d67078daa580b37063c97be66a625aaee9664882/src/Bech32Address.cpp#L61-L64
            PublicKeyType::Secp256k1Extended => public_key
                .to_secp256k1()
                .ok_or(AddressError::PublicKeyTypeMismatch)?
                .uncompressed_without_prefix()
                .to_vec(),
            _ => public_key.to_bytes(),
        };

        let public_key_hash = hasher.hash(&public_key_bytes);
        if public_key_hash.len() < H160::LEN {
            return Err(AddressError::UnexpectedHasher);
        }

        let (_skipped_bytes, key_bytes) =
            public_key_hash.split_at(public_key_hash.len() - H160::LEN);
        Bech32Address::new(hrp, key_bytes.to_vec())
    }

    pub fn with_public_key_coin_context(
        coin: &dyn CoinContext,
        public_key: &PublicKey,
        prefix: Option<Bech32Prefix>,
    ) -> AddressResult<Bech32Address> {
        let hrp = match prefix {
            Some(Bech32Prefix { hrp }) => hrp,
            None => coin.hrp().ok_or(AddressError::InvalidHrp)?,
        };
        let address_hasher = coin
            .address_hasher()
            .ok_or(AddressError::UnexpectedHasher)?;
        Self::with_public_key_hasher(hrp, public_key, address_hasher)
    }

    pub fn with_private_key_coin_context(
        coin: &dyn CoinContext,
        private_key: &PrivateKey,
    ) -> AddressResult<Bech32Address> {
        let hrp = coin.hrp().ok_or(AddressError::InvalidHrp)?;
        let public_key_type = coin.public_key_type();
        let public_key = private_key
            .get_public_key_by_type(public_key_type)
            .map_err(|_| AddressError::PublicKeyTypeMismatch)?;
        let address_hasher = coin
            .address_hasher()
            .ok_or(AddressError::UnexpectedHasher)?;
        Bech32Address::with_public_key_hasher(hrp, &public_key, address_hasher)
    }

    pub fn from_str_checked<I>(
        possible_hrps: I,
        address_str: String,
    ) -> AddressResult<Bech32Address>
    where
        I: IntoIterator<Item = String>,
    {
        let bech32::Decoded { hrp, bytes } =
            bech32::decode(&address_str).map_err(|_| AddressError::InvalidInput)?;
        // Try to find at least one hrp matches the actual value.
        // Copied from the legacy Bech32Address.cpp:
        // https://github.com/trustwallet/wallet-core/blob/d67078daa580b37063c97be66a625aaee9664882/src/Bech32Address.cpp#L21
        if !possible_hrps
            .into_iter()
            .any(|possible_hrp| hrp.starts_with(&possible_hrp))
        {
            return Err(AddressError::InvalidHrp);
        }
        Ok(Bech32Address {
            hrp,
            key_hash: bytes,
            address_str,
        })
    }

    pub fn from_str_with_coin_and_prefix(
        coin: &dyn CoinContext,
        address_str: String,
        prefix: Option<Bech32Prefix>,
    ) -> AddressResult<Bech32Address> {
        let hrp = match prefix {
            Some(Bech32Prefix { hrp }) => hrp,
            None => coin.hrp().ok_or(AddressError::InvalidHrp)?,
        };
        Self::from_str_checked([hrp], address_str)
    }

    pub fn from_key_hash_with_coin(
        coin: &dyn CoinContext,
        key_hash: Data,
    ) -> AddressResult<Bech32Address> {
        let hrp = coin.hrp().ok_or(AddressError::InvalidHrp)?;
        Bech32Address::new(hrp, key_hash)
    }

    pub fn key_hash(&self) -> &[u8] {
        &self.key_hash
    }

    pub fn hrp(&self) -> &str {
        &self.hrp
    }
}

impl CoinAddress for Bech32Address {
    fn data(&self) -> Data {
        self.key_hash.to_vec()
    }
}

impl FromStr for Bech32Address {
    type Err = AddressError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let bech32::Decoded { hrp, bytes } =
            bech32::decode(s).map_err(|_| AddressError::InvalidInput)?;
        Ok(Bech32Address {
            hrp,
            key_hash: bytes,
            address_str: s.to_string(),
        })
    }
}

impl fmt::Display for Bech32Address {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.address_str)
    }
}

impl fmt::Debug for Bech32Address {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{self}")
    }
}

impl<'de> Deserialize<'de> for Bech32Address {
    fn deserialize<D>(deserializer: D) -> Result<Self, D::Error>
    where
        D: Deserializer<'de>,
    {
        use serde::de::Error as DeError;

        let address_str = String::deserialize(deserializer)?;
        Bech32Address::from_str(&address_str).map_err(|e| DeError::custom(format!("{e:?}")))
    }
}

impl Serialize for Bech32Address {
    fn serialize<S>(&self, serializer: S) -> Result<S::Ok, S::Error>
    where
        S: Serializer,
    {
        serializer.serialize_str(&self.to_string())
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use tw_encoding::hex::{DecodeHex, ToHex};

    struct FromPublicKeyTestInput<'a> {
        hrp: &'a str,
        private_key: &'a str,
        public_key_type: PublicKeyType,
        hasher: Hasher,
        expected: &'a str,
    }

    fn test_from_public_key(input: FromPublicKeyTestInput<'_>) {
        let private_key = PrivateKey::new(input.private_key.decode_hex().unwrap()).unwrap();
        let public_key = private_key
            .get_public_key_by_type(input.public_key_type)
            .unwrap();
        let actual =
            Bech32Address::with_public_key_hasher(input.hrp.to_string(), &public_key, input.hasher)
                .unwrap();
        assert_eq!(actual.to_string(), input.expected);
    }

    #[test]
    fn test_address_from_str_checked_valid() {
        fn test_impl(addr: &str, hrp: &str) {
            Bech32Address::from_str_checked([hrp.to_string()], addr.to_string())
                .unwrap_or_else(|e| panic!("ERROR={:?}: hrp={} addr={}", e, hrp, addr));
        }

        test_impl("bnb1grpf0955h0ykzq3ar5nmum7y6gdfl6lxfn46h2", "bnb");

        test_impl("cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02", "cosmos");
        test_impl(
            "cosmospub1addwnpepqftjsmkr7d7nx4tmhw4qqze8w39vjq364xt8etn45xqarlu3l2wu2n7pgrq",
            "cosmos",
        );
        test_impl(
            "cosmosvaloper1sxx9mszve0gaedz5ld7qdkjkfv8z992ax69k08",
            "cosmos",
        );
        test_impl(
            "cosmosvalconspub1zcjduepqjnnwe2jsywv0kfc97pz04zkm7tc9k2437cde2my3y5js9t7cw9mstfg3sa",
            "cosmos",
        );

        test_impl("one1a50tun737ulcvwy0yvve0pvu5skq0kjargvhwe", "one");
        test_impl("one1tp7xdd9ewwnmyvws96au0e7e7mz6f8hjqr3g3p", "one");

        test_impl("io187wzp08vnhjjpkydnr97qlh8kh0dpkkytfam8j", "io");

        test_impl("zil1fwh4ltdguhde9s7nysnp33d5wye6uqpugufkz7", "zil");

        test_impl(
            "erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz",
            "erd",
        );
        test_impl(
            "erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r",
            "erd",
        );
        test_impl(
            "erd19nu5t7hszckwah5nlcadmk5rlchtugzplznskffpwecygcu0520s9tnyy0",
            "erd",
        );

        // uppercase version
        test_impl("BNB1GRPF0955H0YKZQ3AR5NMUM7Y6GDFL6LXFN46H2", "bnb");
    }

    #[test]
    fn test_address_from_str_checked_invalid() {
        fn test_impl(addr: &str, hrp: &str) {
            Bech32Address::from_str_checked([hrp.to_string()], addr.to_string())
                .expect_err(&format!("hrp={} addr={}", hrp, addr));
        }

        // 1-char diff
        test_impl("bnb1grpf0955h0ykzq3ar6nmum7y6gdfl6lxfn46h2", "bnb");
        // mixed case
        test_impl("bnb1grPF0955h0ykzq3ar5nmum7y6gdfl6lxfn46h2", "bnb");

        test_impl("cosmos1xsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02", "cosmos");
        test_impl(
            "cosmospub1xddwnpepqftjsmkr7d7nx4tmhw4qqze8w39vjq364xt8etn45xqarlu3l2wu2n7pgrq",
            "cosmos",
        );
        test_impl(
            "cosmosvaloper1xxx9mszve0gaedz5ld7qdkjkfv8z992ax69k08",
            "cosmos",
        );
        test_impl(
            "cosmosvalconspub1xcjduepqjnnwe2jsywv0kfc97pz04zkm7tc9k2437cde2my3y5js9t7cw9mstfg3sa",
            "cosmos",
        );

        test_impl("one1a50tun737ulcvwy0yvve0pe", "one");
        test_impl("oe1tp7xdd9ewwnmyvws96au0ee7e7mz6f8hjqr3g3p", "one");

        test_impl("io187wzp08vnhjjpkydnr97qlh8kh0dpkkytfam8", "io");
        test_impl("io187wzp08vnhjpkydnr97qlh8kh0dpkkytfam8j", "io");
        test_impl("it187wzp08vnhjjpkydnr97qlh8kh0dpkkytfam8j", "io");

        test_impl("", "erd");
        test_impl(
            "erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35!",
            "erd",
        );
        test_impl(
            "xerd19nu5t7hszckwah5nlcadmk5rlchtugzplznskffpwecygcu0520s9tnyy0",
            "erd",
        );
    }

    #[test]
    fn test_decode() {
        fn test_impl(addr: &str, hrp: &str, expected_hash: &str) {
            let actual =
                Bech32Address::from_str_checked([hrp.to_string()], addr.to_string()).unwrap();
            assert_eq!(actual.key_hash.to_hex(), expected_hash);
        }

        test_impl(
            "one1a50tun737ulcvwy0yvve0pvu5skq0kjargvhwe",
            "one",
            "ed1ebe4fd1f73f86388f231997859ca42c07da5d",
        );

        test_impl(
            "io187wzp08vnhjjpkydnr97qlh8kh0dpkkytfam8j",
            "io",
            "3f9c20bcec9de520d88d98cbe07ee7b5ded0dac4",
        );
    }

    #[test]
    fn test_from_hrp_and_hash() {
        fn test_impl(hrp: &str, key_hash: &str, expected_addr: &str) {
            let actual =
                Bech32Address::new(hrp.to_string(), key_hash.decode_hex().unwrap()).unwrap();
            assert_eq!(actual.to_string(), expected_addr);
        }

        test_impl(
            "bnb",
            "b6561dcc104130059a7c08f48c64610c1f6f9064",
            "bnb1ketpmnqsgycqtxnupr6gcerpps0klyryuudz05",
        );
        test_impl(
            "one",
            "587c66b4b973a7b231d02ebbc7e7d9f6c5a49ef2",
            "one1tp7xdd9ewwnmyvws96au0e7e7mz6f8hjqr3g3p",
        );
        test_impl(
            "zil",
            "0x91cdDcEBE846ce4d47712287EEe53cF17c2cfB77",
            "zil1j8xae6lggm8y63m3y2r7aefu797ze7mhzulnqg",
        );
        test_impl(
            "zil",
            "1d19918a737306218b5cbb3241fcdcbd998c3a72",
            "zil1r5verznnwvrzrz6uhveyrlxuhkvccwnju4aehf",
        );
    }

    #[test]
    fn test_from_hrp_and_public_key_hasher() {
        test_from_public_key(FromPublicKeyTestInput {
            hrp: "bnb",
            private_key: "95949f757db1f57ca94a5dff23314accbe7abee89597bf6a3c7382c84d7eb832",
            public_key_type: PublicKeyType::Secp256k1,
            hasher: Hasher::Sha256ripemd,
            expected: "bnb1grpf0955h0ykzq3ar5nmum7y6gdfl6lxfn46h2",
        });

        test_from_public_key(FromPublicKeyTestInput {
            hrp: "cosmos",
            private_key: "80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005",
            public_key_type: PublicKeyType::Secp256k1,
            hasher: Hasher::Sha256ripemd,
            expected: "cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02",
        });

        test_from_public_key(FromPublicKeyTestInput {
            hrp: "one",
            private_key: "e2f88b4974ae763ca1c2db49218802c2e441293a09eaa9ab681779e05d1b7b94",
            public_key_type: PublicKeyType::Secp256k1Extended,
            hasher: Hasher::Keccak256,
            expected: "one1a50tun737ulcvwy0yvve0pvu5skq0kjargvhwe",
        });

        test_from_public_key(FromPublicKeyTestInput {
            hrp: "io",
            private_key: "0806c458b262edd333a191e92f561aff338211ee3e18ab315a074a2d82aa343f",
            public_key_type: PublicKeyType::Secp256k1Extended,
            hasher: Hasher::Keccak256,
            expected: "io187wzp08vnhjjpkydnr97qlh8kh0dpkkytfam8j",
        });

        test_from_public_key(FromPublicKeyTestInput {
            hrp: "zil",
            private_key: "3382266517e2ebe6df51faf4bfe612236ad46fb8bd59ac982a223b045e080ac6",
            public_key_type: PublicKeyType::Secp256k1,
            hasher: Hasher::Sha256,
            expected: "zil1j8xae6lggm8y63m3y2r7aefu797ze7mhzulnqg",
        });
    }

    /// From same public key, but different hashes: different results.
    #[test]
    fn test_different_hashes() {
        test_from_public_key(FromPublicKeyTestInput {
            hrp: "hrp",
            private_key: "3382266517e2ebe6df51faf4bfe612236ad46fb8bd59ac982a223b045e080ac6",
            public_key_type: PublicKeyType::Secp256k1,
            hasher: Hasher::Sha256ripemd,
            expected: "hrp186zwn9h0z9fyvwfqs4jl92cw3kexusm4xw6ptp",
        });

        test_from_public_key(FromPublicKeyTestInput {
            hrp: "hrp",
            private_key: "3382266517e2ebe6df51faf4bfe612236ad46fb8bd59ac982a223b045e080ac6",
            public_key_type: PublicKeyType::Secp256k1,
            hasher: Hasher::Sha256,
            expected: "hrp1j8xae6lggm8y63m3y2r7aefu797ze7mhgfetvu",
        });

        test_from_public_key(FromPublicKeyTestInput {
            hrp: "hrp",
            private_key: "3382266517e2ebe6df51faf4bfe612236ad46fb8bd59ac982a223b045e080ac6",
            public_key_type: PublicKeyType::Secp256k1Extended,
            hasher: Hasher::Keccak256,
            expected: "hrp17hff3s97m5uxpjcdq3nzqxxatt8cmumnsf03su",
        });
    }

    /// From same public key, but different prefixes: different results (checksum).
    #[test]
    fn test_different_hrp() {
        test_from_public_key(FromPublicKeyTestInput {
            hrp: "hrpone",
            private_key: "3382266517e2ebe6df51faf4bfe612236ad46fb8bd59ac982a223b045e080ac6",
            public_key_type: PublicKeyType::Secp256k1,
            hasher: Hasher::Sha256ripemd,
            expected: "hrpone186zwn9h0z9fyvwfqs4jl92cw3kexusm47das6p",
        });

        test_from_public_key(FromPublicKeyTestInput {
            hrp: "hrptwo",
            private_key: "3382266517e2ebe6df51faf4bfe612236ad46fb8bd59ac982a223b045e080ac6",
            public_key_type: PublicKeyType::Secp256k1,
            hasher: Hasher::Sha256ripemd,
            expected: "hrptwo186zwn9h0z9fyvwfqs4jl92cw3kexusm4qzr8p7",
        });

        test_from_public_key(FromPublicKeyTestInput {
            hrp: "hrpthree",
            private_key: "3382266517e2ebe6df51faf4bfe612236ad46fb8bd59ac982a223b045e080ac6",
            public_key_type: PublicKeyType::Secp256k1,
            hasher: Hasher::Sha256ripemd,
            expected: "hrpthree186zwn9h0z9fyvwfqs4jl92cw3kexusm4wuqkvd",
        });
    }
}
