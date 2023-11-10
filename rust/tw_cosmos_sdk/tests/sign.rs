// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use std::borrow::Cow;
use tw_coin_entry::test_utils::test_context::TestCoinContext;
use tw_cosmos_sdk::context::StandardCosmosContext;
use tw_cosmos_sdk::test_utils::sign_utils::{
    test_sign_json, test_sign_json_error, test_sign_protobuf, test_sign_protobuf_error,
    TestErrorInput, TestInput,
};
use tw_encoding::hex::DecodeHex;
use tw_keypair::tw::PublicKeyType;
use tw_proto::Common::Proto::SigningError;
use tw_proto::Cosmos::Proto;
use tw_proto::Cosmos::Proto::mod_Message::OneOfmessage_oneof as MessageEnum;

fn account_1337_private_key() -> Cow<'static, [u8]> {
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

fn account_1290826_private_key() -> Cow<'static, [u8]> {
    "c7764249cdf77f8f1d840fa8af431579e5e41cf1af937e1e23afa22f3f4f0ccc"
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
        private_key: account_1337_private_key(),
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
        private_key: account_1337_private_key(),
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
        private_key: account_1337_private_key(),
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
        private_key: account_1337_private_key(),
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
fn test_staking_compounding_authz() {
    use Proto::mod_Message::mod_AuthGrant::OneOfgrant_type as ProtoGrantType;
    use Proto::mod_Message::mod_StakeAuthorization::OneOfvalidators as ProtoValidatorsType;

    let coin = TestCoinContext::default()
        .with_public_key_type(PublicKeyType::Secp256k1)
        .with_hrp("cosmos");

    let allow_list = Proto::mod_Message::mod_StakeAuthorization::Validators {
        address: vec!["cosmosvaloper1gjtvly9lel6zskvwtvlg5vhwpu9c9waw7sxzwx".into()],
    };
    let stake_authorization = Proto::mod_Message::StakeAuthorization {
        authorization_type: Proto::mod_Message::AuthorizationType::UNDELEGATE,
        validators: ProtoValidatorsType::allow_list(allow_list),
        ..Proto::mod_Message::StakeAuthorization::default()
    };
    let auth_grant = Proto::mod_Message::AuthGrant {
        granter: "cosmos13k0q0l7lg2kr32kvt7ly236ppldy8v9dzwh3gd".into(),
        grantee: "cosmos1fs7lu28hx5m9akm7rp0c2422cn8r2f7gurujhf".into(),
        grant_type: ProtoGrantType::grant_stake(stake_authorization),
        expiration: 1692309600,
    };
    let input = Proto::SigningInput {
        account_number: 1290826,
        chain_id: "cosmoshub-4".into(),
        sequence: 5,
        fee: Some(make_fee(96681, make_amount("uatom", "2418"))),
        private_key: account_1290826_private_key(),
        messages: vec![make_message(MessageEnum::auth_grant(auth_grant))],
        ..Proto::SigningInput::default()
    };

    // Successfully broadcasted https://www.mintscan.io/cosmos/txs/C4629BC7C88690518D8F448E7A8D239C9D63975B11F8E1CE2F95CC2ADA3CCF67
    test_sign_protobuf::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input: input.clone(),
        // TODO the signature has been updated according to the serialization of the `StakeAuthorization` message.
        // Previous: CvgBCvUBCh4vY29zbW9zLmF1dGh6LnYxYmV0YTEuTXNnR3JhbnQS0gEKLWNvc21vczEzazBxMGw3bGcya3IzMmt2dDdseTIzNnBwbGR5OHY5ZHp3aDNnZBItY29zbW9zMWZzN2x1MjhoeDVtOWFrbTdycDBjMjQyMmNuOHIyZjdndXJ1amhmGnIKaAoqL2Nvc21vcy5zdGFraW5nLnYxYmV0YTEuU3Rha2VBdXRob3JpemF0aW9uEjoSNgo0Y29zbW9zdmFsb3BlcjFnanR2bHk5bGVsNnpza3Z3dHZsZzV2aHdwdTljOXdhdzdzeHp3eCABEgYI4LD6pgYSZwpQCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohA/fcQw1hCVUx904t+kCXTiiziaLIY8lyssu1ENfzaN1KEgQKAggBGAUSEwoNCgV1YXRvbRIEMjQxOBCp8wUaQIFyfuijGKf87Hz61ZqxasfLI1PZnNge4RDq/tRyB/tZI6p80iGRqHecoV6+84EQkc9GTlNRQOSlApRCsivT9XI=
        tx: r#"{"mode":"BROADCAST_MODE_BLOCK","tx_bytes":"CvgBCvUBCh4vY29zbW9zLmF1dGh6LnYxYmV0YTEuTXNnR3JhbnQS0gEKLWNvc21vczEzazBxMGw3bGcya3IzMmt2dDdseTIzNnBwbGR5OHY5ZHp3aDNnZBItY29zbW9zMWZzN2x1MjhoeDVtOWFrbTdycDBjMjQyMmNuOHIyZjdndXJ1amhmGnIKaAoqL2Nvc21vcy5zdGFraW5nLnYxYmV0YTEuU3Rha2VBdXRob3JpemF0aW9uEjogAhI2CjRjb3Ntb3N2YWxvcGVyMWdqdHZseTlsZWw2enNrdnd0dmxnNXZod3B1OWM5d2F3N3N4end4EgYI4LD6pgYSZwpQCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohA/fcQw1hCVUx904t+kCXTiiziaLIY8lyssu1ENfzaN1KEgQKAggBGAUSEwoNCgV1YXRvbRIEMjQxOBCp8wUaQLmIllMlQEFVxjPbK2SNitPNm/7c9nmouNwucybOik6lJs+CR1+/hWhDX7ZKltFNk46c5xWvS/zWfAkCfX5qTr8="}"#,
        // Previous: 81727ee8a318a7fcec7cfad59ab16ac7cb2353d99cd81ee110eafed47207fb5923aa7cd22191a8779ca15ebef3811091cf464e535140e4a5029442b22bd3f572
        signature: "b988965325404155c633db2b648d8ad3cd9bfedcf679a8b8dc2e7326ce8a4ea526cf82475fbf8568435fb64a96d14d938e9ce715af4bfcd67c09027d7e6a4ebf",
        // Previous: gXJ+6KMYp/zsfPrVmrFqx8sjU9mc2B7hEOr+1HIH+1kjqnzSIZGod5yhXr7zgRCRz0ZOU1FA5KUClEKyK9P1cg==
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A/fcQw1hCVUx904t+kCXTiiziaLIY8lyssu1ENfzaN1K"},"signature":"uYiWUyVAQVXGM9srZI2K082b/tz2eai43C5zJs6KTqUmz4JHX7+FaENftkqW0U2TjpznFa9L/NZ8CQJ9fmpOvw=="}]"#,
    });

    // `AuthGrant` doesn't support JSON serialization and signing.
    test_sign_json_error::<StandardCosmosContext>(TestErrorInput {
        coin: &coin,
        input,
        error: SigningError::Error_not_supported,
    });
}

