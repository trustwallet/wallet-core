// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::str::FromStr;
use tw_encoding::hex;
use tw_encoding::hex::ToHex;
use tw_hash::H256;
use tw_number::U256;

struct TestCase {
    num_str: &'static str,
    little_endian_compact: &'static str,
    little_endian: &'static str,
    big_endian_compact: &'static str,
    big_endian: &'static str,
}

const TEST_CASES: &[TestCase] = &[
    TestCase {
        num_str: "0",
        little_endian_compact: "",
        little_endian: "0000000000000000000000000000000000000000000000000000000000000000",
        big_endian_compact: "",
        big_endian: "0000000000000000000000000000000000000000000000000000000000000000",
    },
    TestCase {
        num_str: "1",
        little_endian_compact: "01",
        little_endian: "0100000000000000000000000000000000000000000000000000000000000000",
        big_endian_compact: "01",
        big_endian: "0000000000000000000000000000000000000000000000000000000000000001",
    },
    TestCase {
        num_str: "7",
        little_endian_compact: "07",
        little_endian: "0700000000000000000000000000000000000000000000000000000000000000",
        big_endian_compact: "07",
        big_endian: "0000000000000000000000000000000000000000000000000000000000000007",
    },
    TestCase {
        num_str: "100",
        little_endian_compact: "64",
        little_endian: "6400000000000000000000000000000000000000000000000000000000000000",
        big_endian_compact: "64",
        big_endian: "0000000000000000000000000000000000000000000000000000000000000064",
    },
    TestCase {
        num_str: "255",
        little_endian_compact: "ff",
        little_endian: "ff00000000000000000000000000000000000000000000000000000000000000",
        big_endian_compact: "ff",
        big_endian: "00000000000000000000000000000000000000000000000000000000000000ff",
    },
    TestCase {
        num_str: "256",
        little_endian_compact: "0001",
        little_endian: "0001000000000000000000000000000000000000000000000000000000000000",
        big_endian_compact: "0100",
        big_endian: "0000000000000000000000000000000000000000000000000000000000000100",
    },
    TestCase {
        num_str: "1000000",
        little_endian_compact: "40420f",
        little_endian: "40420f0000000000000000000000000000000000000000000000000000000000",
        big_endian_compact: "0f4240",
        big_endian: "00000000000000000000000000000000000000000000000000000000000f4240",
    },
    TestCase {
        num_str: "20000000000",
        little_endian_compact: "00c817a804",
        little_endian: "00c817a804000000000000000000000000000000000000000000000000000000",
        big_endian_compact: "04a817c800",
        big_endian: "00000000000000000000000000000000000000000000000000000004a817c800",
    },
    TestCase {
        num_str: "1311768467463790320",
        little_endian_compact: "f0debc9a78563412",
        little_endian: "f0debc9a78563412000000000000000000000000000000000000000000000000",
        big_endian_compact: "123456789abcdef0",
        big_endian: "000000000000000000000000000000000000000000000000123456789abcdef0",
    },
    TestCase {
        num_str: "94522879700260683142460330790866415",
        little_endian_compact: "efcdab89674523f1debc9a78563412",
        little_endian: "efcdab89674523f1debc9a785634120000000000000000000000000000000000",
        big_endian_compact: "123456789abcdef123456789abcdef",
        big_endian: "0000000000000000000000000000000000123456789abcdef123456789abcdef",
    },
    TestCase {
        num_str: "91343852333181432387730302044767688728495783936",
        little_endian_compact: "0000000000000000000000000000000000000010",
        little_endian: "0000000000000000000000000000000000000010000000000000000000000000",
        big_endian_compact: "1000000000000000000000000000000000000000",
        big_endian: "0000000000000000000000001000000000000000000000000000000000000000",
    },
    TestCase {
        num_str: "18515461264373351373200002665853028612451056578545711640558177340181847433846",
        little_endian_compact: "766263aa200659e163ff713c5da1e1036086677553fe9521bc39d90b3461ef28",
        little_endian: "766263aa200659e163ff713c5da1e1036086677553fe9521bc39d90b3461ef28",
        big_endian_compact: "28ef61340bd939bc2195fe537567866003e1a15d3c71ff63e1590620aa636276",
        big_endian: "28ef61340bd939bc2195fe537567866003e1a15d3c71ff63e1590620aa636276",
    },
];

#[test]
fn test_u256_big_endian_compact() {
    for test in TEST_CASES {
        let bytes = hex::decode(test.big_endian_compact).unwrap();
        let num = U256::from_big_endian_slice(bytes.as_slice()).unwrap();

        assert_eq!(num.to_string(), test.num_str);

        let actual_hex_encoded = hex::encode(num.to_big_endian_compact(), false);
        assert_eq!(actual_hex_encoded, test.big_endian_compact);
    }
}

#[test]
fn test_u256_big_endian() {
    for test in TEST_CASES {
        let bytes = H256::from_str(test.big_endian).unwrap();
        let num = U256::from_big_endian(bytes);

        assert_eq!(num.to_string(), test.num_str);

        let actual_bytes = num.to_big_endian();
        assert_eq!(actual_bytes, bytes);
    }
}

#[test]
fn test_u256_little_endian_compact() {
    for test in TEST_CASES {
        let bytes = hex::decode(test.little_endian_compact).unwrap();
        let num = U256::from_little_endian_slice(bytes.as_slice()).unwrap();

        assert_eq!(num.to_string(), test.num_str);

        let actual_hex_encoded = hex::encode(num.to_little_endian_compact(), false);
        assert_eq!(actual_hex_encoded, test.little_endian_compact);
    }
}

#[test]
fn test_u256_little_endian() {
    for test in TEST_CASES {
        let bytes = H256::from_str(test.little_endian).unwrap();
        let num = U256::from_little_endian(bytes);

        assert_eq!(num.to_string(), test.num_str);

        let actual_bytes = num.to_little_endian();
        assert_eq!(actual_bytes, bytes);
    }
}

#[test]
fn test_u256_from_str() {
    for test in TEST_CASES {
        let bytes = H256::from_str(test.little_endian).unwrap();
        let num = U256::from_little_endian(bytes);

        let actual = U256::from_str(test.num_str).unwrap();
        assert_eq!(num, actual);
    }
}

#[test]
fn test_u256_big_endian_min_len() {
    // 0x0100
    let num = U256::from(256_u64);

    let num_0 = num.to_big_endian_compact_min_len(0);
    assert_eq!(hex::encode(num_0, false), "0100");

    let num_1 = num.to_big_endian_compact_min_len(1);
    assert_eq!(hex::encode(num_1, false), "0100");

    let num_2 = num.to_big_endian_compact_min_len(2);
    assert_eq!(hex::encode(num_2, false), "0100");

    let num_3 = num.to_big_endian_compact_min_len(3);
    assert_eq!(num_3.to_hex(), "000100");

    let num_20 = num.to_big_endian_compact_min_len(20);
    assert_eq!(num_20.to_hex(), "0000000000000000000000000000000000000100");

    let num_32 = num.to_big_endian_compact_min_len(32);
    assert_eq!(
        num_32.to_hex(),
        "0000000000000000000000000000000000000000000000000000000000000100"
    );

    let num_33 = num.to_big_endian_compact_min_len(33);
    assert_eq!(
        num_33.to_hex(),
        "000000000000000000000000000000000000000000000000000000000000000100"
    );

    let num_64 = num.to_big_endian_compact_min_len(64);
    assert_eq!(
        num_64.to_hex(),
        "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000100"
    );
}
