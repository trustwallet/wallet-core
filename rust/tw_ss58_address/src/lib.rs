use blake2::{Blake2b512, Digest};
use std::fmt::Formatter;
use std::str::FromStr;
use tw_coin_entry::error::prelude::*;
use tw_encoding::{base58, hex};
use tw_keypair::tw::{PrivateKey, PublicKey};

//
// Most of the materials implemented here is based on the following resources:
// - https://wiki.polkadot.network/docs/learn-account-advanced#address-format
// - https://github.com/paritytech/polkadot-sdk/blob/master/substrate/primitives/core/src/crypto.rs
//

#[derive(Debug, Copy, Clone, PartialOrd, Ord, PartialEq, Eq)]
pub struct NetworkId(u16);

impl NetworkId {
    fn new_unchecked(value: u16) -> Self {
        return Self(value);
    }

    pub fn from_u16(value: u16) -> AddressResult<Self> {
        match value {
            0..=0x3fff => Ok(Self::new_unchecked(value)),
            _ => Err(AddressError::InvalidInput),
        }
    }

    pub fn value(&self) -> u16 {
        self.0
    }
}

impl TryFrom<u16> for NetworkId {
    type Error = AddressError;

    fn try_from(value: u16) -> Result<Self, Self::Error> {
        Self::from_u16(value)
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct SS58Address {
    key: Vec<u8>,
    network: NetworkId,
}

impl SS58Address {
    const CHECKSUM_SIZE: usize = 2;
    const KEY_SIZE: usize = 32;
    const SS58_PREFIX: &'static [u8] = b"SS58PRE";

    fn extract_network(bytes: &[u8]) -> AddressResult<(usize, u16)> {
        if bytes.len() == 0 {
            return Err(AddressError::MissingPrefix);
        }

        match bytes[0] {
            0..=63 => Ok((1, bytes[0] as u16)),
            64..=127 if bytes.len() >= 2 => {
                let lower = (bytes[0] << 2) | (bytes[1] >> 6);
                let upper = bytes[1] & 0b0011_1111;
                Ok((2, (lower as u16) | ((upper as u16) << 8)))
            },
            _ => Err(AddressError::UnexpectedAddressPrefix),
        }
    }

    fn encode_network(network_id: NetworkId) -> Vec<u8> {
        let network = network_id.value();
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

    fn compute_expected_checksum(decoded: &[u8]) -> Vec<u8> {
        // XXX: tried using tw_hash::blake2::blake2_b, but it did not produce the correct result
        let mut ctx = Blake2b512::new();
        ctx.update(Self::SS58_PREFIX);
        ctx.update(decoded);

        let mut bytes = ctx.finalize().to_vec();
        bytes.truncate(Self::CHECKSUM_SIZE);
        bytes
    }

    pub fn from_str(repr: &str) -> AddressResult<Self> {
        let decoded = base58::decode(repr, base58::Alphabet::BITCOIN)
            .map_err(|_| AddressError::FromBase58Error)?;

        let (network_prefix_len, network) = Self::extract_network(&decoded)?;

        if network_prefix_len + Self::KEY_SIZE + Self::CHECKSUM_SIZE != decoded.len() {
            return Err(AddressError::FromBase58Error);
        }

        let expected_checksum =
            Self::compute_expected_checksum(&decoded[..decoded.len() - Self::CHECKSUM_SIZE]);
        let checksum = &decoded[decoded.len() - Self::CHECKSUM_SIZE..];

        if expected_checksum != checksum {
            return Err(AddressError::InvalidChecksum);
        }

        Ok(Self {
            key: decoded[network_prefix_len..network_prefix_len + Self::KEY_SIZE].to_owned(),
            network: NetworkId::new_unchecked(network),
        })
    }

    pub fn from_public_key(key: &PublicKey, network: u16) -> AddressResult<Self> {
        let network = NetworkId::try_from(network)?;

        let bytes = match key {
            PublicKey::Ed25519(k) => k.as_slice(),
            _ => return Err(AddressError::InvalidInput),
        };

        Ok(Self {
            key: bytes.to_owned(),
            network,
        })
    }

    pub fn network(&self) -> NetworkId {
        self.network
    }

    pub fn key_bytes(&self) -> &[u8] {
        &self.key
    }

    fn as_bytes(&self) -> Vec<u8> {
        let mut res = Self::encode_network(self.network);
        res.extend(self.key_bytes());
        res.extend(Self::compute_expected_checksum(&res));
        res
    }

    fn as_base58_string(&self) -> String {
        base58::encode(&self.as_bytes(), base58::Alphabet::BITCOIN)
    }

    fn as_hex_string(&self) -> String {
        hex::encode(&self.as_bytes(), false)
    }
}

impl FromStr for SS58Address {
    type Err = AddressError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        Self::from_str(s)
    }
}

impl std::fmt::Display for SS58Address {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        f.write_str(&self.as_base58_string())
    }
}

#[cfg(test)]
mod tests {
    use super::{NetworkId, SS58Address};
    use std::string::ToString;
    use tw_coin_entry::error::prelude::AddressError;
    use tw_encoding::hex;
    use tw_keypair::tw::{PublicKey, PublicKeyType};

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
    fn test_address_from_str_valid() {
        fn test_case(repr: &str, expected_network: u16) {
            let addr = SS58Address::from_str(repr).expect("error parsing address");
            assert_eq!(addr.network().value(), expected_network);
        }

        test_case("15KRsCq9LLNmCxNFhGk55s5bEyazKefunDxUH24GFZwsTxyu", 0);
        test_case("5CK8D1sKNwF473wbuBP6NuhQfPaWUetNsWUNAAzVwTfxqjfr", 42);
        test_case("CpjsLDC1JFyrhm3ftC9Gs4QoyrkHKhZKtK7YqGTRFtTafgp", 2);
        test_case("Fu3r514w83euSVV7q1MyFGWErUR2xDzXS2goHzimUn4S12D", 2);
        test_case("ZG2d3dH5zfqNchsqReS6x4nBJuJCW7Z6Fh5eLvdA3ZXGkPd", 5);
        test_case("cEYtw6AVMB27hFUs4gVukajLM7GqxwxUfJkbPY3rNToHMcCgb", 64);
        test_case("p8EGHjWt7e1MYoD7V6WXvbPZWK9GSJiiK85kv2R7Ur7FisPUL", 172);
        test_case("VDSyeURSP7ykE1zJPJGeqx6GcDZQF2DT3hAKhPMuwM5FuN9HE", 4096);
        test_case("YDTv3GdhXPP3pQMqQtntGVg5hMno4jqanfYUgMPX2rLGJBKX6", 8219);
    }

