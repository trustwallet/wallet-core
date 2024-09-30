// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::str::FromStr;
use tw_hash::H256;
use tw_ton_sdk::address::address_data::AddressData;
use tw_ton_sdk::address::raw_address::RawAddress;
use tw_ton_sdk::address::user_friendly_address::UserFriendlyAddress;

const WORKCHAIN: i32 = 0;
const ADDRESS_BYTES: &str = "e4d954ef9f4e1250a26b5bbad76a1cdd17cfd08babad6f4c23e372270aef6f76";

const RAW_ADDRESS: &str = "0:e4d954ef9f4e1250a26b5bbad76a1cdd17cfd08babad6f4c23e372270aef6f76";
const BOUNCEABLE_URL_ADDRESS: &str = "EQDk2VTvn04SUKJrW7rXahzdF8_Qi6utb0wj43InCu9vdjrR";
const BOUNCEABLE_ADDRESS: &str = "EQDk2VTvn04SUKJrW7rXahzdF8/Qi6utb0wj43InCu9vdjrR";

fn addr_data() -> AddressData {
    let bytes = H256::from_str(ADDRESS_BYTES).unwrap();
    AddressData::new(WORKCHAIN, bytes)
}

#[test]
fn test_raw_address_from_to_string() {
    let actual = RawAddress::from_str(RAW_ADDRESS).unwrap();
    let expected = RawAddress::from(addr_data());
    assert_eq!(actual, expected);
    let actual_encoded = actual.to_string();
    assert_eq!(actual_encoded, RAW_ADDRESS);
}

#[test]
fn test_user_friendly_address_from_to_url_string() {
    let actual = UserFriendlyAddress::from_base64_url(BOUNCEABLE_URL_ADDRESS).unwrap();
    let expected = UserFriendlyAddress::with_flags(addr_data(), true, false);
    assert_eq!(actual, expected);
    let actual_encoded = actual.to_base64_url();
    assert_eq!(actual_encoded, BOUNCEABLE_URL_ADDRESS);
}

#[test]
fn test_user_friendly_address_from_to_std_string() {
    let actual = UserFriendlyAddress::from_base64_std(BOUNCEABLE_ADDRESS).unwrap();
    let expected = UserFriendlyAddress::with_flags(addr_data(), true, false);
    assert_eq!(actual, expected);
    let actual_encoded = actual.to_base64_std();
    assert_eq!(actual_encoded, BOUNCEABLE_ADDRESS);
}
