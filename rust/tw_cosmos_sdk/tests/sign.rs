// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::borrow::Cow;
use tw_coin_entry::test_utils::test_context::TestCoinContext;
use tw_cosmos_sdk::context::StandardCosmosContext;
use tw_cosmos_sdk::modules::tx_builder::TxBuilder;
use tw_cosmos_sdk::test_utils::sign_utils::{
    test_sign_json, test_sign_json_error, test_sign_protobuf, test_sign_protobuf_error,
    TestErrorInput, TestInput,
};
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_keypair::tw::PublicKeyType;
use tw_proto::Common::Proto::SigningError;
use tw_proto::Cosmos::Proto;
use tw_proto::Cosmos::Proto::mod_Message::OneOfmessage_oneof as MessageEnum;

fn account_1037_private_key() -> Cow<'static, [u8]> {
    "80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005"
        .decode_hex()
        .unwrap()
        .into()
}

fn account_546179_private_key() -> Cow<'static, [u8]> {
    "8bbec3772ddb4df68f3186440380c301af116d1422001c1877d6f5e4dba8c8af"
        .decode_hex()
        .unwrap()
        .into()
}

fn account_1366160_private_key() -> Cow<'static, [u8]> {
    "a498a9ee41af9bab5ef2a8be63d5c970135c3c109e70efc8c56c534e6636b433"
        .decode_hex()
        .unwrap()
        .into()
}

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
        private_key: account_1037_private_key(),
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
        private_key: account_1037_private_key(),
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
fn test_sign_raw_json_with_timeout() {
    let coin = TestCoinContext::default()
        .with_public_key_type(PublicKeyType::Secp256k1)
        .with_hrp("cosmos");

    let raw_json_msg = Proto::mod_Message::RawJSON {
        type_pb: "osmosis/poolmanager/split-amount-in".into(),
        value: r#"{
        "routes": [
          {
            "pools": [
              {
                "pool_id": "463",
                "token_out_denom": "ibc/1DC495FCEFDA068A3820F903EDBD78B942FBD204D7E93D3BA2B432E9669D1A59"
              },
              {
                "pool_id": "916",
                "token_out_denom": "ibc/573FCD90FACEE750F55A8864EF7D38265F07E5A9273FA0E8DAFD39951332B580"
              }
            ],
            "token_in_amount": "70000"
          },
          {
            "pools": [
              {
                "pool_id": "907",
                "token_out_denom": "ibc/573FCD90FACEE750F55A8864EF7D38265F07E5A9273FA0E8DAFD39951332B580"
              }
            ],
            "token_in_amount": "30000"
          }
        ],
        "sender": "osmo1qr7dhmvcqm4fnleaqel3gel4u20nk5rp9rwsae",
        "token_in_denom": "uosmo",
        "token_out_min_amount": "885297"
    }"#.into(),
    };
    let input = Proto::SigningInput {
        account_number: 24139,
        chain_id: "osmosis-1".into(),
        sequence: 191,
        fee: Some(make_fee(617438, make_amount("uosmo", "1853"))),
        private_key: account_1037_private_key(),
        messages: vec![make_message(MessageEnum::raw_json_message(raw_json_msg))],
        timeout_height: 13692007,
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
        tx: r#"{"mode":"block","tx":{"fee":{"amount":[{"amount":"1853","denom":"uosmo"}],"gas":"617438"},"memo":"","msg":[{"type":"osmosis/poolmanager/split-amount-in","value":{"routes":[{"pools":[{"pool_id":"463","token_out_denom":"ibc/1DC495FCEFDA068A3820F903EDBD78B942FBD204D7E93D3BA2B432E9669D1A59"},{"pool_id":"916","token_out_denom":"ibc/573FCD90FACEE750F55A8864EF7D38265F07E5A9273FA0E8DAFD39951332B580"}],"token_in_amount":"70000"},{"pools":[{"pool_id":"907","token_out_denom":"ibc/573FCD90FACEE750F55A8864EF7D38265F07E5A9273FA0E8DAFD39951332B580"}],"token_in_amount":"30000"}],"sender":"osmo1qr7dhmvcqm4fnleaqel3gel4u20nk5rp9rwsae","token_in_denom":"uosmo","token_out_min_amount":"885297"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"7gMxXwqzZDe5+h1i16q7A7CgGUtLl2+Q8/YaUZCeYvp8kISbBwD2SlNTpJtz1RLskzF2uNcDebo61HbcVn9dAw=="}],"timeout_height":"13692007"}}"#,
        signature: "ee03315f0ab36437b9fa1d62d7aabb03b0a0194b4b976f90f3f61a51909e62fa7c90849b0700f64a5353a49b73d512ec933176b8d70379ba3ad476dc567f5d03",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"7gMxXwqzZDe5+h1i16q7A7CgGUtLl2+Q8/YaUZCeYvp8kISbBwD2SlNTpJtz1RLskzF2uNcDebo61HbcVn9dAw=="}]"#,
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
        private_key: account_546179_private_key(),
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