#[test]
fn test_staking_remove_compounding_authz() {
    let coin = TestCoinContext::default()
        .with_public_key_type(PublicKeyType::Secp256k1)
        .with_hrp("cosmos");

    let auth_revoke = Proto::mod_Message::AuthRevoke {
        granter: "cosmos13k0q0l7lg2kr32kvt7ly236ppldy8v9dzwh3gd".into(),
        grantee: "cosmos1fs7lu28hx5m9akm7rp0c2422cn8r2f7gurujhf".into(),
        msg_type_url: "/cosmos.staking.v1beta1.MsgDelegate".into(),
    };
    let input = Proto::SigningInput {
        account_number: 1290826,
        chain_id: "cosmoshub-4".into(),
        sequence: 4,
        fee: Some(make_fee(87735, make_amount("uatom", "2194"))),
        private_key: account_1290826_private_key(),
        messages: vec![make_message(MessageEnum::auth_revoke(auth_revoke))],
        ..Proto::SigningInput::default()
    };

    // Successfully broadcasted:  https://www.mintscan.io/cosmos/txs/E3218F634BB6A1BE256545EBE38275D5B02D41E88F504A43F97CD9CD2B624D44
    test_sign_protobuf::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input: input.clone(),
        tx: r#"{"mode":"BROADCAST_MODE_BLOCK","tx_bytes":"CqoBCqcBCh8vY29zbW9zLmF1dGh6LnYxYmV0YTEuTXNnUmV2b2tlEoMBCi1jb3Ntb3MxM2swcTBsN2xnMmtyMzJrdnQ3bHkyMzZwcGxkeTh2OWR6d2gzZ2QSLWNvc21vczFmczdsdTI4aHg1bTlha203cnAwYzI0MjJjbjhyMmY3Z3VydWpoZhojL2Nvc21vcy5zdGFraW5nLnYxYmV0YTEuTXNnRGVsZWdhdGUSZwpQCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohA/fcQw1hCVUx904t+kCXTiiziaLIY8lyssu1ENfzaN1KEgQKAggBGAQSEwoNCgV1YXRvbRIEMjE5NBC3rQUaQI7K+W7MMBoD6FbFZxRBqs9VTjErztjWTy57+fvrLaTCIZ+eBs7CuaKqfUZdSN8otjubSHVTQID3k9DpPAX0yDo="}"#,
        signature: "8ecaf96ecc301a03e856c5671441aacf554e312bced8d64f2e7bf9fbeb2da4c2219f9e06cec2b9a2aa7d465d48df28b63b9b4875534080f793d0e93c05f4c83a",
        // Previous: gXJ+6KMYp/zsfPrVmrFqx8sjU9mc2B7hEOr+1HIH+1kjqnzSIZGod5yhXr7zgRCRz0ZOU1FA5KUClEKyK9P1cg==
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A/fcQw1hCVUx904t+kCXTiiziaLIY8lyssu1ENfzaN1K"},"signature":"jsr5bswwGgPoVsVnFEGqz1VOMSvO2NZPLnv5++stpMIhn54GzsK5oqp9Rl1I3yi2O5tIdVNAgPeT0Ok8BfTIOg=="}]"#,
    });

    // `AuthRevoke` doesn't support JSON serialization and signing.
    test_sign_json_error::<StandardCosmosContext>(TestErrorInput {
        coin: &coin,
        input,
        error: SigningError::Error_not_supported,
    });
}

