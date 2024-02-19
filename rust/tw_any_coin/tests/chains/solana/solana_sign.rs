// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::borrow::Cow;
use tw_any_coin::test_utils::sign_utils::AnySignerHelper;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::base58::{self, Alphabet};
use tw_proto::Common::Proto::SigningError;
use tw_proto::Solana::Proto;

use tw_proto::Solana::Proto::mod_SigningInput::OneOftransaction_type as TransactionType;

fn b58(s: &'static str) -> Cow<'static, [u8]> {
    base58::decode(s, Alphabet::BITCOIN).unwrap().into()
}

#[test]
fn test_solana_sign_transfer() {
    let transfer = Proto::Transfer {
        recipient: "EN2sCsJ1WDV8UFqsiTXHcUPUxQ4juE71eCknHYYMifkd".into(),
        value: 42,
        ..Proto::Transfer::default()
    };
    let input = Proto::SigningInput {
        private_key: b58("A7psj2GW7ZMdY4E5hJq14KMeYg7HFjULSsWSrTXZLvYr"),
        recent_blockhash: "11111111111111111111111111111111".into(),
        transaction_type: TransactionType::transfer_transaction(transfer),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Solana, input);

    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.encoded, "3p2kzZ1DvquqC6LApPuxpTg5CCDVPqJFokGSnGhnBHrta4uq7S2EyehV1XNUVXp51D69GxGzQZUjikfDzbWBG2aFtG3gHT1QfLzyFKHM4HQtMQMNXqay1NAeiiYZjNhx9UvMX4uAQZ4Q6rx6m2AYfQ7aoMUrejq298q1wBFdtS9XVB5QTiStnzC7zs97FUEK2T4XapjF1519EyFBViTfHpGpnf5bfizDzsW9kYUtRDW1UC2LgHr7npgq5W9TBmHf9hSmRgM9XXucjXLqubNWE7HUMhbKjuBqkirRM");
    assert_eq!(output.unsigned_tx, "87PYsiS4MUU1UqXrsDoCBmD5FcKsXhwEBD8hc4zbq78yePu7bLENmbnmjmVbsj4VvaxnZhy4bERndPFzjSRH5WpwKwMLSCKvn9eSDmPESNcdkqne2UdMfWiFoq8ZeQBnF9h98dP8GM9kfzWPjvLmhjwuwA1E2k5WCtfii7LKQ34v6AtmFQGZqgdKiNqygP7ZKusHWGT8ZkTZ");
}
