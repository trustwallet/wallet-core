// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use serde::de::Error as SerdeError;
use serde::{Deserialize, Deserializer, Serialize, Serializer};
use std::fmt::{Display, Formatter};
use std::ops::{RangeFrom, RangeInclusive};
use std::str::FromStr;
use tw_coin_entry::coin_entry::CoinAddress;
use tw_coin_entry::error::prelude::*;
use tw_encoding::hex;
use tw_hash::sha2::sha256;
use tw_hash::{sha3::keccak256, H160, H256};
use tw_keypair::ecdsa::secp256k1;
use tw_memory::Data;

use crate::abi::encode;
use crate::abi::prebuild::erc1967::EIP_1967_PROXY_BYTE_CODE_HEX;
use crate::abi::token::Token;

pub trait EvmAddress: FromStr<Err = AddressError> + Into<Address> {
    /// Tries to parse an address from the string representation.
    /// Returns `Ok(None)` if the given `s` string is empty.
    #[inline]
    fn from_str_optional(s: &str) -> AddressResult<Option<Self>> {
        if s.is_empty() {
            return Ok(None);
        }

        Self::from_str(s).map(Some)
    }
}

/// Represents an Ethereum address.
#[derive(Clone, Copy, Debug, Default, PartialEq)]
pub struct Address {
    bytes: H160,
}

/// cbindgen:ignore
impl Address {
    pub const LEN: usize = 20;

    /// Initializes an address with a `secp256k1` public key.
    pub fn with_secp256k1_pubkey(pubkey: &secp256k1::PublicKey) -> Address {
        /// `keccak256` returns 32 bytes, but Ethereum address is the last 20 bytes of the hash.
        const ADDRESS_HASH_STARTS_AT: usize = H256::len() - H160::len();
        const ADDRESS_HASH_RANGE: RangeFrom<usize> = ADDRESS_HASH_STARTS_AT..;

        let pubkey_bytes = pubkey.uncompressed_without_prefix();
        let hash = keccak256(pubkey_bytes.as_slice());
        assert_eq!(hash.len(), H256::len());

        let bytes = H160::try_from(&hash[ADDRESS_HASH_RANGE]).expect("Expected 20 byte array");

        Address { bytes }
    }

    /// Constructs an address from the 20-length byte array.
    pub fn from_bytes(bytes: H160) -> Address {
        Address { bytes }
    }

    /// Displays the address in mixed-case checksum form
    /// https://github.com/ethereum/EIPs/blob/master/EIPS/eip-55.md
    pub fn into_checksum_address(self) -> String {
        const UPPER_RANGE_1: RangeInclusive<char> = '8'..='9';
        const UPPER_RANGE_2: RangeInclusive<char> = 'a'..='f';

        let prefixed = false;
        let addr_hex = hex::encode(self.bytes, prefixed);
        let addr_hash = hex::encode(keccak256(addr_hex.as_bytes()), prefixed);

        let payload_chars = addr_hex.chars().zip(addr_hash.chars()).map(|(a, h)| {
            if a.is_ascii_digit() {
                a
            } else if UPPER_RANGE_1.contains(&h) || UPPER_RANGE_2.contains(&h) {
                a.to_ascii_uppercase()
            } else {
                a.to_ascii_lowercase()
            }
        });

        "0x".chars().chain(payload_chars).collect()
    }

