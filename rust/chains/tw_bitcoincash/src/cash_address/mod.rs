// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::fmt;
use std::str::FromStr;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::CoinAddress;
use tw_coin_entry::error::prelude::*;
use tw_encoding::bech32;
use tw_hash::ripemd::sha256_ripemd;
use tw_hash::H160;
use tw_keypair::ecdsa;
use tw_memory::Data;
use tw_utxo::address::legacy::LegacyAddress;

/// BitcoinCash specific base32 format.
mod cash_base32;
mod checksum;
mod unchecked;

#[derive(Copy, Clone, Debug, Eq, PartialEq)]
pub enum CashAddressType {
    P2PKH,
    P2SH,
}

#[derive(Debug, Eq, PartialEq)]
pub struct CashAddress {
    hrp: String,
    ty: CashAddressType,
    key_hash: H160,
    /// An address string created from this `hrp`, `ty` and `key_hash`.
    address_str: String,
}

impl CashAddress {
    pub fn p2pkh_with_coin(
        coin: &dyn CoinContext,
        public_key: &ecdsa::secp256k1::PublicKey,
    ) -> AddressResult<CashAddress> {
        let hrp = coin.hrp().ok_or(AddressError::InvalidRegistry)?;
        Self::p2pkh_with_hrp(hrp, public_key)
    }

    pub fn p2pkh_with_hrp(
        hrp: String,
        public_key: &ecdsa::secp256k1::PublicKey,
    ) -> AddressResult<CashAddress> {
        let public_key_hash = sha256_ripemd(public_key.compressed().as_slice());
        let key_hash =
            H160::try_from(public_key_hash.as_slice()).expect("sha256_ripemd returns 20 bytes");
        let address_str = Self::encode(&hrp, CashAddressType::P2PKH, &key_hash)?;
        Ok(CashAddress {
            hrp,
            ty: CashAddressType::P2PKH,
            key_hash: H160::try_from(public_key_hash.as_slice())
                .expect("sha256_ripemd returns 20 bytes"),
            address_str,
        })
    }

    pub fn from_str_with_coin_and_hrp(
        coin: &dyn CoinContext,
        address_str: &str,
        expected_hrp: Option<&str>,
    ) -> AddressResult<Self> {
        let expected_hrp = match expected_hrp {
            Some(hrp) => hrp.to_string(),
            None => coin.hrp().ok_or(AddressError::InvalidRegistry)?,
        };

        Self::from_str_with_hrp(address_str, expected_hrp)
    }

    pub fn from_str_with_hrp(address_str: &str, expected_hrp: String) -> AddressResult<Self> {
        unchecked::UncheckedCashAddress::from_str(address_str)?.checked_with_prefix(expected_hrp)
    }

    pub fn from_str_unchecked(address_str: &str) -> AddressResult<Self> {
        unchecked::UncheckedCashAddress::from_str(address_str)?.partly_checked()
    }

    pub fn to_legacy(&self, p2pkh_prefix: u8, p2sh_prefix: u8) -> AddressResult<LegacyAddress> {
        match self.ty {
            CashAddressType::P2PKH => LegacyAddress::new(p2pkh_prefix, self.key_hash.as_slice()),
            CashAddressType::P2SH => LegacyAddress::new(p2sh_prefix, self.key_hash.as_slice()),
        }
    }

    fn encode(hrp: &str, ty: CashAddressType, key_hash: &H160) -> AddressResult<String> {
        let mut payload = Vec::with_capacity(1 + H160::LEN + checksum::CHECKSUM_LEN);
        payload.push(Self::version_byte(ty));
        payload.extend_from_slice(key_hash.as_slice());

        let payload_u5 = {
            let from = 8;
            let to = 5;
            let pad = true;
            bech32::convert_bits(&payload, from, to, pad).map_err(|_| AddressError::InvalidInput)?
        };

        let payload_with_checksum = checksum::calc_and_append_checksum(hrp, &payload_u5);
        let encoded_payload =
            cash_base32::encode(&payload_with_checksum).map_err(|_| AddressError::InvalidInput)?;
        Ok(format!("{hrp}:{encoded_payload}"))
    }

    fn version_byte(ty: CashAddressType) -> u8 {
        let en_address_type: u8 = match ty {
            CashAddressType::P2PKH => 0,
            CashAddressType::P2SH => 1,
        };

        // Please note we always serialize the key hash length as 0 here,
        // as the length is always 20 (160 bits).
        // https://github.com/bitcoincashorg/bitcoincash.org/blob/master/spec/cashaddr.md#version-byte
        let serialized_hash_len = 0;

        let mut version_byte = en_address_type;
        version_byte <<= 3;
        version_byte |= serialized_hash_len;

        version_byte
    }
}

impl CoinAddress for CashAddress {
    #[inline]
    fn data(&self) -> Data {
        self.key_hash.to_vec()
    }
}