#[test]
fn test_staking_delegate() {
    let coin = TestCoinContext::default()
        .with_public_key_type(PublicKeyType::Secp256k1)
        .with_hrp("cosmos");

    let delegate = Proto::mod_Message::Delegate {
        delegator_address: "cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02".into(),
        validator_address: "cosmosvaloper1zkupr83hrzkn3up5elktzcq3tuft8nxsmwdqgp".into(),
        amount: Some(make_amount("muon", "10")),
        ..Proto::mod_Message::Delegate::default()
    };
    let input = Proto::SigningInput {
        account_number: 1037,
        chain_id: "gaia-13003".into(),
        sequence: 7,
        fee: Some(make_fee(101721, make_amount("muon", "1018"))),
        private_key: account_1337_private_key(),
        messages: vec![make_message(MessageEnum::stake_message(delegate))],
        ..Proto::SigningInput::default()
    };

    test_sign_protobuf::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input: input.clone(),
        tx: r#"{"mode":"BROADCAST_MODE_BLOCK","tx_bytes":"CpsBCpgBCiMvY29zbW9zLnN0YWtpbmcudjFiZXRhMS5Nc2dEZWxlZ2F0ZRJxCi1jb3Ntb3MxaHNrNmpyeXlxamZocDVkaGM1NXRjOWp0Y2t5Z3gwZXBoNmRkMDISNGNvc21vc3ZhbG9wZXIxemt1cHI4M2hyemtuM3VwNWVsa3R6Y3EzdHVmdDhueHNtd2RxZ3AaCgoEbXVvbhICMTASZgpQCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohAlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3FEgQKAggBGAcSEgoMCgRtdW9uEgQxMDE4ENmaBhpA8O9Jm/kL6Za2I3poDs5vpMowYJgNvYCJBRU/vxAjs0lNZYsq40qpTbwOTbORjJA5UjQ6auc40v6uCFT4q4z+uA=="}"#,
        signature: "f0ef499bf90be996b6237a680ece6fa4ca3060980dbd808905153fbf1023b3494d658b2ae34aa94dbc0e4db3918c903952343a6ae738d2feae0854f8ab8cfeb8",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"8O9Jm/kL6Za2I3poDs5vpMowYJgNvYCJBRU/vxAjs0lNZYsq40qpTbwOTbORjJA5UjQ6auc40v6uCFT4q4z+uA=="}]"#,
    });

    test_sign_json::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input,
        tx: r#"{"mode":"block","tx":{"fee":{"amount":[{"amount":"1018","denom":"muon"}],"gas":"101721"},"memo":"","msg":[{"type":"cosmos-sdk/MsgDelegate","value":{"amount":{"amount":"10","denom":"muon"},"delegator_address":"cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02","validator_address":"cosmosvaloper1zkupr83hrzkn3up5elktzcq3tuft8nxsmwdqgp"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"wIvfbCsLRCjzeXXoXTKfHLGXRbAAmUp0O134HVfVc6pfdVNJvvzISMHRUHgYcjsSiFlLyR32heia/yLgMDtIYQ=="}]}}"#,
        signature: "c08bdf6c2b0b4428f37975e85d329f1cb19745b000994a743b5df81d57d573aa5f755349befcc848c1d1507818723b1288594bc91df685e89aff22e0303b4861",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"wIvfbCsLRCjzeXXoXTKfHLGXRbAAmUp0O134HVfVc6pfdVNJvvzISMHRUHgYcjsSiFlLyR32heia/yLgMDtIYQ=="}]"#,
    });
}

