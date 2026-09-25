// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_number::U256;
use tw_ton_sdk::address::address_data::AddressData;
use tw_ton_sdk::address::user_friendly_address::UserFriendlyAddress;
use tw_ton_sdk::boc::BagOfCells;

/// In this test we parse a TON internal transfer message encoded as BoC.
#[test]
fn test_cell_parse_internal_transfer_message() {
    let boc = BagOfCells::parse_base64(
        "te6cckEBAQEAOgAAcGIARUMTww0u7LZO2ecEA9iRbS9qOcmmc6tFLaWOGw5dlFdEAAAAAAAAAAAAAAAAAAAAAAAAAAAAfRXk0w==",
    )
    .unwrap();
    let cell = boc.single_root().unwrap();
    let mut parser = cell.parser();

    let bit_0 = parser.load_bit().unwrap();
    let ihr_disabled = parser.load_bit().unwrap();
    let bounce = parser.load_bit().unwrap();
    let bounced = parser.load_bit().unwrap();
    let src_addr = parser.load_address().unwrap();
    let dest_addr = parser.load_address().unwrap();
    let value = parser.load_coins().unwrap();
    let currency_collections = parser.load_bit().unwrap();
    let ihr_fees = parser.load_coins().unwrap();
    let fwd_fees = parser.load_coins().unwrap();
    let created_lt = parser.load_u64(64).unwrap();
    let created_at = parser.load_u32(32).unwrap();
    let contains_state_init = parser.load_bit().unwrap();
    let contains_data = parser.load_bit().unwrap();

    parser.ensure_empty().expect("Must be read fully");

    let expected_dest_addr =
        UserFriendlyAddress::from_base64_url("EQCKhieGGl3ZbJ2zzggHsSLaXtRzk0znVopbSxw2HLsorkdl")
            .unwrap();
    let expected_value = U256::from(9_223_372_036_854_775_808_u64);

    assert_eq!(bit_0, false);
    assert_eq!(ihr_disabled, true);
    assert_eq!(bounce, true);
    assert_eq!(bounced, false);
    assert_eq!(src_addr, AddressData::NULL);
    assert_eq!(dest_addr, expected_dest_addr.into_data());
    assert_eq!(value, expected_value);
    assert_eq!(currency_collections, false);
    assert_eq!(ihr_fees, U256::ZERO);
    assert_eq!(fwd_fees, U256::ZERO);
    assert_eq!(created_lt, 0);
    assert_eq!(created_at, 0);
    assert_eq!(contains_state_init, false);
    assert_eq!(contains_data, false);
}