#[test]
fn test_sign_direct() {
    let coin = TestCoinContext::default()
        .with_public_key_type(PublicKeyType::Secp256k1)
        .with_hrp("cosmos");

    let body_bytes = "0a89010a1c2f636f736d6f732e62616e6b2e763162657461312e4d736753656e6412690a2d636f736d6f733168736b366a727979716a6668703564686335357463396a74636b796778306570683664643032122d636f736d6f73317a743530617a7570616e716c66616d356166687633686578777975746e756b656834633537331a090a046d756f6e120131".decode_hex().unwrap();
    let auth_info_bytes = "0a500a460a1f2f636f736d6f732e63727970746f2e736563703235366b312e5075624b657912230a210257286ec3f37d33557bbbaa000b27744ac9023aa9967cae75a181d1ff91fa9dc512040a020801180812110a0b0a046d756f6e120332303010c09a0c".decode_hex().unwrap();
    let sign_direct = Proto::mod_Message::SignDirect {
        body_bytes: Cow::from(body_bytes),
        auth_info_bytes: Cow::from(auth_info_bytes),
    };
    let mut input = Proto::SigningInput {
        account_number: 1037,
        chain_id: "gaia-13003".into(),
        private_key: account_1037_private_key(),
        messages: vec![make_message(MessageEnum::sign_direct_message(sign_direct))],
        ..Proto::SigningInput::default()
    };

    // real-world tx: https://www.mintscan.io/cosmos/txs/817101F3D96314AD028733248B28BAFAD535024D7D2C8875D3FE31DC159F096B
    // curl -H 'Content-Type: application/json' --data-binary '{"tx_bytes": "Cr4BCr...1yKOU=", "mode": "BROADCAST_MODE_BLOCK"}' https://api.cosmos.network/cosmos/tx/v1beta1/txs
    // also similar TX: BCDAC36B605576C8182C2829C808B30A69CAD4959D5ED1E6FF9984ABF280D603
    test_sign_protobuf::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input: input.clone(),
        tx: r#"{"mode":"BROADCAST_MODE_BLOCK","tx_bytes":"CowBCokBChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEmkKLWNvc21vczFoc2s2anJ5eXFqZmhwNWRoYzU1dGM5anRja3lneDBlcGg2ZGQwMhItY29zbW9zMXp0NTBhenVwYW5xbGZhbTVhZmh2M2hleHd5dXRudWtlaDRjNTczGgkKBG11b24SATESZQpQCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohAlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3FEgQKAggBGAgSEQoLCgRtdW9uEgMyMDAQwJoMGkD54fQAFlekIAnE62hZYl0uQelh/HLv0oQpCciY5Dn8H1SZFuTsrGdu41PH1Uxa4woptCELi/8Ov9yzdeEFAC9H"}"#,
        signature: "f9e1f4001657a42009c4eb6859625d2e41e961fc72efd2842909c898e439fc1f549916e4ecac676ee353c7d54c5ae30a29b4210b8bff0ebfdcb375e105002f47",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"+eH0ABZXpCAJxOtoWWJdLkHpYfxy79KEKQnImOQ5/B9UmRbk7KxnbuNTx9VMWuMKKbQhC4v/Dr/cs3XhBQAvRw=="}]"#,
    });

    // `Transfer` doesn't support JSON serialization and signing.
    // Set the default fee to get `SigningError::Error_not_supported`.
    input.fee = Some(Proto::Fee::default());
    test_sign_json_error::<StandardCosmosContext>(TestErrorInput {
        coin: &coin,
        input,
        error: SigningError::Error_not_supported,
    });
}