#[test]
fn test_staking_undelegate() {
    let coin = TestCoinContext::default()
        .with_public_key_type(PublicKeyType::Secp256k1)
        .with_hrp("cosmos");

    let undelegate = Proto::mod_Message::Undelegate {
        delegator_address: "cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02".into(),
        validator_address: "cosmosvaloper1zkupr83hrzkn3up5elktzcq3tuft8nxsmwdqgp".into(),
        amount: Some(make_amount("muon", "10")),
        ..Proto::mod_Message::Undelegate::default()
    };
    let input = Proto::SigningInput {
        account_number: 1037,
        chain_id: "gaia-13003".into(),
        sequence: 7,
        fee: Some(make_fee(101721, make_amount("muon", "1018"))),
        private_key: account_1337_private_key(),
        messages: vec![make_message(MessageEnum::unstake_message(undelegate))],
        ..Proto::SigningInput::default()
    };

    test_sign_protobuf::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input: input.clone(),
        tx: r#"{"mode":"BROADCAST_MODE_BLOCK","tx_bytes":"Cp0BCpoBCiUvY29zbW9zLnN0YWtpbmcudjFiZXRhMS5Nc2dVbmRlbGVnYXRlEnEKLWNvc21vczFoc2s2anJ5eXFqZmhwNWRoYzU1dGM5anRja3lneDBlcGg2ZGQwMhI0Y29zbW9zdmFsb3BlcjF6a3VwcjgzaHJ6a24zdXA1ZWxrdHpjcTN0dWZ0OG54c213ZHFncBoKCgRtdW9uEgIxMBJmClAKRgofL2Nvc21vcy5jcnlwdG8uc2VjcDI1NmsxLlB1YktleRIjCiECVyhuw/N9M1V7u6oACyd0SskCOqmWfK51oYHR/5H6ncUSBAoCCAEYBxISCgwKBG11b24SBDEwMTgQ2ZoGGkBhlxHFnjBERxLtjLbMCKXcrDctaSZ9djtWCa3ely1bpV6m+6aAFjpr8aEZH+q2AtjJSEdgpQRJxP+9/gQsRTnZ"}"#,
        signature: "619711c59e30444712ed8cb6cc08a5dcac372d69267d763b5609adde972d5ba55ea6fba680163a6bf1a1191feab602d8c9484760a50449c4ffbdfe042c4539d9",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"YZcRxZ4wREcS7Yy2zAil3Kw3LWkmfXY7Vgmt3pctW6VepvumgBY6a/GhGR/qtgLYyUhHYKUEScT/vf4ELEU52Q=="}]"#,
    });

    test_sign_json::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input,
        tx: r#"{"mode":"block","tx":{"fee":{"amount":[{"amount":"1018","denom":"muon"}],"gas":"101721"},"memo":"","msg":[{"type":"cosmos-sdk/MsgUndelegate","value":{"amount":{"amount":"10","denom":"muon"},"delegator_address":"cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02","validator_address":"cosmosvaloper1zkupr83hrzkn3up5elktzcq3tuft8nxsmwdqgp"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"j4WpUVohGIHa6/s0bCvuyjq1wtQGqbOtQCz92qPQjisTN44Tz++Ozx1lAP6F0M4+eTA03XerqQ8hZCeAfL/3nw=="}]}}"#,
        signature: "8f85a9515a211881daebfb346c2beeca3ab5c2d406a9b3ad402cfddaa3d08e2b13378e13cfef8ecf1d6500fe85d0ce3e793034dd77aba90f216427807cbff79f",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"j4WpUVohGIHa6/s0bCvuyjq1wtQGqbOtQCz92qPQjisTN44Tz++Ozx1lAP6F0M4+eTA03XerqQ8hZCeAfL/3nw=="}]"#,
    });
}

