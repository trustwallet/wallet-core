// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_number::U256;
use tw_ton_sdk::address::address_data::AddressData;
use tw_ton_sdk::address::user_friendly_address::UserFriendlyAddress;
use tw_ton_sdk::boc::BagOfCells;
use tw_ton_sdk::cell::cell_builder::CellBuilder;

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

/// Anycast (Maybe Anycast = 1) in addr_std must be rejected.
/// Before the fix, load_address() silently discarded the anycast bit and read the
/// next 8 bits as the workchain, allowing a crafted BoC to substitute an attacker-chosen
/// recipient address.
#[test]
fn test_load_address_rejects_anycast() {
    // Build: addr_std$10 anycast=1 workchain=0 hash=zeros (267 bits total).
    let mut builder = CellBuilder::new();
    builder.store_u8(2, 0b10).unwrap(); // addr_std tag
    builder.store_bit(true).unwrap(); // anycast = 1
    builder.store_u8(8, 0).unwrap(); // workchain = 0
    builder.store_slice(&[0u8; 32]).unwrap(); // hash
    let cell = builder.build().unwrap();

    // Must fail regardless of the specific error (anycast check or trailing-bits check).
    assert!(cell.parse_fully(|p| p.load_address()).is_err());
}
