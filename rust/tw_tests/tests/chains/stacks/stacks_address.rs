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
        "SPBSKNASSGSQEFPBDDR1F9EGYDH4RG0S2HK8C10WR",
    );

    let private_key_hexstr = "4646464646464646464646464646464646464646464646464646464646464646";
    test_address_derive(
        CoinType::Stacks,
        KeyType::PrivateKey(private_key_hexstr),
        "SPQP90H2XQX0PP26MVJKXVEJG910AJPY2F1KPYCZG",
    );

    let private_key_hexstr = "5ee478841eee47c23c164530d3df3469168431af52569c3dcc5d715666a7321b";
    test_address_derive(
        CoinType::Stacks,
        KeyType::PrivateKey(private_key_hexstr),
        "SPY0N18AK006MXD3R27BJF6CKFRZ6H5W5XBNXKDTG",
    );

    let private_key_hexstr = "0000000000000000000000000000000000000000000000000000000000000001";
    test_address_derive(
        CoinType::Stacks,
        KeyType::PrivateKey(private_key_hexstr),
        "SPEMF7DT0SJ6BD8N4M3H2X3CX34FRM6EYP8F82XN0",
    );

    let private_key_hexstr = "1111111111111111111111111111111111111111111111111111111111111111";
    test_address_derive(
        CoinType::Stacks,
        KeyType::PrivateKey(private_key_hexstr),
        "SPZHS518GHVVEWE3Q5M9SRVSFGF0BKA77FGY2PW10",
    );
}

#[test]
fn test_stacks_address_normalization() {
    test_address_normalization(
        CoinType::Stacks,
        "SPBSKNASSGSQEFPBDDR1F9EGYDH4RGOS2HK8C1oWR",
        "SPBSKNASSGSQEFPBDDR1F9EGYDH4RG0S2HK8C10WR",
    );
    test_address_normalization(
        CoinType::Stacks,
        "SPBSKNASSGSQEFPBDDRIF9EGYDH4RG0S2HK8Ci0WR",
        "SPBSKNASSGSQEFPBDDR1F9EGYDH4RG0S2HK8C10WR",
    );
    test_address_normalization(
        CoinType::Stacks,
        "SPBSKNASSGSQEFPBDDRLF9EGYDH4RG0S2HK8Cl0WR",
        "SPBSKNASSGSQEFPBDDR1F9EGYDH4RG0S2HK8C10WR",
    );
}

#[test]
fn test_stacks_address_is_valid() {
    test_address_valid(
        CoinType::Stacks,
        "SPBSKNASSGSQEFPBDDR1F9EGYDH4RG0S2HK8C10WR",
    );
}

#[test]
fn test_stacks_address_invalid() {
    test_address_invalid(
        CoinType::Stacks,
        "ZPBSKNASSGSQEFPBDDR1F9EGYDH4RG0S2HK8C10WR",
    );
    test_address_invalid(
        CoinType::Stacks,
        "SXBSKNASSGSQEFPBDDR1F9EGYDH4RG0S2HK8C10WR",
    );
    test_address_invalid(
        CoinType::Stacks,
        "SPBSKNASSGSQEFPBDDR1F9EGYDH4RG0S2HK8C10WS",
    );
}

#[test]
fn test_stacks_address_get_data() {
    test_address_get_data(
        CoinType::Stacks,
        "SPBSKNASSGSQEFPBDDRLF9EGYDH4RG0S2HK8Cl0WR",
        "0x5e67556730cddcfb2dadc05e9743cd8931006451",
    );
    /*
    test_address_get_data(
        CoinType::Stacks,
        "SP2J6ZY48GV1EZ5V2V5RB9MP66SW86PYKKNRV9EJ7",
        "0xa46ff88886c2ef9762d970b4d2c63678835bd39d",
    );
    test_address_get_data(
        CoinType::Stacks,
        "SM2J6ZY48GV1EZ5V2V5RB9MP66SW86PYKKQVX8X0G",
        "0xa46ff88886c2ef9762d970b4d2c63678835bd39d",
    );
    test_address_get_data(
        CoinType::Stacks,
        "ST2J6ZY48GV1EZ5V2V5RB9MP66SW86PYKKQYAC0RQ",
        "0xa46ff88886c2ef9762d970b4d2c63678835bd39d",
    );
    test_address_get_data(
        CoinType::Stacks,
        "SN2J6ZY48GV1EZ5V2V5RB9MP66SW86PYKKP6D2ZK9",
        "0xa46ff88886c2ef9762d970b4d2c63678835bd39d",
    );
    */
}
