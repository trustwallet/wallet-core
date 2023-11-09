// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_coin_entry::test_utils::test_context::TestCoinContext;
use tw_cosmos_sdk::context::StandardCosmosContext;
use tw_cosmos_sdk::test_utils::{
    test_sign_json, test_sign_json_error, test_sign_protobuf, test_sign_protobuf_error,
    TestErrorInput, TestInput,
};
use tw_encoding::hex::DecodeHex;
use tw_keypair::tw::PublicKeyType;
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

fn make_height(revision_number: u64, revision_height: u64) -> Proto::Height {
    Proto::Height {
        revision_number,
        revision_height,
    }
}

#[test]
fn test_sign_coin_send() {
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

    test_sign_protobuf::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input: input.clone(),
        tx: r#"{"mode":"BROADCAST_MODE_BLOCK","tx_bytes":"CowBCokBChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEmkKLWNvc21vczFoc2s2anJ5eXFqZmhwNWRoYzU1dGM5anRja3lneDBlcGg2ZGQwMhItY29zbW9zMXp0NTBhenVwYW5xbGZhbTVhZmh2M2hleHd5dXRudWtlaDRjNTczGgkKBG11b24SATESZQpQCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohAlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3FEgQKAggBGAgSEQoLCgRtdW9uEgMyMDAQwJoMGkD54fQAFlekIAnE62hZYl0uQelh/HLv0oQpCciY5Dn8H1SZFuTsrGdu41PH1Uxa4woptCELi/8Ov9yzdeEFAC9H"}"#,
        signature: "f9e1f4001657a42009c4eb6859625d2e41e961fc72efd2842909c898e439fc1f549916e4ecac676ee353c7d54c5ae30a29b4210b8bff0ebfdcb375e105002f47",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"+eH0ABZXpCAJxOtoWWJdLkHpYfxy79KEKQnImOQ5/B9UmRbk7KxnbuNTx9VMWuMKKbQhC4v/Dr/cs3XhBQAvRw=="}]"#,
    });
    test_sign_json::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input,
        tx: r#"{"mode":"block","tx":{"fee":{"amount":[{"amount":"200","denom":"muon"}],"gas":"200000"},"memo":"","msg":[{"type":"cosmos-sdk/MsgSend","value":{"amount":[{"amount":"1","denom":"muon"}],"from_address":"cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02","to_address":"cosmos1zt50azupanqlfam5afhv3hexwyutnukeh4c573"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"/D74mdIGyIB3/sQvIboLTfS9P9EV/fYGrgHZE2/vNj9X6eM6e57G3atljNB+PABnRw3pTk51uXmhCFop8O/ZJg=="}]}}"#,
        signature: "fc3ef899d206c88077fec42f21ba0b4df4bd3fd115fdf606ae01d9136fef363f57e9e33a7b9ec6ddab658cd07e3c0067470de94e4e75b979a1085a29f0efd926",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"/D74mdIGyIB3/sQvIboLTfS9P9EV/fYGrgHZE2/vNj9X6eM6e57G3atljNB+PABnRw3pTk51uXmhCFop8O/ZJg=="}]"#,
    });
}

#[test]
fn test_sign_raw_json() {
    let coin = TestCoinContext::default()
        .with_public_key_type(PublicKeyType::Secp256k1)
        .with_hrp("cosmos");

    let raw_json_msg = Proto::mod_Message::RawJSON {
        type_pb: "test".into(),
        value: r#"{"test":"hello"}"#.into(),
    };
    let input = Proto::SigningInput {
        account_number: 1037,
        chain_id: "gaia-13003".into(),
        sequence: 8,
        fee: Some(make_fee(200000, make_amount("muon", "200"))),
        private_key: "80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005"
            .decode_hex()
            .unwrap()
            .into(),
        messages: vec![make_message(MessageEnum::raw_json_message(raw_json_msg))],
        ..Proto::SigningInput::default()
    };

    // `RawJSON` doesn't support Protobuf serialization and signing.
    test_sign_protobuf_error::<StandardCosmosContext>(TestErrorInput {
        coin: &coin,
        input: input.clone(),
        error: SigningError::Error_not_supported,
    });
    test_sign_json::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input,
        tx: r#"{"mode":"block","tx":{"fee":{"amount":[{"amount":"200","denom":"muon"}],"gas":"200000"},"memo":"","msg":[{"type":"test","value":{"test":"hello"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"qhxxCOMiVhP7e7Mx+98HUZI0t5DNOFXwzIqNQz+fT6hDKR/ebW0uocsYnE5CiBNEalmBcs5gSIJegNkHhgyEmA=="}]}}"#,
        signature: "aa1c7108e3225613fb7bb331fbdf07519234b790cd3855f0cc8a8d433f9f4fa843291fde6d6d2ea1cb189c4e428813446a598172ce6048825e80d907860c8498",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"qhxxCOMiVhP7e7Mx+98HUZI0t5DNOFXwzIqNQz+fT6hDKR/ebW0uocsYnE5CiBNEalmBcs5gSIJegNkHhgyEmA=="}]"#,
    });
}

