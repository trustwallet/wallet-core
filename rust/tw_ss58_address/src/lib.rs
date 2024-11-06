use std::fmt::Formatter;
use std::str::FromStr;

use tw_coin_entry::error::prelude::*;
use tw_encoding::{base58, hex};
use tw_hash::blake2::blake2_b;
use tw_keypair::ed25519::sha512::PublicKey;
use tw_scale::{Raw, ToScale};

//
// Most of the materials implemented here are based on the following resources:
// - https://wiki.polkadot.network/docs/learn-account-advanced#address-format
// - https://github.com/paritytech/polkadot-sdk/blob/master/substrate/primitives/core/src/crypto.rs
//

#[derive(Debug, Copy, Clone, PartialOrd, Ord, PartialEq, Eq, Hash)]
pub struct NetworkId(u16);

impl NetworkId {
    pub const POLKADOT: Self = Self::new_unchecked(0);
    pub const KUSAMA: Self = Self::new_unchecked(2);
    pub const POLYMESH: Self = Self::new_unchecked(12);
    pub const GENERIC_SUBSTRATE: Self = Self::new_unchecked(42);
}

impl NetworkId {
    const fn new_unchecked(value: u16) -> Self {
        Self(value)
    }

    pub fn from_u16(value: u16) -> AddressResult<Self> {
        match value {
            0..=0x3fff => Ok(Self::new_unchecked(value)),
            _ => Err(AddressError::InvalidInput),
        }
    }

    pub fn from_bytes(bytes: &[u8]) -> AddressResult<Self> {
        if bytes.is_empty() {
            return Err(AddressError::MissingPrefix);
        }

        match bytes[0] {
            0..=63 => Ok(bytes[0] as u16),
            64..=127 if bytes.len() >= 2 => {
                let lower = (bytes[0] << 2) | (bytes[1] >> 6);
                let upper = bytes[1] & 0b0011_1111;
                Ok((lower as u16) | ((upper as u16) << 8))
            },
            _ => Err(AddressError::UnexpectedAddressPrefix),
        }
        .map(Self::new_unchecked)
    }

    pub fn value(&self) -> u16 {
        self.0
    }

    pub fn prefix_len(&self) -> usize {
        if self.value() < 64 {
            1
        } else {
            2
        }
    }

    pub fn to_bytes(&self) -> Vec<u8> {
        let network = self.value();
        match network {
            0..=63 => {
                vec![network as u8]
            },
            _ => {
                let first = ((network & 0b0000_0000_1111_1100) as u8) >> 2;
                let second =
                    ((network >> 8) as u8) | ((network & 0b0000_0000_0000_0011) as u8) << 6;
                vec![first | 0b01000000, second]
            },
        }
    }
}

impl TryFrom<u16> for NetworkId {
    type Error = AddressError;

    fn try_from(value: u16) -> Result<Self, Self::Error> {
        Self::from_u16(value)
    }
}

impl TryFrom<&[u8]> for NetworkId {
    type Error = AddressError;

    fn try_from(bytes: &[u8]) -> Result<Self, Self::Error> {
        Self::from_bytes(bytes)
    }
}

#[derive(Debug, Clone, PartialEq, Eq, Hash)]
pub struct SS58Address {
    key: Vec<u8>,
    network: NetworkId,
}

impl SS58Address {
    const CHECKSUM_SIZE: usize = 2;
    const KEY_SIZE: usize = 32;
    const SS58_PREFIX: &'static [u8] = b"SS58PRE";

    fn compute_expected_checksum(decoded: &[u8]) -> Vec<u8> {
        let mut data = Vec::from(Self::SS58_PREFIX);
        data.extend(decoded);

        let mut bytes = blake2_b(&data, 64).expect("hash length should be valid");
        bytes.truncate(Self::CHECKSUM_SIZE);
        bytes
    }

    pub fn parse(repr: &str) -> AddressResult<Self> {
        let decoded = base58::decode(repr, base58::Alphabet::Bitcoin)
            .map_err(|_| AddressError::FromBase58Error)?;

        let network = NetworkId::from_bytes(&decoded)?;

        if network.prefix_len() + Self::KEY_SIZE + Self::CHECKSUM_SIZE != decoded.len() {
            return Err(AddressError::FromBase58Error);
        }

        let expected_checksum =
            Self::compute_expected_checksum(&decoded[..decoded.len() - Self::CHECKSUM_SIZE]);
        let checksum = &decoded[decoded.len() - Self::CHECKSUM_SIZE..];

        if expected_checksum != checksum {
            return Err(AddressError::InvalidChecksum);
        }

        Ok(Self {
            key: decoded[network.prefix_len()..network.prefix_len() + Self::KEY_SIZE].to_owned(),
            network,
        })
    }