#[test]
fn test_staking_restake() {
    let coin = TestCoinContext::default()
        .with_public_key_type(PublicKeyType::Secp256k1)
        .with_hrp("cosmos");

    let redelegate = Proto::mod_Message::BeginRedelegate {
        delegator_address: "cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02".into(),
        validator_src_address: "cosmosvaloper1zkupr83hrzkn3up5elktzcq3tuft8nxsmwdqgp".into(),
        validator_dst_address: "cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02".into(),
        amount: Some(make_amount("muon", "10")),
        ..Proto::mod_Message::BeginRedelegate::default()
    };
    let input = Proto::SigningInput {
        account_number: 1037,
        chain_id: "gaia-13003".into(),
        sequence: 7,
        fee: Some(make_fee(101721, make_amount("muon", "1018"))),
        private_key: account_1337_private_key(),
        messages: vec![make_message(MessageEnum::restake_message(redelegate))],
        ..Proto::SigningInput::default()
    };

    test_sign_protobuf::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input: input.clone(),
        tx: r#"{"mode":"BROADCAST_MODE_BLOCK","tx_bytes":"CtIBCs8BCiovY29zbW9zLnN0YWtpbmcudjFiZXRhMS5Nc2dCZWdpblJlZGVsZWdhdGUSoAEKLWNvc21vczFoc2s2anJ5eXFqZmhwNWRoYzU1dGM5anRja3lneDBlcGg2ZGQwMhI0Y29zbW9zdmFsb3BlcjF6a3VwcjgzaHJ6a24zdXA1ZWxrdHpjcTN0dWZ0OG54c213ZHFncBotY29zbW9zMWhzazZqcnl5cWpmaHA1ZGhjNTV0YzlqdGNreWd4MGVwaDZkZDAyIgoKBG11b24SAjEwEmYKUApGCh8vY29zbW9zLmNyeXB0by5zZWNwMjU2azEuUHViS2V5EiMKIQJXKG7D830zVXu7qgALJ3RKyQI6qZZ8rnWhgdH/kfqdxRIECgIIARgHEhIKDAoEbXVvbhIEMTAxOBDZmgYaQJ52qO5xdtBkNUeFeWrnqUXkngyHFKCXnOPPClyVI0HrULdp5jbwGra2RujEOn4BrbFCb3JFnpc2o1iuLXbKQxg="}"#,
        signature: "9e76a8ee7176d064354785796ae7a945e49e0c8714a0979ce3cf0a5c952341eb50b769e636f01ab6b646e8c43a7e01adb1426f72459e9736a358ae2d76ca4318",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"nnao7nF20GQ1R4V5auepReSeDIcUoJec488KXJUjQetQt2nmNvAatrZG6MQ6fgGtsUJvckWelzajWK4tdspDGA=="}]"#,
    });

    test_sign_json::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input,
        tx: r#"{"mode":"block","tx":{"fee":{"amount":[{"amount":"1018","denom":"muon"}],"gas":"101721"},"memo":"","msg":[{"type":"cosmos-sdk/MsgBeginRedelegate","value":{"amount":{"amount":"10","denom":"muon"},"delegator_address":"cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02","validator_dst_address":"cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02","validator_src_address":"cosmosvaloper1zkupr83hrzkn3up5elktzcq3tuft8nxsmwdqgp"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"5k03Yb0loovvzagMCg4gjQJP2woriZVRcOZaXF1FSros6B1X4B8MEm3lpZwrWBJMEJVgyYA9ZaF6FLVI3WxQ2w=="}]}}"#,
        signature: "e64d3761bd25a28befcda80c0a0e208d024fdb0a2b89955170e65a5c5d454aba2ce81d57e01f0c126de5a59c2b58124c109560c9803d65a17a14b548dd6c50db",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"5k03Yb0loovvzagMCg4gjQJP2woriZVRcOZaXF1FSros6B1X4B8MEm3lpZwrWBJMEJVgyYA9ZaF6FLVI3WxQ2w=="}]"#,
    });
}