    /// Returns the account path from the address according to EIP-2645
    /// https://docs.starkware.co/starkex/key-derivation.html
    pub fn account_path(
        &self,
        layer: &str,
        application: &str,
        index: &str,
    ) -> Result<String, AddressError> {
        let layer_bits = hex::data_to_bits(sha256(layer.as_bytes()).as_slice())?;
        let layer_hash = hex::bits_to_u32(&layer_bits, 256 - 31, 256)?;
        let application_bits = hex::data_to_bits(sha256(application.as_bytes()).as_slice())?;
        let application_hash = hex::bits_to_u32(&application_bits, 256 - 31, 256)?;
        let addr_hex = hex::encode(self.bytes, false);

        let addr_bits = hex::hex_to_bits(&addr_hex)?;
        let addr1 = u32::from_str_radix(&addr_bits[addr_bits.len() - 31..], 2)
            .map_err(|_| AddressError::InvalidInput)?;
        let addr2 = u32::from_str_radix(&addr_bits[addr_bits.len() - 62..addr_bits.len() - 31], 2)
            .map_err(|_| AddressError::InvalidInput)?;

        Ok(format!(
            "m/2645'/{layer_hash}'/{application_hash}'/{addr1}'/{addr2}'/{index}"
        ))
    }

    pub fn eip1014_create2_address(
        from: &str,
        salt: &H256,
        init_code_hash: &H256,
    ) -> Result<Address, AddressError> {
        let from = Address::from_str(from)?;
        let mut address = vec![255];
        address.extend_from_slice(from.as_slice());
        address.extend_from_slice(salt.as_slice());
        address.extend_from_slice(init_code_hash.as_slice());
        let address = keccak256(&address);
        let mut address_bytes = [0u8; 20];
        address_bytes.copy_from_slice(&address[address.len() - 20..]);
        Ok(Address::from_bytes(address_bytes.into()))
    }

    pub fn eip_1967_proxy_init_code(
        logic_address: &str,
        data: &[u8],
    ) -> Result<Vec<u8>, AddressError> {
        let logic_address = Address::from_str(logic_address)?;

        let tokens = [Token::Address(logic_address), Token::Bytes(data.to_vec())];

        let mut encoded = hex::decode(EIP_1967_PROXY_BYTE_CODE_HEX).expect("Expected valid hex");
        encoded.extend_from_slice(&encode::encode_tokens(&tokens));
        Ok(encoded)
    }

    /// Returns bytes of the address.
    #[inline]
    pub fn bytes(&self) -> H160 {
        self.bytes
    }

    /// Returns bytes as a slice of the address.
    #[inline]
    pub fn as_slice(&self) -> &[u8] {
        self.bytes.as_slice()
    }
}

impl CoinAddress for Address {
    #[inline]
    fn data(&self) -> Data {
        self.bytes.to_vec()
    }
}

impl FromStr for Address {
    type Err = AddressError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let addr_hex = s.strip_prefix("0x").ok_or(AddressError::MissingPrefix)?;
        let addr_hash = H160::from_str(addr_hex).map_err(|_| AddressError::FromHexError)?;
        Ok(Address { bytes: addr_hash })
    }
}

impl<'a> TryFrom<&'a [u8]> for Address {
    type Error = AddressError;

    fn try_from(bytes: &'a [u8]) -> Result<Self, Self::Error> {
        let bytes = H160::try_from(bytes).map_err(|_| AddressError::InvalidInput)?;
        Ok(Address { bytes })
    }
}

impl EvmAddress for Address {}

/// Implement `str` -> `PrivateKey` conversion for test purposes.
impl From<&'static str> for Address {
    #[inline]
    fn from(addr: &'static str) -> Self {
        Address::from_str(addr).expect("Expected a valid Ethereum address")
    }
}

impl Display for Address {
    #[inline]
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}", self.into_checksum_address())
    }
}

impl<'de> Deserialize<'de> for Address {
    fn deserialize<D>(deserializer: D) -> Result<Self, D::Error>
    where
        D: Deserializer<'de>,
    {
        let s: &str = Deserialize::deserialize(deserializer)?;
        Address::from_str(s).map_err(|e| SerdeError::custom(format!("{e:?}")))
    }
}

impl Serialize for Address {
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
    use tw_keypair::ecdsa::secp256k1::PrivateKey;

    struct Eip55Test {
        test: &'static str,
        mixedcase: &'static str,
    }