#[test]
fn test_sign_direct_0a90010a() {
    let coin = TestCoinContext::default()
        .with_public_key_type(PublicKeyType::Secp256k1)
        .with_hrp("cosmos");

    // Transaction body has the following content:
    // https://github.com/trustwallet/wallet-core/blob/1382e3c8ac6d8e956e25c0475039f6c3988f9355/tests/chains/Cosmos/SignerTests.cpp#L327-L340
    let body_bytes = "0a90010a1c2f636f736d6f732e62616e6b2e763162657461312e4d736753656e6412700a2d636f736d6f7331706b707472653766646b6c366766727a6c65736a6a766878686c63337234676d6d6b38727336122d636f736d6f7331717970717870713971637273737a673270767871367273307a716733797963356c7a763778751a100a0575636f736d120731323334353637".decode_hex().unwrap();
    let auth_info_bytes = "0a0a0a0012040a020801180112130a0d0a0575636f736d12043230303010c09a0c"
        .decode_hex()
        .unwrap();
    let sign_direct = Proto::mod_Message::SignDirect {
        body_bytes: Cow::from(body_bytes),
        auth_info_bytes: Cow::from(auth_info_bytes),
    };
    let mut input = Proto::SigningInput {
        account_number: 1,
        chain_id: "cosmoshub-4".into(),
        private_key: account_1037_private_key(),
        messages: vec![make_message(MessageEnum::sign_direct_message(sign_direct))],
        ..Proto::SigningInput::default()
    };

    test_sign_protobuf::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input: input.clone(),
        tx: r#"{"mode":"BROADCAST_MODE_BLOCK","tx_bytes":"CpMBCpABChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEnAKLWNvc21vczFwa3B0cmU3ZmRrbDZnZnJ6bGVzamp2aHhobGMzcjRnbW1rOHJzNhItY29zbW9zMXF5cHF4cHE5cWNyc3N6ZzJwdnhxNnJzMHpxZzN5eWM1bHp2N3h1GhAKBXVjb3NtEgcxMjM0NTY3EiEKCgoAEgQKAggBGAESEwoNCgV1Y29zbRIEMjAwMBDAmgwaQEgXmSAlm4M5bz+OX1GtvvZ3fBV2wrZrp4A/Imd55KM7ASivB/siYJegmYiOKzQ82uwoEmFalNnG2BrHHDwDR2Y="}"#,
        signature: "48179920259b83396f3f8e5f51adbef6777c1576c2b66ba7803f226779e4a33b0128af07fb226097a099888e2b343cdaec2812615a94d9c6d81ac71c3c034766",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"SBeZICWbgzlvP45fUa2+9nd8FXbCtmungD8iZ3nkozsBKK8H+yJgl6CZiI4rNDza7CgSYVqU2cbYGsccPANHZg=="}]"#,
    });

    // `Transfer` doesn't support JSON serialization and signing.
    // Set the default fee to get `SigningError::Error_not_supported`.
    input.fee = Some(Proto::Fee::default());
    test_sign_json_error::<StandardCosmosContext>(TestErrorInput {
        coin: &coin,
        input,
        error: SigningError::Error_not_supported,
    });
}