#[test]
fn test_staking_withdraw_rewards() {
    let coin = TestCoinContext::default()
        .with_public_key_type(PublicKeyType::Secp256k1)
        .with_hrp("cosmos");

    let withdraw = Proto::mod_Message::WithdrawDelegationReward {
        delegator_address: "cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02".into(),
        validator_address: "cosmosvaloper1zkupr83hrzkn3up5elktzcq3tuft8nxsmwdqgp".into(),
        ..Proto::mod_Message::WithdrawDelegationReward::default()
    };
    let input = Proto::SigningInput {
        account_number: 1037,
        chain_id: "gaia-13003".into(),
        sequence: 7,
        fee: Some(make_fee(101721, make_amount("muon", "1018"))),
        private_key: account_1337_private_key(),
        messages: vec![make_message(MessageEnum::withdraw_stake_reward_message(
            withdraw,
        ))],
        ..Proto::SigningInput::default()
    };

    test_sign_protobuf::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input: input.clone(),
        tx: r#"{"mode":"BROADCAST_MODE_BLOCK","tx_bytes":"CqMBCqABCjcvY29zbW9zLmRpc3RyaWJ1dGlvbi52MWJldGExLk1zZ1dpdGhkcmF3RGVsZWdhdG9yUmV3YXJkEmUKLWNvc21vczFoc2s2anJ5eXFqZmhwNWRoYzU1dGM5anRja3lneDBlcGg2ZGQwMhI0Y29zbW9zdmFsb3BlcjF6a3VwcjgzaHJ6a24zdXA1ZWxrdHpjcTN0dWZ0OG54c213ZHFncBJmClAKRgofL2Nvc21vcy5jcnlwdG8uc2VjcDI1NmsxLlB1YktleRIjCiECVyhuw/N9M1V7u6oACyd0SskCOqmWfK51oYHR/5H6ncUSBAoCCAEYBxISCgwKBG11b24SBDEwMTgQ2ZoGGkBW1Cd+0pNfMPEVXQtqG1VIijDjZP2UOiDlvUF478axnxlF8PaOAsY0S5OdUE3Wz7+nu8YVmrLZQS/8mlqLaK05"}"#,
        signature: "56d4277ed2935f30f1155d0b6a1b55488a30e364fd943a20e5bd4178efc6b19f1945f0f68e02c6344b939d504dd6cfbfa7bbc6159ab2d9412ffc9a5a8b68ad39",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"VtQnftKTXzDxFV0LahtVSIow42T9lDog5b1BeO/GsZ8ZRfD2jgLGNEuTnVBN1s+/p7vGFZqy2UEv/Jpai2itOQ=="}]"#,
    });

    test_sign_json::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input,
        tx: r#"{"mode":"block","tx":{"fee":{"amount":[{"amount":"1018","denom":"muon"}],"gas":"101721"},"memo":"","msg":[{"type":"cosmos-sdk/MsgWithdrawDelegationReward","value":{"delegator_address":"cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02","validator_address":"cosmosvaloper1zkupr83hrzkn3up5elktzcq3tuft8nxsmwdqgp"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"VG8NZzVvavlM+1qyK5dOSZwzEj8sLCkvTw5kh44Oco9GQxBf13FVC+s/I3HwiICqo4+o8jNMEDp3nx2C0tuY1g=="}]}}"#,
        signature: "546f0d67356f6af94cfb5ab22b974e499c33123f2c2c292f4f0e64878e0e728f4643105fd771550beb3f2371f08880aaa38fa8f2334c103a779f1d82d2db98d6",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"VG8NZzVvavlM+1qyK5dOSZwzEj8sLCkvTw5kh44Oco9GQxBf13FVC+s/I3HwiICqo4+o8jNMEDp3nx2C0tuY1g=="}]"#,
    });
}

