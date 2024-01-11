// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::coin_entry_ext::CoinEntryExt;
use tw_coin_entry::test_utils::test_context::TestCoinContext;
use tw_encoding::hex::DecodeHex;
use tw_ethereum::entry::EthereumEntry;

#[test]
fn test_sign_json() {
    let coin = TestCoinContext::default();

    let input_json = r#"{"chainId":"AQ==","gasPrice":"1pOkAA==","gasLimit":"Ugg=","toAddress":"0x7d8bf18C7cE84b3E175b339c4Ca93aEd1dD166F1","transaction":{"transfer":{"amount":"A0i8paFgAA=="}}})"#;
    let private_key = "17209af590a86462395d5881e60d11c7fa7d482cfb02b5a01b93c2eeef243543"
        .decode_hex()
        .unwrap();

    EthereumEntry
        .sign_json(&coin, input_json, private_key)
        .expect_err("'EthEntry::sign_json' is not supported yet");

    // Expected result - "f86a8084d693a400825208947d8bf18c7ce84b3e175b339c4ca93aed1dd166f1870348bca5a160008025a0fe5802b49e04c6b1705088310e133605ed8b549811a18968ad409ea02ad79f21a05bf845646fb1e1b9365f63a7fd5eb5e984094e3ed35c3bed7361aebbcbf41f10"
}