    #[test]
    fn test_to_from_string() {
        let tests = [
            Eip55Test {
                test: "0x5aaeb6053f3e94c9b9a09f33669435e7ef1beaed",
                mixedcase: "0x5aAeb6053F3E94C9b9A09f33669435E7Ef1BeAed",
            },
            Eip55Test {
                test: "0x5AAEB6053F3E94C9b9A09f33669435E7Ef1BEAED",
                mixedcase: "0x5aAeb6053F3E94C9b9A09f33669435E7Ef1BeAed",
            },
            Eip55Test {
                test: "0xfB6916095ca1df60bB79Ce92cE3Ea74c37c5d359",
                mixedcase: "0xfB6916095ca1df60bB79Ce92cE3Ea74c37c5d359",
            },
            Eip55Test {
                test: "0xdbF03B407c01E7cD3CBea99509d93f8DDDC8C6FB",
                mixedcase: "0xdbF03B407c01E7cD3CBea99509d93f8DDDC8C6FB",
            },
            Eip55Test {
                test: "0xD1220A0cf47c7B9Be7A2E6BA89F429762e7b9aDb",
                mixedcase: "0xD1220A0cf47c7B9Be7A2E6BA89F429762e7b9aDb",
            },
        ];

        for test in tests {
            let addr: Address = test.test.parse().unwrap();

            let expected_payload = H160::from(test.test);
            assert_eq!(addr.bytes, expected_payload);

            let mixed = addr.to_string();
            assert_eq!(mixed, test.mixedcase);
        }
    }

    #[test]
    fn test_from_string_invalid() {
        let tests = [
            "abc",
            "0xabc",
            "0xaaeb60f3e94c9b9a09f33669435e7ef1beaed",
            "fB6916095ca1df60bB79Ce92cE3Ea74c37c5d359",
        ];
        for test in tests {
            Address::from_str(test).unwrap_err();
        }
    }

    #[test]
    fn test_from_public_key() {
        let private = PrivateKey::try_from(
            "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5",
        )
        .unwrap();
        let public = private.public();
        let addr = Address::with_secp256k1_pubkey(&public);
        assert_eq!(
            addr.to_string(),
            "0xAc1ec44E4f0ca7D172B7803f6836De87Fb72b309"
        );
    }

    #[test]
    fn test_account_path() {
        // https://github.com/immutable/imx-core-sdk-swift/blob/main/Tests/ImmutableXCoreTests/Crypto/Stark/StarkKeyTests.swift#L30
        let addr: Address = "0xa76e3eeb2f7143165618ab8feaabcd395b6fac7f"
            .parse()
            .unwrap();
        let path = addr.account_path("starkex", "immutablex", "1").unwrap();
        assert_eq!(
            path,
            "m/2645'/579218131'/211006541'/1534045311'/1431804530'/1"
        );
    }

    #[test]
    fn test_eip1014_create2_address_example0() {
        let from = "0x0000000000000000000000000000000000000000";
        let salt = H256::try_from(
            hex::decode("0x0000000000000000000000000000000000000000000000000000000000000000")
                .unwrap()
                .as_slice(),
        )
        .unwrap();
        let init_code_hash =
            H256::try_from(keccak256(hex::decode("0x00").unwrap().as_slice()).as_slice()).unwrap();
        let address = Address::eip1014_create2_address(from, &salt, &init_code_hash).unwrap();
        assert_eq!(
            address.into_checksum_address(),
            "0x4D1A2e2bB4F88F0250f26Ffff098B0b30B26BF38"
        );
    }

    #[test]
    fn test_eip1014_create2_address_example1() {
        let from = "0xdeadbeef00000000000000000000000000000000";
        let salt = H256::try_from(
            hex::decode("0x0000000000000000000000000000000000000000000000000000000000000000")
                .unwrap()
                .as_slice(),
        )
        .unwrap();
        let init_code_hash =
            H256::try_from(keccak256(hex::decode("0x00").unwrap().as_slice()).as_slice()).unwrap();
        let address = Address::eip1014_create2_address(from, &salt, &init_code_hash).unwrap();
        assert_eq!(
            address.into_checksum_address(),
            "0xB928f69Bb1D91Cd65274e3c79d8986362984fDA3"
        );
    }

