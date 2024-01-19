// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::test_utils::address_utils::{
    test_address_get_data, test_address_invalid, test_address_normalization, test_address_valid,
};
use tw_coin_registry::coin_type::CoinType;

#[test]
fn test_bitcoin_address_normalization() {
    test_address_normalization(
        CoinType::Bitcoin,
        "19cAJn4Ms8jodBBGtroBNNpCZiHAWGAq7X",
        "19cAJn4Ms8jodBBGtroBNNpCZiHAWGAq7X",
    );
}

#[test]
fn test_bitcoin_address_is_valid() {
    test_address_valid(CoinType::Bitcoin, "1MrZNGN7mfWZiZNQttrzHjfw72jnJC2JNx");
    test_address_valid(
        CoinType::Bitcoin,
        "bc1qunq74p3h8425hr6wllevlvqqr6sezfxj262rff",
    );
    test_address_valid(
        CoinType::Bitcoin,
        "bc1pwse34zfpvt344rvlt7tw0ngjtfh9xasc4q03avf0lk74jzjpzjuqaz7ks5",
    );
}

#[test]
fn test_bitcoin_address_invalid() {
    test_address_invalid(
        CoinType::Bitcoin,
        "0xb16db98b365b1f89191996942612b14f1da4bd5f",
    );
}

#[test]
fn test_bitcoin_address_get_data() {
    test_address_get_data(
        CoinType::Bitcoin,
        "1Bp9U1ogV3A14FMvKbRJms7ctyso4Z4Tcx",
        "00769bdff96a02f9135a1d19b749db6a78fe07dc90",
    );
}
