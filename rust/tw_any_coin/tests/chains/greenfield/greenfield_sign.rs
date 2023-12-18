// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_any_coin::test_utils::sign_utils::AnySignerHelper;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::DecodeHex;
use tw_proto::Common::Proto::SigningError;
use tw_proto::Greenfield::Proto;
use Proto::mod_Message::OneOfmessage_oneof as MessageEnum;

const PRIVATE_KEY_15560: &str = "9066aa168c379a403becb235c15e7129c133c244e56a757ab07bc369288bcab0";

fn make_amount<'a>(denom: &'a str, amount: &'a str) -> Proto::Amount<'a> {
    Proto::Amount {
        denom: denom.into(),
        amount: amount.into(),
    }
}

/// Successfully broadcasted https://greenfieldscan.com/tx/0x9f895cf2dd64fb1f428cefcf2a6585a813c3540fc9fe1ef42db1da2cb1df55ab
#[test]
fn test_greenfield_sign_send_order_9f895c() {
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
        private_key: PRIVATE_KEY_15560.decode_hex().unwrap().into(),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Greenfield, input);

    let expected = r#"{"mode":"BROADCAST_MODE_SYNC","tx_bytes":"CpQBCpEBChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEnEKKjB4OWQxZDk3YURGY2QzMjRCYmQ2MDNEMzg3MkJENzhlMDQwOTg1MTBiMRIqMHgyODBiMjdmMzY3NmRiMUM0NDc1RUUxMEY3NUQ1MTBFYjUyN2ZkMTU1GhcKA0JOQhIQMTAwMDAwMDAwMDAwMDAwMBJ5ClgKTQomL2Nvc21vcy5jcnlwdG8uZXRoLmV0aHNlY3AyNTZrMS5QdWJLZXkSIwohAnnvNAZNoQ2wRjxwSAYWugIHA+w6RQJt73vr0ggvXW/IEgUKAwjIBRgCEh0KFwoDQk5CEhAyMDAwMDAwMDAwMDAwMDAwEMCaDBpByzpGhKmRAUo4egSoW1kifrt5VnwgJa3cspa0yoVun4ENO1JvKg0PrWrRsSazuVFvizvgIKfMqcA8489H9BmbbRs="}"#;
    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.serialized, expected);
}
