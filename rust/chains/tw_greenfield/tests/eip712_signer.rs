// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_coin_entry::test_utils::test_context::TestCoinContext;
use tw_encoding::hex::DecodeHex;
use tw_greenfield::modules::eip712_preimager::{Eip712Signer, Eip712TxPreimage};
use tw_greenfield::modules::tx_builder::TxBuilder;
use tw_misc::assert_eq_json;
use tw_proto::Greenfield::Proto;
use Proto::mod_Message::OneOfmessage_oneof as MessageEnum;

const SEND_ORDER_9F895C: &str = include_str!("data/send_order_9F895C.json");

const PUBLIC_KEY_15560: &str = "0279ef34064da10db0463c70480616ba020703ec3a45026def7bebd2082f5d6fc8";
// const PRIVATE_KEY_15560: &str = "9066aa168c379a403becb235c15e7129c133c244e56a757ab07bc369288bcab0";

fn make_amount<'a>(denom: &'a str, amount: &'a str) -> Proto::Amount<'a> {
    Proto::Amount {
        denom: denom.into(),
        amount: amount.into(),
    }
}

/// Testnet transaction.
#[test]
fn test_eip712_signer_encode_send() {
    let coin = TestCoinContext::default();

    let send_order = Proto::mod_Message::Send {
        from_address: "0x9d1d97aDFcd324Bbd603D3872BD78e04098510b1".into(),
        to_address: "0x280b27f3676db1C4475EE10F75D510Eb527fd155".into(),
        amounts: vec![make_amount("BNB", "1000000000000000")],
        ..Proto::mod_Message::Send::default()
    };

    let input = Proto::SigningInput {
        signing_mode: Proto::SigningMode::Eip712,
        account_number: 15560,
        eth_chain_id: "5600".into(),
        cosmos_chain_id: "greenfield_5600-1".into(),
        fee: Some(Proto::Fee {
            amounts: vec![make_amount("BNB", "2000000000000000")],
            gas: 200000,
        }),
        sequence: 2,
        messages: vec![Proto::Message {
            message_oneof: MessageEnum::send_coins_message(send_order),
        }],
        public_key: PUBLIC_KEY_15560.decode_hex().unwrap().into(),
        ..Proto::SigningInput::default()
    };

    let unsigned_tx = TxBuilder::unsigned_tx_from_proto(&coin, &input)
        .expect("Error on generating an unsigned transaction");

    let Eip712TxPreimage { eip712_tx, .. } =
        Eip712Signer::preimage_hash(&unsigned_tx).expect("Error on TX preimage");

    assert_eq_json!(eip712_tx, SEND_ORDER_9F895C);
}
