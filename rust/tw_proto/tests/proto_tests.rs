// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::borrow::Cow;
use tw_proto::{deserialize, deserialize_prefixed, serialize, serialize_prefixed, Ethereum};

const SERIALIZED: [u8; 6] = [10, 1, 1, 18, 1, 11];
const SERIALIZED_PREFIXED: [u8; 7] = [6, 10, 1, 1, 18, 1, 11];

fn test_msg() -> Ethereum::Proto::SigningInput<'static> {
    Ethereum::Proto::SigningInput {
        chain_id: Cow::Owned(vec![1]),
        nonce: Cow::Owned(vec![11]),
        ..Ethereum::Proto::SigningInput::default()
    }
}

#[test]
fn test_serialize() {
    let msg = test_msg();
    let serialized = serialize(&msg).unwrap();
    // Must not contain a prefix.
    assert_eq!(serialized, SERIALIZED);
}

#[test]
fn test_deserialize() {
    let actual: Ethereum::Proto::SigningInput = deserialize(&SERIALIZED).unwrap();
    assert_eq!(actual, test_msg());
}

#[test]
fn test_serialize_prefixed() {
    let msg = test_msg();
    let serialized = serialize_prefixed(&msg).unwrap();
    // Must not contain a prefix.
    assert_eq!(serialized, SERIALIZED_PREFIXED);
}

#[test]
fn test_deserialize_prefixed() {
    let actual: Ethereum::Proto::SigningInput = deserialize_prefixed(&SERIALIZED_PREFIXED).unwrap();
    assert_eq!(actual, test_msg());
}

#[test]
fn test_serialize_deserialize() {
    let serialized = [
        10, 1, 1, 18, 1, 0, 34, 5, 9, 199, 101, 36, 0, 42, 3, 1, 48, 185, 66, 42, 48, 120, 54, 98,
        49, 55, 53, 52, 55, 52, 101, 56, 57, 48, 57, 52, 99, 52, 52, 100, 97, 57, 56, 98, 57, 53,
        52, 101, 101, 100, 101, 97, 99, 52, 57, 53, 50, 55, 49, 100, 48, 102, 74, 32, 96, 141, 203,
        23, 66, 187, 63, 183, 174, 192, 2, 7, 78, 52, 32, 228, 250, 183, 208, 12, 206, 215, 156,
        205, 172, 83, 237, 91, 39, 19, 129, 81, 82, 56, 18, 54, 10, 42, 48, 120, 53, 51, 50, 50,
        98, 51, 52, 99, 56, 56, 101, 100, 48, 54, 57, 49, 57, 55, 49, 98, 102, 53, 50, 97, 55, 48,
        52, 55, 52, 52, 56, 102, 48, 102, 52, 101, 102, 99, 56, 52, 18, 8, 27, 193, 109, 103, 78,
        200, 0, 0,
    ];
    let privkey = [
        96u8, 141, 203, 23, 66, 187, 63, 183, 174, 192, 2, 7, 78, 52, 32, 228, 250, 183, 208, 12,
        206, 215, 156, 205, 172, 83, 237, 91, 39, 19, 129, 81,
    ];

    let expected_erc20 = Ethereum::Proto::mod_Transaction::ERC20Transfer {
        to: Cow::from("0x5322b34c88ed0691971bf52a7047448f0f4efc84"),
        amount: Cow::Borrowed(&[27u8, 193, 109, 103, 78, 200, 0, 0]),
    };
    let expected_tx = Ethereum::Proto::Transaction {
        transaction_oneof: Ethereum::Proto::mod_Transaction::OneOftransaction_oneof::erc20_transfer(
            expected_erc20,
        ),
    };
    let expected = Ethereum::Proto::SigningInput {
        chain_id: Cow::Borrowed(&[1]),
        nonce: Cow::Borrowed(&[0]),
        gas_price: Cow::Borrowed(&[9, 199, 101, 36, 0]),
        gas_limit: Cow::Borrowed(&[1, 48, 185]),
        to_address: Cow::from("0x6b175474e89094c44da98b954eedeac495271d0f"),
        private_key: Cow::Borrowed(&privkey),
        transaction: Some(expected_tx),
        ..Ethereum::Proto::SigningInput::default()
    };

    let actual: Ethereum::Proto::SigningInput = deserialize(&serialized).unwrap();
    assert_eq!(actual, expected);

    let actual_serialized = serialize(&actual).unwrap();
    assert_eq!(actual_serialized, serialized);
}
