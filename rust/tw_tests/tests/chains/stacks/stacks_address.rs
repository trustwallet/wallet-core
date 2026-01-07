// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::test_utils::address_utils::{
    test_address_derive, test_address_get_data, test_address_invalid, test_address_normalization,
    test_address_valid, KeyType,
};
use tw_coin_registry::coin_type::CoinType;

#[test]
fn test_stacks_address_derive() {
    let private_key_hexstr = "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5";
    test_address_derive(
        CoinType::Stacks,
        KeyType::PrivateKey(private_key_hexstr),
        "SP2SF6ENB7636XSYSDNQ05X5T3SP4K2034A6D1G43K",
    );

    let private_key_hexstr = "4646464646464646464646464646464646464646464646464646464646464646";
    test_address_derive(
        CoinType::Stacks,
        KeyType::PrivateKey(private_key_hexstr),
        "SP2TYS424BPZM2TR8TKEAFQDTA1441AAVR9W6EVSKY",
    );

    let private_key_hexstr = "5ee478841eee47c23c164530d3df3469168431af52569c3dcc5d715666a7321b";
    test_address_derive(
        CoinType::Stacks,
        KeyType::PrivateKey(private_key_hexstr),
        "SP2VR2M51AC00TKNMF08XE9WSJDZ3WT4QGQNEQPDQA",
    );

    let private_key_hexstr = "0000000000000000000000000000000000000000000000000000000000000001";
    test_address_derive(
        CoinType::Stacks,
        KeyType::PrivateKey(private_key_hexstr),
        "SP2STHWXQ8368SDN2MJGE4BMDKMCHZ2GSVTS1X0BPM",
    );

    let private_key_hexstr = "1111111111111111111111111111111111111111111111111111111111111111";
    test_address_derive(
        CoinType::Stacks,
        KeyType::PrivateKey(private_key_hexstr),
        "SP2VY74M5227FDVHREWPH773F5Y1W1ED8WXY3RAVG4",
    );
}

#[test]
fn test_stacks_address_normalization() {
    test_address_normalization(
        CoinType::Stacks,
        "SP2VR2M51ACOoTKNMF08XE9WSJDZ3WT4QGQNEQPDQA",
        "SP2VR2M51AC00TKNMF08XE9WSJDZ3WT4QGQNEQPDQA",
    );
    test_address_normalization(
        CoinType::Stacks,
        "SP2VR2M5IAC00TKNMF08XE9WSJDZ3WT4QGQNEQPDQA",
        "SP2VR2M51AC00TKNMF08XE9WSJDZ3WT4QGQNEQPDQA",
    );
    test_address_normalization(
        CoinType::Stacks,
        "SP2VR2M5iAC00TKNMF08XE9WSJDZ3WT4QGQNEQPDQA",
        "SP2VR2M51AC00TKNMF08XE9WSJDZ3WT4QGQNEQPDQA",
    );
    test_address_normalization(
        CoinType::Stacks,
        "SP2VR2M5LAC00TKNMF08XE9WSJDZ3WT4QGQNEQPDQA",
        "SP2VR2M51AC00TKNMF08XE9WSJDZ3WT4QGQNEQPDQA",
    );
    test_address_normalization(
        CoinType::Stacks,
        "SP2VR2M5lAC00TKNMF08XE9WSJDZ3WT4QGQNEQPDQA",
        "SP2VR2M51AC00TKNMF08XE9WSJDZ3WT4QGQNEQPDQA",
    );
}

#[test]
fn test_stacks_address_is_valid() {
    test_address_valid(
        CoinType::Stacks,
        "SP2VR2M51AC00TKNMF08XE9WSJDZ3WT4QGQNEQPDQA",
    );
}

#[test]
fn test_stacks_address_invalid() {
    test_address_invalid(
        CoinType::Stacks,
        "AP2VR2M51AC00TKNMF08XE9WSJDZ3WT4QGQNEQPDQA",
    );
    test_address_invalid(
        CoinType::Stacks,
        "SX2VR2M51AC00TKNMF08XE9WSJDZ3WT4QGQNEQPDQA",
    );
    test_address_invalid(
        CoinType::Stacks,
        "SM2VR2M51AC00TKNMF08XE9WSJDZ3WT4QGQNEQPDQA",
    );
}

#[test]
fn test_stacks_address_get_data() {
    test_address_get_data(
        CoinType::Stacks,
        "SP2VR2M51AC00TKNMF08XE9WSJDZ3WT4QGQNEQPDQA",
        "0xf02a142a6001a9d68f023ae4f3326fc7cd12f0bd",
    );
}
