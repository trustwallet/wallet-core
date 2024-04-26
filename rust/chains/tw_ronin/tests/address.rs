// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::str::FromStr;
use tw_ronin::address::Address;

#[test]
fn test_ronin_address_valid() {
    let normalized = "ronin:EC49280228b0D05Aa8e8b756503254e1eE7835ab";
    let valid = [
        normalized,
        "ronin:ec49280228b0d05aa8e8b756503254e1ee7835ab",
        "0xEC49280228b0D05Aa8e8b756503254e1eE7835ab",
        "ronin:0xEC49280228b0D05Aa8e8b756503254e1eE7835ab",
    ];

    for test in valid {
        let addr = Address::from_str(test).unwrap();
        assert_eq!(addr.to_string(), normalized);
    }
}

#[test]
fn test_ronin_address_invalid() {
    let invalid = [
        "EC49280228b0D05Aa8e8b756503254e1eE7835ab", // no prefix
        "ec49280228b0d05aa8e8b756503254e1ee7835ab", // no prefix
        "roni:EC49280228b0D05Aa8e8b756503254e1eE7835ab", // wrong prefix
        "ronin=EC49280228b0D05Aa8e8b756503254e1eE7835ab", // wrong prefix
        "0xronin:EC49280228b0D05Aa8e8b756503254e1eE7835ab", // wrong prefix
        "EC49280228b0D05Aa8e8b756503254e1eE7835",   // too short
        "ronin:EC49280228b0D05Aa8e8b756503254e1eE7835", // too short
        "ronin:ec49280228b0d05aa8e8b756503254e1ee7835", // too short
        "",                                         // empty
    ];

    for test in invalid {
        Address::from_str(test).unwrap_err();
    }
}