    #[test]
    fn test_eip1014_create2_address_example2() {
        let from = "0xdeadbeef00000000000000000000000000000000";
        let salt = H256::try_from(
            hex::decode("0x000000000000000000000000feed000000000000000000000000000000000000")
                .unwrap()
                .as_slice(),
        )
        .unwrap();
        let mut init_code = hex::decode("0x00").unwrap();
        init_code.resize(32, 0);
        let init_code_hash = H256::try_from(init_code.as_slice()).unwrap();
        let address = Address::eip1014_create2_address(from, &salt, &init_code_hash).unwrap();
        assert_eq!(
            address.into_checksum_address(),
            "0x2DB27D1d6BE32C9abfA484BA3d591101881D4B9f"
        );
    }

    #[test]
    fn test_eip1014_create2_address_example3() {
        let from = "0x0000000000000000000000000000000000000000";
        let salt = H256::try_from(
            hex::decode("0x0000000000000000000000000000000000000000000000000000000000000000")
                .unwrap()
                .as_slice(),
        )
        .unwrap();
        let mut init_code = hex::decode("0xdeadbeef").unwrap();
        init_code.resize(32, 0);
        let init_code_hash = H256::try_from(init_code.as_slice()).unwrap();
        let address = Address::eip1014_create2_address(from, &salt, &init_code_hash).unwrap();
        assert_eq!(
            address.into_checksum_address(),
            "0x219438aC82230Cb9A9C13Cd99D324fA1d66CF018"
        );
    }

    #[test]
    fn test_eip1014_create2_address_example4() {
        let from = "0x00000000000000000000000000000000deadbeef";
        let salt = H256::try_from(
            hex::decode("0x00000000000000000000000000000000000000000000000000000000cafebabe")
                .unwrap()
                .as_slice(),
        )
        .unwrap();
        let init_code_hash =
            H256::try_from(keccak256(hex::decode("0xdeadbeef").unwrap().as_slice()).as_slice())
                .unwrap();
        let address = Address::eip1014_create2_address(from, &salt, &init_code_hash).unwrap();
        assert_eq!(
            address.into_checksum_address(),
            "0x60f3f640a8508fC6a86d45DF051962668E1e8AC7"
        );
    }

    #[test]
    fn test_eip1014_create2_address_example5() {
        let from = "0x00000000000000000000000000000000deadbeef";
        let salt = H256::try_from(
            hex::decode("0x00000000000000000000000000000000000000000000000000000000cafebabe")
                .unwrap()
                .as_slice(),
        )
        .unwrap();
        let init_code_hash = H256::try_from(keccak256(hex::decode("0xdeadbeefdeadbeefdeadbeefdeadbeefdeadbeefdeadbeefdeadbeefdeadbeefdeadbeefdeadbeefdeadbeef").unwrap().as_slice()).as_slice()).unwrap();
        let address = Address::eip1014_create2_address(from, &salt, &init_code_hash).unwrap();
        assert_eq!(
            address.into_checksum_address(),
            "0x1d8bfDC5D46DC4f61D6b6115972536eBE6A8854C"
        );
    }

    #[test]
    fn test_eip1014_create2_address_example6() {
        let from = "0x0000000000000000000000000000000000000000";
        let salt = H256::try_from(
            hex::decode("0x0000000000000000000000000000000000000000000000000000000000000000")
                .unwrap()
                .as_slice(),
        )
        .unwrap();
        let init_code_hash =
            H256::try_from(keccak256(hex::decode("0x").unwrap().as_slice()).as_slice()).unwrap();
        let address = Address::eip1014_create2_address(from, &salt, &init_code_hash).unwrap();
        assert_eq!(
            address.into_checksum_address(),
            "0xE33C0C7F7df4809055C3ebA6c09CFe4BaF1BD9e0"
        );
    }

