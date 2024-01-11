// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::test_utils::test_context::TestCoinContext;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_greenfield::modules::eip712_signer::{Eip712Signer, Eip712TxPreimage};
use tw_greenfield::modules::tx_builder::TxBuilder;
use tw_misc::assert_eq_json;
use tw_proto::Greenfield::Proto;
use Proto::mod_Message::OneOfmessage_oneof as MessageEnum;

const SEND_ORDER_EIP712: &str = include_str!("data/send_order_eip712.json");
const TRANSFER_OUT_EIP712: &str = include_str!("data/transfer_out_eip712.json");

const PUBLIC_KEY_15560: &str = "0279ef34064da10db0463c70480616ba020703ec3a45026def7bebd2082f5d6fc8";

fn make_amount<'a>(denom: &'a str, amount: &'a str) -> Proto::Amount<'a> {
    Proto::Amount {
        denom: denom.into(),
        amount: amount.into(),
    }
}

/// Testnet transaction:
/// https://greenfieldscan.com/tx/0x9f895cf2dd64fb1f428cefcf2a6585a813c3540fc9fe1ef42db1da2cb1df55ab
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

    let Eip712TxPreimage { eip712_tx, tx_hash } =
        Eip712Signer::preimage_hash(&unsigned_tx).expect("Error on TX preimage");

    assert_eq_json!(eip712_tx, SEND_ORDER_EIP712);
    assert_eq!(
        tx_hash.to_hex(),
        "b8c62654582ca96b37ca94966199682bf70ed934e740d2f874ff54675a0ac344"
    );
}

#[test]
fn test_eip712_signer_encode_transfer_out() {
    let coin = TestCoinContext::default();

    let send_order = Proto::mod_Message::BridgeTransferOut {
        from_address: "0x9d1d97aDFcd324Bbd603D3872BD78e04098510b1".into(),
        to_address: "0x9d1d97aDFcd324Bbd603D3872BD78e04098510b1".into(),
        amount: Some(make_amount("BNB", "1000000000000000")),
        ..Proto::mod_Message::BridgeTransferOut::default()
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
            message_oneof: MessageEnum::bridge_transfer_out(send_order),
        }],
        public_key: PUBLIC_KEY_15560.decode_hex().unwrap().into(),
        ..Proto::SigningInput::default()
    };

    let unsigned_tx = TxBuilder::unsigned_tx_from_proto(&coin, &input)
        .expect("Error on generating an unsigned transaction");

    let Eip712TxPreimage { eip712_tx, tx_hash } =
        Eip712Signer::preimage_hash(&unsigned_tx).expect("Error on TX preimage");

    assert_eq_json!(eip712_tx, TRANSFER_OUT_EIP712);
    assert_eq!(
        tx_hash.to_hex(),
        "ea7731461041f5f652ab424bb767c670e484cfe1f4a85179deba8a6596873af4"
    );
}
