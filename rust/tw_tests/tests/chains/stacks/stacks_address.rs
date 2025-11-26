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
    let private_key_hexstr = "5ee478841eee47c23c164530d3df3469168431af52569c3dcc5d715666a7321b";
    //let private_key = tw_keypair::ecdsa::secp256k1::PrivateKey::from();

    test_address_derive(
        CoinType::Stacks,
        KeyType::PrivateKey(private_key_hexstr),
        "SP2VR2M51AC00TKNMF08XE9WSJDZ3WT4QGQNEQPDQA",
    );
}

#[test]
fn test_stacks_address_normalization() {
    test_address_normalization(
        CoinType::Stacks,
        "SP2VR2M51AC00TKNMF08XE9WSJDZ3WT4QGQNEQPDQA",
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
