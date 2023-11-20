// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::ffi::tw_any_address::{
    tw_any_address_create_with_string, tw_any_address_data, tw_any_address_delete,
    tw_any_address_description, tw_any_address_is_valid, TWAnyAddress,
};
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::DecodeHex;
use tw_memory::test_utils::tw_data_helper::TWDataHelper;
use tw_memory::test_utils::tw_string_helper::TWStringHelper;
use tw_memory::test_utils::tw_wrapper::{TWWrapper, WithDestructor};

pub type TWAnyAddressHelper = TWWrapper<TWAnyAddress>;

impl WithDestructor for TWAnyAddress {
    fn destructor() -> unsafe extern "C" fn(*mut Self) {
        tw_any_address_delete
    }
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
    let address = TWStringHelper::create(address);
    assert!(unsafe { tw_any_address_is_valid(address.ptr(), coin as u32) });
}

pub fn test_address_invalid(coin: CoinType, address: &str) {
    let address = TWStringHelper::create(address);
    assert!(!unsafe { tw_any_address_is_valid(address.ptr(), coin as u32) });
}

pub fn test_address_get_data(coin: CoinType, address: &str, data_hex: &str) {
    let address_str = TWStringHelper::create(address);
    let any_address = TWAnyAddressHelper::wrap(unsafe {
        tw_any_address_create_with_string(address_str.ptr(), coin as u32)
    });

    let actual_data = TWDataHelper::wrap(unsafe { tw_any_address_data(any_address.ptr()) });
    assert_eq!(actual_data.to_vec(), Some(data_hex.decode_hex().unwrap()));
}