    #[test]
    fn test_address_from_public_key() {
        let key_hex = "92fd9c237030356e26cfcc4568dc71055d5ec92dfe0ff903767e00611971bad3";
        let key_bytes = hex::decode(key_hex).expect("error decoding public key");
        let key = PublicKey::new(key_bytes, PublicKeyType::Ed25519)
            .expect("error creating test public key");

        let addr = SS58Address::from_public_key(&key, 0).expect("error creating address");
        assert_eq!(addr.network().value(), 0);
        assert_eq!(addr.key_bytes(), key.to_bytes());

        let addr = SS58Address::from_public_key(&key, 5).expect("error creating address");
        assert_eq!(addr.network().value(), 5);
        assert_eq!(addr.key_bytes(), key.to_bytes());

        let addr = SS58Address::from_public_key(&key, 172).expect("error creating address");
        assert_eq!(addr.network().value(), 172);
        assert_eq!(addr.key_bytes(), key.to_bytes());
    }

    #[test]
    fn test_address_from_public_key_with_invalid_network() {
        let key_hex = "92fd9c237030356e26cfcc4568dc71055d5ec92dfe0ff903767e00611971bad3";
        let key_bytes = hex::decode(key_hex).expect("error decoding public key");
        let key = PublicKey::new(key_bytes, PublicKeyType::Ed25519)
            .expect("error creating test public key");

        let res = SS58Address::from_public_key(&key, 32771);
        assert_eq!(res, Err(AddressError::InvalidInput));
    }

    #[test]
    fn test_extract_network_valid() {
        fn test_case(prefix: &[u8], expected_network: u16) {
            let (prefix_length, network) =
                SS58Address::extract_network(prefix).expect("error extracting network");
            let expected_prefix_length = if network < 64 { 1 } else { 2 } as usize;
            assert_eq!(
                prefix_length, expected_prefix_length,
                "for expected network {}",
                expected_network
            );
            assert_eq!(
                network, expected_network,
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
    fn test_extract_network_invalid() {
        // at least one byte is expected
        let res = SS58Address::extract_network(&[]);
        assert_eq!(res, Err(AddressError::MissingPrefix));

        // the first byte should be in 0..=127
        let res = SS58Address::extract_network(&[0xFF]);
        assert_eq!(res, Err(AddressError::UnexpectedAddressPrefix));

        // a second byte should follow
        let res = SS58Address::extract_network(&[0x40]);
        assert_eq!(res, Err(AddressError::UnexpectedAddressPrefix));
    }

    #[test]
    fn test_encode_network() {
        fn test_case(expected_prefix: &[u8], network: u16) {
            let prefix = SS58Address::encode_network(NetworkId::new_unchecked(network));

            assert_eq!(prefix, expected_prefix, "for network {}", network);
        }

        for (prefix, network) in networks() {
            test_case(&prefix, network);
        }
    }

    #[test]
    fn test_as_base58_string() {
        fn test_case(repr: &str) {
            let addr = SS58Address::from_str(repr).expect("error parsing address");
            assert_eq!(addr.as_base58_string(), repr);
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
            addr.as_hex_string(),
            "000aff6865635ae11013a83835c019d44ec3f865145943f487ae82a8e7bed3a66b29d7"
        );
    }
}