#[test]
fn test_staking_set_withdraw_address() {
    let coin = TestCoinContext::default()
        .with_public_key_type(PublicKeyType::Secp256k1)
        .with_hrp("cosmos");

    let set_address = Proto::mod_Message::SetWithdrawAddress {
        delegator_address: "cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02".into(),
        withdraw_address: "cosmosvaloper1zkupr83hrzkn3up5elktzcq3tuft8nxsmwdqgp".into(),
        ..Proto::mod_Message::SetWithdrawAddress::default()
    };
    let input = Proto::SigningInput {
        account_number: 1037,
        chain_id: "gaia-13003".into(),
        sequence: 7,
        fee: Some(make_fee(101721, make_amount("muon", "1018"))),
        private_key: account_1337_private_key(),
        messages: vec![make_message(MessageEnum::set_withdraw_address_message(
            set_address,
        ))],
        ..Proto::SigningInput::default()
    };

    test_sign_protobuf::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input: input.clone(),
        tx: r#"{"mode":"BROADCAST_MODE_BLOCK","tx_bytes":"Cp4BCpsBCjIvY29zbW9zLmRpc3RyaWJ1dGlvbi52MWJldGExLk1zZ1NldFdpdGhkcmF3QWRkcmVzcxJlCi1jb3Ntb3MxaHNrNmpyeXlxamZocDVkaGM1NXRjOWp0Y2t5Z3gwZXBoNmRkMDISNGNvc21vc3ZhbG9wZXIxemt1cHI4M2hyemtuM3VwNWVsa3R6Y3EzdHVmdDhueHNtd2RxZ3ASZgpQCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohAlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3FEgQKAggBGAcSEgoMCgRtdW9uEgQxMDE4ENmaBhpAkm2TJLw4FcIwN5bkqVaGbmAgkTSHeYD8sUkIyJHLa89cPvThkFO/lKlxBMl2UAMs06hL6cYcl4Px+B6rpFdBpA=="}"#,
        signature: "926d9324bc3815c2303796e4a956866e60209134877980fcb14908c891cb6bcf5c3ef4e19053bf94a97104c97650032cd3a84be9c61c9783f1f81eaba45741a4",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"km2TJLw4FcIwN5bkqVaGbmAgkTSHeYD8sUkIyJHLa89cPvThkFO/lKlxBMl2UAMs06hL6cYcl4Px+B6rpFdBpA=="}]"#,
    });

    test_sign_json::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input,
        tx: r#"{"mode":"block","tx":{"fee":{"amount":[{"amount":"1018","denom":"muon"}],"gas":"101721"},"memo":"","msg":[{"type":"cosmos-sdk/MsgSetWithdrawAddress","value":{"delegator_address":"cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02","withdraw_address":"cosmosvaloper1zkupr83hrzkn3up5elktzcq3tuft8nxsmwdqgp"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"Is+87DPQbtQmIyZASdEdb7hlZhA9ViGiOxREAi6xqs46B5ChxGtIwCGGiWFtr5f5mucsNYmWYgXeRbVxlPutog=="}]}}"#,
        signature: "22cfbcec33d06ed42623264049d11d6fb86566103d5621a23b1444022eb1aace3a0790a1c46b48c0218689616daf97f99ae72c3589966205de45b57194fbada2",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"Is+87DPQbtQmIyZASdEdb7hlZhA9ViGiOxREAi6xqs46B5ChxGtIwCGGiWFtr5f5mucsNYmWYgXeRbVxlPutog=="}]"#,
    });
}