    #[test]
    fn test_eip1014_create2_address_example7() {
        let from = "0x7EF2e0048f5bAeDe046f6BF797943daF4ED8CB47";
        let salt = H256::try_from(
            hex::decode("0x0000000000000000000000000000000000000000000000000000000000000000")
                .unwrap()
                .as_slice(),
        )
        .unwrap();
        let init_code_hash = keccak256(hex::decode("0x608060405260405162000c5138038062000c51833981810160405281019062000029919062000580565b6200003d828260006200004560201b60201c565b5050620007d7565b62000056836200008860201b60201c565b600082511180620000645750805b156200008357620000818383620000df60201b620000371760201c565b505b505050565b62000099816200011560201b60201c565b8073ffffffffffffffffffffffffffffffffffffffff167fbc7cd75a20ee27fd9adebab32041f755214dbc6bffa90cc0225b39da2e5c2d3b60405160405180910390a250565b60606200010d838360405180606001604052806027815260200162000c2a60279139620001eb60201b60201c565b905092915050565b6200012b816200027d60201b620000641760201c565b6200016d576040517f08c379a000000000000000000000000000000000000000000000000000000000815260040162000164906200066d565b60405180910390fd5b80620001a77f360894a13ba1a3210667c828492db98dca3e2076cc3735a920a3ca505d382bbc60001b620002a060201b620000871760201c565b60000160006101000a81548173ffffffffffffffffffffffffffffffffffffffff021916908373ffffffffffffffffffffffffffffffffffffffff16021790555050565b60606000808573ffffffffffffffffffffffffffffffffffffffff1685604051620002179190620006dc565b600060405180830381855af49150503d806000811462000254576040519150601f19603f3d011682016040523d82523d6000602084013e62000259565b606091505b50915091506200027286838387620002aa60201b60201c565b925050509392505050565b6000808273ffffffffffffffffffffffffffffffffffffffff163b119050919050565b6000819050919050565b606083156200031a5760008351036200031157620002ce856200027d60201b60201c565b62000310576040517f08c379a0000000000000000000000000000000000000000000000000000000008152600401620003079062000745565b60405180910390fd5b5b8290506200032d565b6200032c83836200033560201b60201c565b5b949350505050565b600082511115620003495781518083602001fd5b806040517f08c379a00000000000000000000000000000000000000000000000000000000081526004016200037f9190620007b3565b60405180910390fd5b6000604051905090565b600080fd5b600080fd5b600073ffffffffffffffffffffffffffffffffffffffff82169050919050565b6000620003c9826200039c565b9050919050565b620003db81620003bc565b8114620003e757600080fd5b50565b600081519050620003fb81620003d0565b92915050565b600080fd5b600080fd5b6000601f19601f8301169050919050565b7f4e487b7100000000000000000000000000000000000000000000000000000000600052604160045260246000fd5b62000456826200040b565b810181811067ffffffffffffffff821117156200047857620004776200041c565b5b80604052505050565b60006200048d62000388565b90506200049b82826200044b565b919050565b600067ffffffffffffffff821115620004be57620004bd6200041c565b5b620004c9826200040b565b9050602081019050919050565b60005b83811015620004f6578082015181840152602081019050620004d9565b60008484015250505050565b6000620005196200051384620004a0565b62000481565b90508281526020810184848401111562000538576200053762000406565b5b62000545848285620004d6565b509392505050565b600082601f83011262000565576200056462000401565b5b81516200057784826020860162000502565b91505092915050565b600080604083850312156200059a576200059962000392565b5b6000620005aa85828601620003ea565b925050602083015167ffffffffffffffff811115620005ce57620005cd62000397565b5b620005dc858286016200054d565b9150509250929050565b600082825260208201905092915050565b7f455243313936373a206e657720696d706c656d656e746174696f6e206973206e60008201527f6f74206120636f6e747261637400000000000000000000000000000000000000602082015250565b600062000655602d83620005e6565b91506200066282620005f7565b604082019050919050565b60006020820190508181036000830152620006888162000646565b9050919050565b600081519050919050565b600081905092915050565b6000620006b2826200068f565b620006be81856200069a565b9350620006d0818560208601620004d6565b80840191505092915050565b6000620006ea8284620006a5565b915081905092915050565b7f416464726573733a2063616c6c20746f206e6f6e2d636f6e7472616374000000600082015250565b60006200072d601d83620005e6565b91506200073a82620006f5565b602082019050919050565b6000602082019050818103600083015262000760816200071e565b9050919050565b600081519050919050565b60006200077f8262000767565b6200078b8185620005e6565b93506200079d818560208601620004d6565b620007a8816200040b565b840191505092915050565b60006020820190508181036000830152620007cf818462000772565b905092915050565b61044380620007e76000396000f3fe6080604052366100135761001161001d565b005b61001b61001d565b005b610025610091565b610035610030610093565b6100a2565b565b606061005c83836040518060600160405280602781526020016103e7602791396100c8565b905092915050565b6000808273ffffffffffffffffffffffffffffffffffffffff163b119050919050565b6000819050919050565b565b600061009d61014e565b905090565b3660008037600080366000845af43d6000803e80600081146100c3573d6000f35b3d6000fd5b60606000808573ffffffffffffffffffffffffffffffffffffffff16856040516100f291906102db565b600060405180830381855af49150503d806000811461012d576040519150601f19603f3d011682016040523d82523d6000602084013e610132565b606091505b5091509150610143868383876101a5565b925050509392505050565b600061017c7f360894a13ba1a3210667c828492db98dca3e2076cc3735a920a3ca505d382bbc60001b610087565b60000160009054906101000a900473ffffffffffffffffffffffffffffffffffffffff16905090565b606083156102075760008351036101ff576101bf85610064565b6101fe576040517f08c379a00000000000000000000000000000000000000000000000000000000081526004016101f59061034f565b60405180910390fd5b5b829050610212565b610211838361021a565b5b949350505050565b60008251111561022d5781518083602001fd5b806040517f08c379a000000000000000000000000000000000000000000000000000000000815260040161026191906103c4565b60405180910390fd5b600081519050919050565b600081905092915050565b60005b8381101561029e578082015181840152602081019050610283565b60008484015250505050565b60006102b58261026a565b6102bf8185610275565b93506102cf818560208601610280565b80840191505092915050565b60006102e782846102aa565b915081905092915050565b600082825260208201905092915050565b7f416464726573733a2063616c6c20746f206e6f6e2d636f6e7472616374000000600082015250565b6000610339601d836102f2565b915061034482610303565b602082019050919050565b600060208201905081810360008301526103688161032c565b9050919050565b600081519050919050565b6000601f19601f8301169050919050565b60006103968261036f565b6103a081856102f2565b93506103b0818560208601610280565b6103b98161037a565b840191505092915050565b600060208201905081810360008301526103de818461038b565b90509291505056fe416464726573733a206c6f772d6c6576656c2064656c65676174652063616c6c206661696c6564a2646970667358221220e57dd3eafc9985be746025b6d82d4f011b9a7bb3db56f9a1eb7eadfddd376b6064736f6c63430008110033416464726573733a206c6f772d6c6576656c2064656c65676174652063616c6c206661696c6564000000000000000000000000d9ec9e840bb5df076dbbb488d01485058f421e5800000000000000000000000000000000000000000000000000000000000000400000000000000000000000000000000000000000000000000000000000000024c4d66de8000000000000000000000000be8fa0112dcb7d21dc63645b633073651e19934800000000000000000000000000000000000000000000000000000000").unwrap().as_slice());
        let init_code_hash = H256::try_from(init_code_hash.as_slice()).unwrap();
        let address = Address::eip1014_create2_address(from, &salt, &init_code_hash).unwrap();
        assert_eq!(
            address.into_checksum_address(),
            "0x4455e5f0038795939c001aa4d296A45956C460AA"
        );
    }