    pub fn from_public_key(key: &PublicKey, network: NetworkId) -> AddressResult<Self> {
        Ok(Self {
            key: key.as_slice().to_owned(),
            network,
        })
    }

    pub fn network(&self) -> NetworkId {
        self.network
    }

    pub fn key_bytes(&self) -> &[u8] {
        &self.key
    }

    pub fn to_bytes(&self) -> Vec<u8> {
        let mut res = self.network.to_bytes();
        res.extend(self.key_bytes());
        res.extend(Self::compute_expected_checksum(&res));
        res
    }

    pub fn to_base58_string(&self) -> String {
        base58::encode(&self.to_bytes(), base58::Alphabet::Bitcoin)
    }

    pub fn to_hex_string(&self) -> String {
        hex::encode(self.to_bytes(), false)
    }
}

impl ToScale for SS58Address {
    fn to_scale_into(&self, out: &mut Vec<u8>) {
        Raw(self.key_bytes()).to_scale_into(out)
    }
}

impl FromStr for SS58Address {
    type Err = AddressError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        Self::parse(s)
    }
}

impl std::fmt::Display for SS58Address {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        f.write_str(&self.to_base58_string())
    }
}

#[cfg(test)]
mod tests {
    use super::{NetworkId, SS58Address};
    use std::str::FromStr;
    use tw_coin_entry::error::prelude::AddressError;
    use tw_keypair::ed25519::sha512::PublicKey;

    fn networks() -> [(Vec<u8>, u16); 27] {
        [
            (vec![0x00], 0x00),
            (vec![0x01], 0x01),
            (vec![0x02], 0x02),
            (vec![0x03], 0x03),
            (vec![0x04], 0x04),
            (vec![0x08], 0x08),
            (vec![0x0b], 0x0b),
            (vec![0x10], 0x10),
            (vec![0x20], 0x20),
            (vec![0x23], 0x23),
            (vec![0x30], 0x30),
            (vec![0x3f], 0x3f),
            (vec![0x50, 0x00], 0x40),
            (vec![0x50, 0x40], 0x41),
            (vec![0x60, 0x00], 0x80),
            (vec![0x40, 0x01], 0x0100),
            (vec![0x48, 0xc1], 0x0123),
            (vec![0x40, 0x02], 0x0200),
            (vec![0x40, 0x03], 0x0300),
            (vec![0x40, 0x04], 0x0400),
            (vec![0x40, 0x08], 0x0800),
            (vec![0x7f, 0xcf], 0x0fff),
            (vec![0x40, 0x10], 0x1000),
            (vec![0x40, 0xd0], 0x1003),
            (vec![0x40, 0x20], 0x2000),
            (vec![0x40, 0x30], 0x3000),
            (vec![0x7f, 0xff], 0x3fff),
        ]
    }

    #[test]
    fn test_network_id() {
        for (_, network) in networks() {
            let n = NetworkId::try_from(network).expect("error parsing network");
            assert_eq!(n.value(), network);
        }

        assert_eq!(NetworkId::try_from(0x4000), Err(AddressError::InvalidInput));
    }

    #[test]
    fn test_network_from_bytes() {
        fn test_case(prefix: &[u8], expected_network: u16) {
            let network = NetworkId::from_bytes(prefix).expect("error extracting network");
            let expected_prefix_length = if network.value() < 64 { 1 } else { 2 } as usize;
            assert_eq!(
                network.prefix_len(),
                expected_prefix_length,
                "for expected network {}",
                expected_network
            );
            assert_eq!(
                network.value(),
                expected_network,
                "for expected network {}",
                expected_network
            );
        }

        for (prefix, network) in networks() {
            test_case(&prefix, network);
        }

        // ensure prefix length is returned as expected
        test_case(&[0x00, 0x00], 0x00)
    }

    #[test]
    fn test_network_from_bytes_invalid() {
        // at least one byte is expected
        let res = NetworkId::from_bytes(&[]);
        assert_eq!(res, Err(AddressError::MissingPrefix));

        // the first byte should be in 0..=127
        let res = NetworkId::from_bytes(&[0xFF]);
        assert_eq!(res, Err(AddressError::UnexpectedAddressPrefix));

        // a second byte should follow
        let res = NetworkId::from_bytes(&[0x40]);
        assert_eq!(res, Err(AddressError::UnexpectedAddressPrefix));
    }

    #[test]
    fn test_network_as_bytes() {
        fn test_case(expected_prefix: &[u8], network: u16) {
            let prefix = NetworkId::new_unchecked(network).to_bytes();

            assert_eq!(prefix, expected_prefix, "for network {}", network);
        }

        for (prefix, network) in networks() {
            test_case(&prefix, network);
        }
    }

