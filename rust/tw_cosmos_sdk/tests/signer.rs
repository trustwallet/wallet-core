// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_coin_entry::test_utils::test_context::TestCoinContext;
use tw_cosmos_sdk::context::StandardCosmosContext;
use tw_cosmos_sdk::modules::signer::TWSigner;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_keypair::tw::PublicKeyType;
use tw_misc::assert_json;
use tw_proto::Common::Proto::SigningError;
use tw_proto::Cosmos::Proto;
use tw_proto::Cosmos::Proto::mod_Message::OneOfmessage_oneof as MessageEnum;

fn make_fee(gas: u64, amount: Proto::Amount<'_>) -> Proto::Fee<'_> {
    Proto::Fee {
        amounts: vec![amount],
        gas,
    }
}

fn make_message(message_oneof: MessageEnum) -> Proto::Message {
    Proto::Message { message_oneof }
}

fn make_amount<'a>(denom: &'a str, amount: &'a str) -> Proto::Amount<'a> {
    Proto::Amount {
        denom: denom.into(),
        amount: amount.into(),
    }
}

#[test]
fn test_sign_protobuf_coin_send() {
    let coin = TestCoinContext::default()
        .with_public_key_type(PublicKeyType::Secp256k1)
        .with_hrp("cosmos");

    let send_msg = Proto::mod_Message::Send {
        from_address: "cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02".into(),
        to_address: "cosmos1zt50azupanqlfam5afhv3hexwyutnukeh4c573".into(),
        amounts: vec![make_amount("muon", "1")],
        ..Proto::mod_Message::Send::default()
    };

    let input = Proto::SigningInput {
        signing_mode: Proto::SigningMode::Protobuf,
        account_number: 1037,
        chain_id: "gaia-13003".into(),
        sequence: 8,
        fee: Some(make_fee(200000, make_amount("muon", "200"))),
        private_key: "80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005"
            .decode_hex()
            .unwrap()
            .into(),
        messages: vec![make_message(MessageEnum::send_coins_message(send_msg))],
        ..Proto::SigningInput::default()
    };

    let output = TWSigner::<StandardCosmosContext>::sign(&coin, input);

    assert_eq!(output.error, SigningError::OK);
    assert!(output.error_message.is_empty());
    assert_json!(
        output.serialized,
        r#"{"tx_bytes": "CowBCokBChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEmkKLWNvc21vczFoc2s2anJ5eXFqZmhwNWRoYzU1dGM5anRja3lneDBlcGg2ZGQwMhItY29zbW9zMXp0NTBhenVwYW5xbGZhbTVhZmh2M2hleHd5dXRudWtlaDRjNTczGgkKBG11b24SATESZQpQCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohAlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3FEgQKAggBGAgSEQoLCgRtdW9uEgMyMDAQwJoMGkD54fQAFlekIAnE62hZYl0uQelh/HLv0oQpCciY5Dn8H1SZFuTsrGdu41PH1Uxa4woptCELi/8Ov9yzdeEFAC9H", "mode": "BROADCAST_MODE_BLOCK"}"#
    );
    assert_eq!(output.signature.to_hex(), "f9e1f4001657a42009c4eb6859625d2e41e961fc72efd2842909c898e439fc1f549916e4ecac676ee353c7d54c5ae30a29b4210b8bff0ebfdcb375e105002f47");
}
