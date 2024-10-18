// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::test_utils::address_utils::{
    test_address_derive, test_address_get_data, test_address_invalid, test_address_normalization,
    test_address_valid,
};
use tw_coin_registry::coin_type::CoinType;

#[test]
fn test_pactus_address_derive() {
    test_address_derive(
        CoinType::Pactus,
        "2134ae97465505dfd5a1fd05a8a0f146209c601eb3f1b0363b4cfe4b47ba1ab4",
        "pc1r7jkvfnegf0rf5ua05fzu9krjhjxcrrygl3v4nl",
    );
}

#[test]
fn test_pactus_address_normalization() {
    test_address_normalization(
        CoinType::Pactus,
        "pc1r7jkvfnegf0rf5ua05fzu9krjhjxcrrygl3v4nl",
        "pc1r7jkvfnegf0rf5ua05fzu9krjhjxcrrygl3v4nl",
    );
}

#[test]
fn test_pactus_address_is_valid() {
    test_address_valid(
        CoinType::Pactus,
        "000000000000000000000000000000000000000000",
    );
    test_address_valid(
        CoinType::Pactus,
        "pc1p0hrct7eflrpw4ccrttxzs4qud2axex4dcdzdfr",
    );
    test_address_valid(
        CoinType::Pactus,
        "pc1zzqkzzu4vyddss052as6c37qrdcfptegquw826x",
    );
    test_address_valid(
        CoinType::Pactus,
        "pc1r7jkvfnegf0rf5ua05fzu9krjhjxcrrygl3v4nl",
    );
}

#[test]
fn test_pactus_address_invalid() {
    test_address_invalid(CoinType::Pactus, "");
    test_address_invalid(CoinType::Pactus, "00");
    test_address_invalid(CoinType::Pactus, "not_proper_encoded");
    test_address_invalid(CoinType::Pactus, "pc1ioiooi");
    test_address_invalid(CoinType::Pactus, "pc19p72rf");
    test_address_invalid(
        CoinType::Pactus,
        "qc1z0hrct7eflrpw4ccrttxzs4qud2axex4dh8zz75",
    );
    test_address_invalid(
        CoinType::Pactus,
        "pc1p0hrct7eflrpw4ccrttxzs4qud2axex4dg8xaf5",
    ); // invalid checksum
    test_address_invalid(
        CoinType::Pactus,
        "pc1p0hrct7eflrpw4ccrttxzs4qud2axexs2dhdk8",
    ); // invalid length
    test_address_invalid(
        CoinType::Pactus,
        "pc1y0hrct7eflrpw4ccrttxzs4qud2axex4dksmred",
    ); // invalid type
}

#[test]
fn test_pactus_address_get_data() {
    test_address_get_data(
        CoinType::Pactus,
        "000000000000000000000000000000000000000000",
        "000000000000000000000000000000000000000000",
    );
    test_address_get_data(
        CoinType::Pactus,
        "pc1p0hrct7eflrpw4ccrttxzs4qud2axex4dcdzdfr",
        "017dc785fb29f8c2eae3035acc28541c6aba6c9aad",
    );
    test_address_get_data(
        CoinType::Pactus,
        "pc1zzqkzzu4vyddss052as6c37qrdcfptegquw826x",
        "02102c2172ac235b083e8aec3588f8036e1215e500",
    );
    test_address_get_data(
        CoinType::Pactus,
        "pc1r7jkvfnegf0rf5ua05fzu9krjhjxcrrygl3v4nl",
        "03f4acc4cf284bc69a73afa245c2d872bc8d818c88",
    );
}