    #[test]
    fn test_address_from_str() {
        fn test_case(repr: &str, expected_network: u16) {
            let addr = SS58Address::from_str(repr).expect("error parsing address");
            assert_eq!(addr.network().value(), expected_network);
        }

        test_case(
            "15KRsCq9LLNmCxNFhGk55s5bEyazKefunDxUH24GFZwsTxyu",
            NetworkId::POLKADOT.value(),
        );
        test_case(
            "5CK8D1sKNwF473wbuBP6NuhQfPaWUetNsWUNAAzVwTfxqjfr",
            NetworkId::GENERIC_SUBSTRATE.value(),
        );
        test_case(
            "CpjsLDC1JFyrhm3ftC9Gs4QoyrkHKhZKtK7YqGTRFtTafgp",
            NetworkId::KUSAMA.value(),
        );
        test_case(
            "Fu3r514w83euSVV7q1MyFGWErUR2xDzXS2goHzimUn4S12D",
            NetworkId::KUSAMA.value(),
        );
        test_case("ZG2d3dH5zfqNchsqReS6x4nBJuJCW7Z6Fh5eLvdA3ZXGkPd", 5);
        test_case("cEYtw6AVMB27hFUs4gVukajLM7GqxwxUfJkbPY3rNToHMcCgb", 64);
        test_case("p8EGHjWt7e1MYoD7V6WXvbPZWK9GSJiiK85kv2R7Ur7FisPUL", 172);
        test_case("VDSyeURSP7ykE1zJPJGeqx6GcDZQF2DT3hAKhPMuwM5FuN9HE", 4096);
        test_case("YDTv3GdhXPP3pQMqQtntGVg5hMno4jqanfYUgMPX2rLGJBKX6", 8219);
    }

    #[test]
    fn test_address_from_public_key() {
        let key_hex = "92fd9c237030356e26cfcc4568dc71055d5ec92dfe0ff903767e00611971bad3";
        let key = PublicKey::try_from(key_hex).expect("error creating test public key");

        let addr = SS58Address::from_public_key(&key, NetworkId::POLKADOT)
            .expect("error creating address");
        assert_eq!(addr.network().value(), 0);
        assert_eq!(addr.key_bytes(), key.as_slice());

        let addr = SS58Address::from_public_key(&key, NetworkId::new_unchecked(5))
            .expect("error creating address");
        assert_eq!(addr.network().value(), 5);
        assert_eq!(addr.key_bytes(), key.as_slice());

        let addr = SS58Address::from_public_key(&key, NetworkId::new_unchecked(172))
            .expect("error creating address");
        assert_eq!(addr.network().value(), 172);
        assert_eq!(addr.key_bytes(), key.as_slice());
    }

    #[test]
    fn test_as_base58_string() {
        fn test_case(repr: &str) {
            let addr = SS58Address::from_str(repr).expect("error parsing address");
            assert_eq!(addr.to_base58_string(), repr);
        }

        test_case("Fu3r514w83euSVV7q1MyFGWErUR2xDzXS2goHzimUn4S12D");
        test_case("15KRsCq9LLNmCxNFhGk55s5bEyazKefunDxUH24GFZwsTxyu");
        test_case("5CK8D1sKNwF473wbuBP6NuhQfPaWUetNsWUNAAzVwTfxqjfr");
        test_case("CpjsLDC1JFyrhm3ftC9Gs4QoyrkHKhZKtK7YqGTRFtTafgp");
        test_case("Fu3r514w83euSVV7q1MyFGWErUR2xDzXS2goHzimUn4S12D");
        test_case("ZG2d3dH5zfqNchsqReS6x4nBJuJCW7Z6Fh5eLvdA3ZXGkPd");
        test_case("cEYtw6AVMB27hFUs4gVukajLM7GqxwxUfJkbPY3rNToHMcCgb");
        test_case("p8EGHjWt7e1MYoD7V6WXvbPZWK9GSJiiK85kv2R7Ur7FisPUL");
        test_case("VDSyeURSP7ykE1zJPJGeqx6GcDZQF2DT3hAKhPMuwM5FuN9HE");
        test_case("YDTv3GdhXPP3pQMqQtntGVg5hMno4jqanfYUgMPX2rLGJBKX6");
    }

    #[test]
    fn test_as_hex_string() {
        let addr = SS58Address::from_str("1FRMM8PEiWXYax7rpS6X4XZX1aAAxSWx1CrKTyrVYhV24fg")
            .expect("error parsing address");
        assert_eq!(
            addr.to_hex_string(),
            "000aff6865635ae11013a83835c019d44ec3f865145943f487ae82a8e7bed3a66b29d7"
        );
    }
}