    #[test]
    fn test_eip1967_example0() {
        let login_address = "0x5C9eb5D6a6C2c1B3EFc52255C0b356f116f6f66D";
        let data = hex::decode(
            "0xc4d66de8000000000000000000000000a5a1dddef094095afb7b6e322de72961df2e1988",
        )
        .unwrap();
        let init_code = Address::eip_1967_proxy_init_code(login_address, &data).unwrap();
        assert_eq!(
            hex::encode(&init_code, true),
            "0x608060405260405162000c5138038062000c51833981810160405281019062000029919062000580565b6200003d828260006200004560201b60201c565b5050620007d7565b62000056836200008860201b60201c565b600082511180620000645750805b156200008357620000818383620000df60201b620000371760201c565b505b505050565b62000099816200011560201b60201c565b8073ffffffffffffffffffffffffffffffffffffffff167fbc7cd75a20ee27fd9adebab32041f755214dbc6bffa90cc0225b39da2e5c2d3b60405160405180910390a250565b60606200010d838360405180606001604052806027815260200162000c2a60279139620001eb60201b60201c565b905092915050565b6200012b816200027d60201b620000641760201c565b6200016d576040517f08c379a000000000000000000000000000000000000000000000000000000000815260040162000164906200066d565b60405180910390fd5b80620001a77f360894a13ba1a3210667c828492db98dca3e2076cc3735a920a3ca505d382bbc60001b620002a060201b620000871760201c565b60000160006101000a81548173ffffffffffffffffffffffffffffffffffffffff021916908373ffffffffffffffffffffffffffffffffffffffff16021790555050565b60606000808573ffffffffffffffffffffffffffffffffffffffff1685604051620002179190620006dc565b600060405180830381855af49150503d806000811462000254576040519150601f19603f3d011682016040523d82523d6000602084013e62000259565b606091505b50915091506200027286838387620002aa60201b60201c565b925050509392505050565b6000808273ffffffffffffffffffffffffffffffffffffffff163b119050919050565b6000819050919050565b606083156200031a5760008351036200031157620002ce856200027d60201b60201c565b62000310576040517f08c379a0000000000000000000000000000000000000000000000000000000008152600401620003079062000745565b60405180910390fd5b5b8290506200032d565b6200032c83836200033560201b60201c565b5b949350505050565b600082511115620003495781518083602001fd5b806040517f08c379a00000000000000000000000000000000000000000000000000000000081526004016200037f9190620007b3565b60405180910390fd5b6000604051905090565b600080fd5b600080fd5b600073ffffffffffffffffffffffffffffffffffffffff82169050919050565b6000620003c9826200039c565b9050919050565b620003db81620003bc565b8114620003e757600080fd5b50565b600081519050620003fb81620003d0565b92915050565b600080fd5b600080fd5b6000601f19601f8301169050919050565b7f4e487b7100000000000000000000000000000000000000000000000000000000600052604160045260246000fd5b62000456826200040b565b810181811067ffffffffffffffff821117156200047857620004776200041c565b5b80604052505050565b60006200048d62000388565b90506200049b82826200044b565b919050565b600067ffffffffffffffff821115620004be57620004bd6200041c565b5b620004c9826200040b565b9050602081019050919050565b60005b83811015620004f6578082015181840152602081019050620004d9565b60008484015250505050565b6000620005196200051384620004a0565b62000481565b90508281526020810184848401111562000538576200053762000406565b5b62000545848285620004d6565b509392505050565b600082601f83011262000565576200056462000401565b5b81516200057784826020860162000502565b91505092915050565b600080604083850312156200059a576200059962000392565b5b6000620005aa85828601620003ea565b925050602083015167ffffffffffffffff811115620005ce57620005cd62000397565b5b620005dc858286016200054d565b9150509250929050565b600082825260208201905092915050565b7f455243313936373a206e657720696d706c656d656e746174696f6e206973206e60008201527f6f74206120636f6e747261637400000000000000000000000000000000000000602082015250565b600062000655602d83620005e6565b91506200066282620005f7565b604082019050919050565b60006020820190508181036000830152620006888162000646565b9050919050565b600081519050919050565b600081905092915050565b6000620006b2826200068f565b620006be81856200069a565b9350620006d0818560208601620004d6565b80840191505092915050565b6000620006ea8284620006a5565b915081905092915050565b7f416464726573733a2063616c6c20746f206e6f6e2d636f6e7472616374000000600082015250565b60006200072d601d83620005e6565b91506200073a82620006f5565b602082019050919050565b6000602082019050818103600083015262000760816200071e565b9050919050565b600081519050919050565b60006200077f8262000767565b6200078b8185620005e6565b93506200079d818560208601620004d6565b620007a8816200040b565b840191505092915050565b60006020820190508181036000830152620007cf818462000772565b905092915050565b61044380620007e76000396000f3fe6080604052366100135761001161001d565b005b61001b61001d565b005b610025610091565b610035610030610093565b6100a2565b565b606061005c83836040518060600160405280602781526020016103e7602791396100c8565b905092915050565b6000808273ffffffffffffffffffffffffffffffffffffffff163b119050919050565b6000819050919050565b565b600061009d61014e565b905090565b3660008037600080366000845af43d6000803e80600081146100c3573d6000f35b3d6000fd5b60606000808573ffffffffffffffffffffffffffffffffffffffff16856040516100f291906102db565b600060405180830381855af49150503d806000811461012d576040519150601f19603f3d011682016040523d82523d6000602084013e610132565b606091505b5091509150610143868383876101a5565b925050509392505050565b600061017c7f360894a13ba1a3210667c828492db98dca3e2076cc3735a920a3ca505d382bbc60001b610087565b60000160009054906101000a900473ffffffffffffffffffffffffffffffffffffffff16905090565b606083156102075760008351036101ff576101bf85610064565b6101fe576040517f08c379a00000000000000000000000000000000000000000000000000000000081526004016101f59061034f565b60405180910390fd5b5b829050610212565b610211838361021a565b5b949350505050565b60008251111561022d5781518083602001fd5b806040517f08c379a000000000000000000000000000000000000000000000000000000000815260040161026191906103c4565b60405180910390fd5b600081519050919050565b600081905092915050565b60005b8381101561029e578082015181840152602081019050610283565b60008484015250505050565b60006102b58261026a565b6102bf8185610275565b93506102cf818560208601610280565b80840191505092915050565b60006102e782846102aa565b915081905092915050565b600082825260208201905092915050565b7f416464726573733a2063616c6c20746f206e6f6e2d636f6e7472616374000000600082015250565b6000610339601d836102f2565b915061034482610303565b602082019050919050565b600060208201905081810360008301526103688161032c565b9050919050565b600081519050919050565b6000601f19601f8301169050919050565b60006103968261036f565b6103a081856102f2565b93506103b0818560208601610280565b6103b98161037a565b840191505092915050565b600060208201905081810360008301526103de818461038b565b90509291505056fe416464726573733a206c6f772d6c6576656c2064656c65676174652063616c6c206661696c6564a2646970667358221220e57dd3eafc9985be746025b6d82d4f011b9a7bb3db56f9a1eb7eadfddd376b6064736f6c63430008110033416464726573733a206c6f772d6c6576656c2064656c65676174652063616c6c206661696c65640000000000000000000000005c9eb5d6a6c2c1b3efc52255c0b356f116f6f66d00000000000000000000000000000000000000000000000000000000000000400000000000000000000000000000000000000000000000000000000000000024c4d66de8000000000000000000000000a5a1dddef094095afb7b6e322de72961df2e198800000000000000000000000000000000000000000000000000000000"
        );
    }
}