#[test]
fn test_sign_ibc_transfer() {
    let coin = TestCoinContext::default()
        .with_public_key_type(PublicKeyType::Secp256k1)
        .with_hrp("cosmos");

    let transfer_tokens = Proto::mod_Message::Transfer {
        source_port: "transfer".into(),
        source_channel: "channel-141".into(),
        token: Some(make_amount("uatom", "100000")),
        sender: "cosmos1mky69cn8ektwy0845vec9upsdphktxt03gkwlx".into(),
        receiver: "osmo18s0hdnsllgcclweu9aymw4ngktr2k0rkvn7jmn".into(),
        timeout_height: Some(make_height(1, 8800000)),
        ..Proto::mod_Message::Transfer::default()
    };
    let input = Proto::SigningInput {
        account_number: 546179,
        chain_id: "cosmoshub-4".into(),
        sequence: 2,
        fee: Some(make_fee(500000, make_amount("uatom", "12500"))),
        private_key: "8bbec3772ddb4df68f3186440380c301af116d1422001c1877d6f5e4dba8c8af"
            .decode_hex()
            .unwrap()
            .into(),
        messages: vec![make_message(MessageEnum::transfer_tokens_message(
            transfer_tokens,
        ))],
        ..Proto::SigningInput::default()
    };

    // real-world tx: https://www.mintscan.io/cosmos/txs/817101F3D96314AD028733248B28BAFAD535024D7D2C8875D3FE31DC159F096B
    // curl -H 'Content-Type: application/json' --data-binary '{"tx_bytes": "Cr4BCr...1yKOU=", "mode": "BROADCAST_MODE_BLOCK"}' https://api.cosmos.network/cosmos/tx/v1beta1/txs
    // also similar TX: BCDAC36B605576C8182C2829C808B30A69CAD4959D5ED1E6FF9984ABF280D603
    test_sign_protobuf::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input: input.clone(),
        tx: r#"{"mode":"BROADCAST_MODE_BLOCK","tx_bytes":"Cr4BCrsBCikvaWJjLmFwcGxpY2F0aW9ucy50cmFuc2Zlci52MS5Nc2dUcmFuc2ZlchKNAQoIdHJhbnNmZXISC2NoYW5uZWwtMTQxGg8KBXVhdG9tEgYxMDAwMDAiLWNvc21vczFta3k2OWNuOGVrdHd5MDg0NXZlYzl1cHNkcGhrdHh0MDNna3dseCorb3NtbzE4czBoZG5zbGxnY2Nsd2V1OWF5bXc0bmdrdHIyazBya3ZuN2ptbjIHCAEQgI6ZBBJoClAKRgofL2Nvc21vcy5jcnlwdG8uc2VjcDI1NmsxLlB1YktleRIjCiEC7O9c5DejAsZ/lUaN5LMfNukR9GfX5qUrQcHhPh1WNkkSBAoCCAEYAhIUCg4KBXVhdG9tEgUxMjUwMBCgwh4aQK0HIWdFMk+C6Gi1KG/vELe1ffcc1aEWUIqz2t/ZhwqNNHxUUSp27wteiugHEMVTEIOBhs84t2gIcT/nD/1yKOU="}"#,
        signature: "ad07216745324f82e868b5286fef10b7b57df71cd5a116508ab3dadfd9870a8d347c54512a76ef0b5e8ae80710c55310838186cf38b76808713fe70ffd7228e5",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AuzvXOQ3owLGf5VGjeSzHzbpEfRn1+alK0HB4T4dVjZJ"},"signature":"rQchZ0UyT4LoaLUob+8Qt7V99xzVoRZQirPa39mHCo00fFRRKnbvC16K6AcQxVMQg4GGzzi3aAhxP+cP/XIo5Q=="}]"#,
    });
    // `Transfer` doesn't support JSON serialization and signing.
    test_sign_json_error::<StandardCosmosContext>(TestErrorInput {
        coin: &coin,
        input,
        error: SigningError::Error_not_supported,
    });
}