#[test]
fn test_sign_vote() {
    let coin = TestCoinContext::default()
        .with_public_key_type(PublicKeyType::Secp256k1)
        .with_hrp("cosmos");

    let vote_msg = Proto::mod_Message::MsgVote {
        proposal_id: 77,
        voter: "cosmos1mry47pkga5tdswtluy0m8teslpalkdq07pswu4".into(),
        option: Proto::mod_Message::VoteOption::YES,
    };
    let input = Proto::SigningInput {
        account_number: 1366160,
        chain_id: "cosmoshub-4".into(),
        sequence: 0,
        fee: Some(make_fee(97681, make_amount("uatom", "2418"))),
        private_key: account_1366160_private_key(),
        messages: vec![make_message(MessageEnum::msg_vote(vote_msg))],
        ..Proto::SigningInput::default()
    };

    // Successfully broadcasted https://www.mintscan.io/cosmos/txs/2EFA054B842B1641B131137B13360F95164C6C1D51BB4A4AC6DE8F75F504AA4C
    test_sign_protobuf::<StandardCosmosContext>(TestInput {
            coin: &coin,
            input: input.clone(),
            tx: r#"{"mode":"BROADCAST_MODE_BLOCK","tx_bytes":"ClQKUgobL2Nvc21vcy5nb3YudjFiZXRhMS5Nc2dWb3RlEjMITRItY29zbW9zMW1yeTQ3cGtnYTV0ZHN3dGx1eTBtOHRlc2xwYWxrZHEwN3Bzd3U0GAESZQpOCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohAsv9teRyiTMiKU5gzwiD1D30MeEInSnstEep5tVQRarlEgQKAggBEhMKDQoFdWF0b20SBDI0MTgQkfsFGkA+Nb3NULc38quGC1x+8ZXry4w9mMX3IA7wUjFboTv7kVOwPlleIc8UqIsjVvKTUFnUuW8dlGQzNR1KkvbvZ1NA"}"#,
            signature: "3e35bdcd50b737f2ab860b5c7ef195ebcb8c3d98c5f7200ef052315ba13bfb9153b03e595e21cf14a88b2356f2935059d4b96f1d946433351d4a92f6ef675340",
            signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"Asv9teRyiTMiKU5gzwiD1D30MeEInSnstEep5tVQRarl"},"signature":"PjW9zVC3N/KrhgtcfvGV68uMPZjF9yAO8FIxW6E7+5FTsD5ZXiHPFKiLI1byk1BZ1LlvHZRkMzUdSpL272dTQA=="}]"#,
        });

    // `MsgVote` doesn't support JSON serialization and signing.
    test_sign_json_error::<StandardCosmosContext>(TestErrorInput {
        coin: &coin,
        input,
        error: SigningError::Error_not_supported,
    });
}

#[test]
fn test_vote_payload() {
    let coin = TestCoinContext::default()
        .with_public_key_type(PublicKeyType::Secp256k1)
        .with_hrp("cosmos");

    let mut vote_msg = Proto::mod_Message::MsgVote {
        proposal_id: 123,
        voter: "cosmos1mry47pkga5tdswtluy0m8teslpalkdq07pswu4".into(),
        option: Proto::mod_Message::VoteOption::_UNSPECIFIED,
    };

    let tests = [
        (Proto::mod_Message::VoteOption::ABSTAIN, "087b122d636f736d6f73316d72793437706b67613574647377746c7579306d387465736c70616c6b6471303770737775341802"),
        (Proto::mod_Message::VoteOption::NO, "087b122d636f736d6f73316d72793437706b67613574647377746c7579306d387465736c70616c6b6471303770737775341803"),
        (Proto::mod_Message::VoteOption::NO_WITH_VETO, "087b122d636f736d6f73316d72793437706b67613574647377746c7579306d387465736c70616c6b6471303770737775341804"),
        (Proto::mod_Message::VoteOption::_UNSPECIFIED, "087b122d636f736d6f73316d72793437706b67613574647377746c7579306d387465736c70616c6b647130377073777534"),
    ];

    for (option, expected) in tests {
        vote_msg.option = option;
        let payload =
            TxBuilder::<StandardCosmosContext>::vote_msg_from_proto(&coin, &vote_msg).unwrap();
        let actual = payload.to_proto().unwrap();
        assert_eq!(actual.value.to_hex(), expected);
    }
}

#[test]
fn test_error_missing_message() {
    let coin = TestCoinContext::default()
        .with_public_key_type(PublicKeyType::Secp256k1)
        .with_hrp("cosmos");

    let input = Proto::SigningInput {
        account_number: 1037,
        chain_id: "gaia-13003".into(),
        sequence: 8,
        fee: Some(make_fee(200000, make_amount("muon", "200"))),
        private_key: account_1037_private_key(),
        messages: Vec::default(),
        ..Proto::SigningInput::default()
    };

    test_sign_protobuf_error::<StandardCosmosContext>(TestErrorInput {
        coin: &coin,
        input: input.clone(),
        error: SigningError::Error_invalid_params,
    });

    test_sign_json_error::<StandardCosmosContext>(TestErrorInput {
        coin: &coin,
        input,
        error: SigningError::Error_invalid_params,
    });
}
