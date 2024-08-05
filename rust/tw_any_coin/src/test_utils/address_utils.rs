// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::ffi::tw_any_address::{
    tw_any_address_create_base58_with_public_key, tw_any_address_create_bech32_with_public_key,
    tw_any_address_create_with_public_key_derivation, tw_any_address_create_with_string,
    tw_any_address_data, tw_any_address_delete, tw_any_address_description,
    tw_any_address_is_valid, tw_any_address_is_valid_base58, tw_any_address_is_valid_bech32,
    TWAnyAddress,
};
use tw_coin_registry::coin_type::CoinType;
use tw_coin_registry::registry::get_coin_item;
use tw_coin_registry::tw_derivation::TWDerivation;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_keypair::ffi::privkey::tw_private_key_get_public_key_by_type;
use tw_keypair::test_utils::tw_private_key_helper::TWPrivateKeyHelper;
use tw_keypair::test_utils::tw_public_key_helper::TWPublicKeyHelper;
use tw_keypair::tw::PublicKeyType;
use tw_memory::test_utils::tw_data_helper::TWDataHelper;
use tw_memory::test_utils::tw_string_helper::TWStringHelper;
use tw_memory::test_utils::tw_wrapper::{TWWrapper, WithDestructor};

pub type TWAnyAddressHelper = TWWrapper<TWAnyAddress>;

impl WithDestructor for TWAnyAddress {
    fn destructor() -> unsafe extern "C" fn(*mut Self) {
        tw_any_address_delete
    }
}

pub fn test_address_derive(coin: CoinType, private_key: &str, address: &str) {
    let coin_item = get_coin_item(coin).unwrap();

    let private_key = TWPrivateKeyHelper::with_hex(private_key);
    let public_key = TWPublicKeyHelper::wrap(unsafe {
        tw_private_key_get_public_key_by_type(private_key.ptr(), coin_item.public_key_type as u32)
    });

    let any_address = TWAnyAddressHelper::wrap(unsafe {
        tw_any_address_create_with_public_key_derivation(
            public_key.ptr(),
            coin as u32,
            TWDerivation::Default as u32,
        )
    });

    let actual = TWStringHelper::wrap(unsafe { tw_any_address_description(any_address.ptr()) });
    assert_eq!(actual.to_string().unwrap(), address);
}

pub fn test_address_normalization(coin: CoinType, denormalized: &str, normalized: &str) {
    let expected = normalized;
    let denormalized = TWStringHelper::create(denormalized);

    let any_address = TWAnyAddressHelper::wrap(unsafe {
        tw_any_address_create_with_string(denormalized.ptr(), coin as u32)
    });

    let normalized = TWStringHelper::wrap(unsafe { tw_any_address_description(any_address.ptr()) });

    assert_eq!(normalized.to_string(), Some(expected.to_string()));
}

pub fn test_address_valid(coin: CoinType, address: &str) {
    let addr = TWStringHelper::create(address);
    assert!(
        unsafe { tw_any_address_is_valid(addr.ptr(), coin as u32) },
        "'{}' expected to be valid",
        address
    );
}

pub fn test_address_invalid(coin: CoinType, address: &str) {
    let addr = TWStringHelper::create(address);
    assert!(
        !unsafe { tw_any_address_is_valid(addr.ptr(), coin as u32) },
        "'{}' expected to be invalid",
        address
    );
}

pub fn test_address_get_data(coin: CoinType, address: &str, data_hex: &str) {
    let address_str = TWStringHelper::create(address);
    let any_address = TWAnyAddressHelper::wrap(unsafe {
        tw_any_address_create_with_string(address_str.ptr(), coin as u32)
    });

    let actual_data = TWDataHelper::wrap(unsafe { tw_any_address_data(any_address.ptr()) })
        .to_vec()
        .unwrap_or_else(|| panic!("!tw_any_address_data"));
    assert_eq!(
        actual_data.to_hex(),
        // Decode and encode again to allow to use `0x` and non-prefixed data hexes.
        data_hex.decode_hex().unwrap().to_hex()
    );
}

pub struct AddressCreateBech32WithPublicKey<'a> {
    pub coin: CoinType,
    pub private_key: &'a str,
    pub public_key_type: PublicKeyType,
    pub hrp: &'a str,
    pub expected: &'a str,
}

pub fn test_address_create_bech32_with_public_key(input: AddressCreateBech32WithPublicKey<'_>) {
    let private_key = TWPrivateKeyHelper::with_hex(input.private_key);
    let public_key = TWPublicKeyHelper::wrap(unsafe {
        tw_private_key_get_public_key_by_type(private_key.ptr(), input.public_key_type as u32)
    });
    let hrp = TWStringHelper::create(input.hrp);

    let any_address = TWAnyAddressHelper::wrap(unsafe {
        tw_any_address_create_bech32_with_public_key(public_key.ptr(), input.coin as u32, hrp.ptr())
    });

    let actual = TWStringHelper::wrap(unsafe { tw_any_address_description(any_address.ptr()) });
    assert_eq!(actual.to_string(), Some(input.expected.to_string()));
}

pub struct AddressBech32IsValid<'a> {
    pub coin: CoinType,
    pub address: &'a str,
    pub hrp: &'a str,
}

pub fn test_address_bech32_is_valid(input: AddressBech32IsValid<'_>) {
    let address_str = TWStringHelper::create(input.address);
    let hrp = TWStringHelper::create(input.hrp);
    // Should be valid even though Osmosis chain has `osmo` default hrp.
    let result =
        unsafe { tw_any_address_is_valid_bech32(address_str.ptr(), input.coin as u32, hrp.ptr()) };
    assert!(result);
}

pub struct AddressCreateBase58WithPublicKey<'a> {
    pub coin: CoinType,
    pub public_key: &'a str,
    pub public_key_type: PublicKeyType,
    pub p2pkh: u8,
    pub p2sh: u8,
    pub expected: &'a str,
}

pub fn test_address_create_base58_with_public_key(input: AddressCreateBase58WithPublicKey<'_>) {
    let public_key = TWPublicKeyHelper::with_hex(input.public_key, input.public_key_type);

    let any_address = TWAnyAddressHelper::wrap(unsafe {
        tw_any_address_create_base58_with_public_key(
            public_key.ptr(),
            input.coin as u32,
            input.p2pkh,
            input.p2sh,
        )
    });

    let actual = TWStringHelper::wrap(unsafe { tw_any_address_description(any_address.ptr()) });
    assert_eq!(actual.to_string(), Some(input.expected.to_string()));
}

pub struct AddressBase58IsValid<'a> {
    pub coin: CoinType,
    pub address: &'a str,
    pub p2pkh: u8,
    pub p2sh: u8,
}

pub fn test_address_base58_is_valid(input: AddressBase58IsValid<'_>) {
    // First, check if the address is valid.
    let addr_str = TWStringHelper::create(input.address);
    let is_valid = unsafe {
        tw_any_address_is_valid_base58(addr_str.ptr(), input.coin as u32, input.p2pkh, input.p2sh)
    };
    assert!(is_valid, "!tw_any_address_is_valid_base58");
}