impl fmt::Display for CashAddress {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.address_str)
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[track_caller]
    fn test_address_from_str_impl(s: &str, expected: CashAddress) {
        let actual = CashAddress::from_str_with_hrp(s, expected.hrp.clone()).unwrap();
        assert_eq!(actual, expected);
    }

    #[test]
    fn test_cash_address_from_str() {
        test_address_from_str_impl(
            "bitcoincash:pq4ql3ph6738xuv2cycduvkpu4rdwqge5q2uxdfg6f",
            CashAddress {
                hrp: "bitcoincash".to_string(),
                ty: CashAddressType::P2SH,
                key_hash: H160::from("2a0fc437d7a273718ac130de32c1e546d70119a0"),
                address_str: "bitcoincash:pq4ql3ph6738xuv2cycduvkpu4rdwqge5q2uxdfg6f".to_string(),
            },
        );

        test_address_from_str_impl(
            "qrplwyx7kueqkrh6dmd3fclta6u32hafp5tnpkchx2",
            CashAddress {
                hrp: "bitcoincash".to_string(),
                ty: CashAddressType::P2PKH,
                key_hash: H160::from("c3f710deb7320b0efa6edb14e3ebeeb9155fa90d"),
                address_str: "bitcoincash:qrplwyx7kueqkrh6dmd3fclta6u32hafp5tnpkchx2".to_string(),
            },
        );

        test_address_from_str_impl(
            "BitCoinCash:QRPLWYX7KUEQKRH6DMD3FCLTA6U32HAFP5TNPKCHX2",
            CashAddress {
                hrp: "bitcoincash".to_string(),
                ty: CashAddressType::P2PKH,
                key_hash: H160::from("c3f710deb7320b0efa6edb14e3ebeeb9155fa90d"),
                address_str: "bitcoincash:qrplwyx7kueqkrh6dmd3fclta6u32hafp5tnpkchx2".to_string(),
            },
        );

        test_address_from_str_impl(
            "bchtest:qqjr7yu573z4faxw8ltgvjwpntwys08fysk07zmvce",
            CashAddress {
                hrp: "bchtest".to_string(),
                ty: CashAddressType::P2PKH,
                key_hash: H160::from("243f1394f44554f4ce3fd68649c19adc483ce924"),
                address_str: "bchtest:qqjr7yu573z4faxw8ltgvjwpntwys08fysk07zmvce".to_string(),
            },
        );
    }

    struct AddressToLegacyTest<'a> {
        p2pkh_prefix: u8,
        p2sh_prefixes: u8,
        hrp: &'a str,
        address: &'a str,
        legacy: &'a str,
    }

    #[track_caller]
    fn test_address_to_legacy_impl(input: AddressToLegacyTest) {
        let cash = CashAddress::from_str_with_hrp(input.address, input.hrp.to_string()).unwrap();
        let legacy = cash
            .to_legacy(input.p2pkh_prefix, input.p2sh_prefixes)
            .unwrap();
        assert_eq!(legacy.to_string(), input.legacy);
    }

    #[test]
    fn test_cash_address_to_legacy() {
        // P2PKH
        test_address_to_legacy_impl(AddressToLegacyTest {
            p2pkh_prefix: 0,
            p2sh_prefixes: 5,
            hrp: "bitcoincash",
            address: "bitcoincash:qpm2qsznhks23z7629mms6s4cwef74vcwvy22gdx6a",
            legacy: "1BpEi6DfDAUFd7GtittLSdBeYJvcoaVggu",
        });

        // P2PKH
        test_address_to_legacy_impl(AddressToLegacyTest {
            p2pkh_prefix: 0,
            p2sh_prefixes: 5,
            hrp: "bitcoincash",
            address: "qr95sy3j9xwd2ap32xkykttr4cvcu7as4y0qverfuy",
            legacy: "1KXrWXciRDZUpQwQmuM1DbwsKDLYAYsVLR",
        });

        // P2SH
        test_address_to_legacy_impl(AddressToLegacyTest {
            p2pkh_prefix: 0,
            p2sh_prefixes: 5,
            hrp: "bitcoincash",
            address: "bitcoincash:pqq3728yw0y47sqn6l2na30mcw6zm78dzq5ucqzc37",
            legacy: "31nwvkZwyPdgzjBJZXfDmSWsC4ZLKpYyUw",
        });

        // P2PKH testnet
        test_address_to_legacy_impl(AddressToLegacyTest {
            p2pkh_prefix: 111,
            p2sh_prefixes: 196,
            hrp: "bchtest",
            address: "bchtest:qpcgc96kles6v35a2525dppaapzk2htdkv72dv08qf",
            legacy: "mqn3oF7tcju9uqoG1H1AZt4M6CP1PTaELo",
        });
    }
}
