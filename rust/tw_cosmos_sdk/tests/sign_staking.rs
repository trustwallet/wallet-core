// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::borrow::Cow;
use tw_coin_entry::test_utils::test_context::TestCoinContext;
use tw_cosmos_sdk::context::StandardCosmosContext;
use tw_cosmos_sdk::test_utils::proto_utils::{make_amount, make_fee, make_message};
use tw_cosmos_sdk::test_utils::sign_utils::{
    test_sign_json, test_sign_json_error, test_sign_protobuf, TestErrorInput, TestInput,
};
use tw_encoding::hex::DecodeHex;
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

fn account_1290826_private_key() -> Cow<'static, [u8]> {
    "c7764249cdf77f8f1d840fa8af431579e5e41cf1af937e1e23afa22f3f4f0ccc"
        .decode_hex()
        .unwrap()
        .into()
}

fn account_1932898_private_key() -> Cow<'static, [u8]> {
    "d142e036ceebe70c4e61e3909d6c16bab518edfeac8bdf91000463ce0b4a6156"
        .decode_hex()
        .unwrap()
        .into()
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
        authorization_type: Proto::mod_Message::AuthorizationType::DELEGATE,
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

    // Original test: https://github.com/trustwallet/wallet-core/blob/a60033f797e33628e557af7c66be539c8d78bc61/tests/chains/Cosmos/StakingTests.cpp#L18-L52
    // Please note the signature has been updated according to the serialization of the `StakeAuthorization` message.
    test_sign_protobuf::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input: input.clone(),
        // Previous: CvgBCvUBCh4vY29zbW9zLmF1dGh6LnYxYmV0YTEuTXNnR3JhbnQS0gEKLWNvc21vczEzazBxMGw3bGcya3IzMmt2dDdseTIzNnBwbGR5OHY5ZHp3aDNnZBItY29zbW9zMWZzN2x1MjhoeDVtOWFrbTdycDBjMjQyMmNuOHIyZjdndXJ1amhmGnIKaAoqL2Nvc21vcy5zdGFraW5nLnYxYmV0YTEuU3Rha2VBdXRob3JpemF0aW9uEjoSNgo0Y29zbW9zdmFsb3BlcjFnanR2bHk5bGVsNnpza3Z3dHZsZzV2aHdwdTljOXdhdzdzeHp3eCABEgYI4LD6pgYSZwpQCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohA/fcQw1hCVUx904t+kCXTiiziaLIY8lyssu1ENfzaN1KEgQKAggBGAUSEwoNCgV1YXRvbRIEMjQxOBCp8wUaQIFyfuijGKf87Hz61ZqxasfLI1PZnNge4RDq/tRyB/tZI6p80iGRqHecoV6+84EQkc9GTlNRQOSlApRCsivT9XI=
        tx: r#"{"mode":"BROADCAST_MODE_BLOCK","tx_bytes":"CvgBCvUBCh4vY29zbW9zLmF1dGh6LnYxYmV0YTEuTXNnR3JhbnQS0gEKLWNvc21vczEzazBxMGw3bGcya3IzMmt2dDdseTIzNnBwbGR5OHY5ZHp3aDNnZBItY29zbW9zMWZzN2x1MjhoeDVtOWFrbTdycDBjMjQyMmNuOHIyZjdndXJ1amhmGnIKaAoqL2Nvc21vcy5zdGFraW5nLnYxYmV0YTEuU3Rha2VBdXRob3JpemF0aW9uEjogARI2CjRjb3Ntb3N2YWxvcGVyMWdqdHZseTlsZWw2enNrdnd0dmxnNXZod3B1OWM5d2F3N3N4end4EgYI4LD6pgYSZwpQCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohA/fcQw1hCVUx904t+kCXTiiziaLIY8lyssu1ENfzaN1KEgQKAggBGAUSEwoNCgV1YXRvbRIEMjQxOBCp8wUaQEAN1nIfDawlHnep2bNEm14w+g7tYybJJT3htcGVS6s9D7va3ed1OUEIk9LZoc3G//VenJ+KLw26SRVBaRukgVI="}"#,
        // Previous: 81727ee8a318a7fcec7cfad59ab16ac7cb2353d99cd81ee110eafed47207fb5923aa7cd22191a8779ca15ebef3811091cf464e535140e4a5029442b22bd3f572
        signature: "400dd6721f0dac251e77a9d9b3449b5e30fa0eed6326c9253de1b5c1954bab3d0fbbdadde77539410893d2d9a1cdc6fff55e9c9f8a2f0dba491541691ba48152",
        // Previous: gXJ+6KMYp/zsfPrVmrFqx8sjU9mc2B7hEOr+1HIH+1kjqnzSIZGod5yhXr7zgRCRz0ZOU1FA5KUClEKyK9P1cg==
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A/fcQw1hCVUx904t+kCXTiiziaLIY8lyssu1ENfzaN1K"},"signature":"QA3Wch8NrCUed6nZs0SbXjD6Du1jJsklPeG1wZVLqz0Pu9rd53U5QQiT0tmhzcb/9V6cn4ovDbpJFUFpG6SBUg=="}]"#,
    });

    // `AuthGrant` doesn't support JSON serialization and signing.
    test_sign_json_error::<StandardCosmosContext>(TestErrorInput {
        coin: &coin,
        input,
        error: SigningError::Error_not_supported,
    });
}

#[test]
fn test_staking_compounding_authz_f355e659() {
    use Proto::mod_Message::mod_AuthGrant::OneOfgrant_type as ProtoGrantType;
    use Proto::mod_Message::mod_StakeAuthorization::OneOfvalidators as ProtoValidatorsType;

    let coin = TestCoinContext::default()
        .with_public_key_type(PublicKeyType::Secp256k1)
        .with_hrp("cosmos");

    let allow_list = Proto::mod_Message::mod_StakeAuthorization::Validators {
        address: vec!["cosmosvaloper1gjtvly9lel6zskvwtvlg5vhwpu9c9waw7sxzwx".into()],
    };
    let stake_authorization = Proto::mod_Message::StakeAuthorization {
        authorization_type: Proto::mod_Message::AuthorizationType::DELEGATE,
        validators: ProtoValidatorsType::allow_list(allow_list),
        ..Proto::mod_Message::StakeAuthorization::default()
    };
    let auth_grant = Proto::mod_Message::AuthGrant {
        granter: "cosmos1wd0hdkzq68nmwzpprcugx82msj3l2y3wh8g5vv".into(),
        grantee: "cosmos1fs7lu28hx5m9akm7rp0c2422cn8r2f7gurujhf".into(),
        grant_type: ProtoGrantType::grant_stake(stake_authorization),
        expiration: 1733011200,
    };
    let input = Proto::SigningInput {
        account_number: 1932898,
        chain_id: "cosmoshub-4".into(),
        sequence: 0,
        fee: Some(make_fee(96681, make_amount("uatom", "2418"))),
        private_key: account_1932898_private_key(),
        messages: vec![make_message(MessageEnum::auth_grant(auth_grant))],
        ..Proto::SigningInput::default()
    };

    // Successfully broadcasted https://www.mintscan.io/cosmos/tx/F355E659CBB8C0191213415E8F3EC6FD0AD1541F96FF192855147F6C0872A98B?height=17879293
    test_sign_protobuf::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input: input,
        tx: r#"{"mode":"BROADCAST_MODE_BLOCK","tx_bytes":"CvgBCvUBCh4vY29zbW9zLmF1dGh6LnYxYmV0YTEuTXNnR3JhbnQS0gEKLWNvc21vczF3ZDBoZGt6cTY4bm13enBwcmN1Z3g4Mm1zajNsMnkzd2g4ZzV2dhItY29zbW9zMWZzN2x1MjhoeDVtOWFrbTdycDBjMjQyMmNuOHIyZjdndXJ1amhmGnIKaAoqL2Nvc21vcy5zdGFraW5nLnYxYmV0YTEuU3Rha2VBdXRob3JpemF0aW9uEjogARI2CjRjb3Ntb3N2YWxvcGVyMWdqdHZseTlsZWw2enNrdnd0dmxnNXZod3B1OWM5d2F3N3N4end4EgYIgM6uugYSZQpOCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohA1yVadHrd1uQDhwasOzMBbg6zarM8PjyhRmDVY97HiX5EgQKAggBEhMKDQoFdWF0b20SBDI0MTgQqfMFGkCcDzlVwE+RUkWhjH0PiBrKDzqGgIczmj9fuMI0umrOFTqmKm/IQGol0eo4XZOIcahSYlqJ+1MOptAZM8Csqoay"}"#,
        signature: "9c0f3955c04f915245a18c7d0f881aca0f3a868087339a3f5fb8c234ba6ace153aa62a6fc8406a25d1ea385d938871a852625a89fb530ea6d01933c0acaa86b2",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A1yVadHrd1uQDhwasOzMBbg6zarM8PjyhRmDVY97HiX5"},"signature":"nA85VcBPkVJFoYx9D4gayg86hoCHM5o/X7jCNLpqzhU6pipvyEBqJdHqOF2TiHGoUmJaiftTDqbQGTPArKqGsg=="}]"#,
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
        private_key: account_1037_private_key(),
        messages: vec![make_message(MessageEnum::stake_message(delegate))],
        mode: Proto::BroadcastMode::ASYNC,
        ..Proto::SigningInput::default()
    };

    test_sign_protobuf::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input: input.clone(),
        tx: r#"{"mode":"BROADCAST_MODE_ASYNC","tx_bytes":"CpsBCpgBCiMvY29zbW9zLnN0YWtpbmcudjFiZXRhMS5Nc2dEZWxlZ2F0ZRJxCi1jb3Ntb3MxaHNrNmpyeXlxamZocDVkaGM1NXRjOWp0Y2t5Z3gwZXBoNmRkMDISNGNvc21vc3ZhbG9wZXIxemt1cHI4M2hyemtuM3VwNWVsa3R6Y3EzdHVmdDhueHNtd2RxZ3AaCgoEbXVvbhICMTASZgpQCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohAlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3FEgQKAggBGAcSEgoMCgRtdW9uEgQxMDE4ENmaBhpA8O9Jm/kL6Za2I3poDs5vpMowYJgNvYCJBRU/vxAjs0lNZYsq40qpTbwOTbORjJA5UjQ6auc40v6uCFT4q4z+uA=="}"#,
        signature: "f0ef499bf90be996b6237a680ece6fa4ca3060980dbd808905153fbf1023b3494d658b2ae34aa94dbc0e4db3918c903952343a6ae738d2feae0854f8ab8cfeb8",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"8O9Jm/kL6Za2I3poDs5vpMowYJgNvYCJBRU/vxAjs0lNZYsq40qpTbwOTbORjJA5UjQ6auc40v6uCFT4q4z+uA=="}]"#,
    });

    test_sign_json::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input,
        tx: r#"{"mode":"async","tx":{"fee":{"amount":[{"amount":"1018","denom":"muon"}],"gas":"101721"},"memo":"","msg":[{"type":"cosmos-sdk/MsgDelegate","value":{"amount":{"amount":"10","denom":"muon"},"delegator_address":"cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02","validator_address":"cosmosvaloper1zkupr83hrzkn3up5elktzcq3tuft8nxsmwdqgp"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"wIvfbCsLRCjzeXXoXTKfHLGXRbAAmUp0O134HVfVc6pfdVNJvvzISMHRUHgYcjsSiFlLyR32heia/yLgMDtIYQ=="}]}}"#,
        signature: "c08bdf6c2b0b4428f37975e85d329f1cb19745b000994a743b5df81d57d573aa5f755349befcc848c1d1507818723b1288594bc91df685e89aff22e0303b4861",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"wIvfbCsLRCjzeXXoXTKfHLGXRbAAmUp0O134HVfVc6pfdVNJvvzISMHRUHgYcjsSiFlLyR32heia/yLgMDtIYQ=="}]"#,
    });
}

#[test]
fn test_staking_delegate_custom_msg_type() {
    let coin = TestCoinContext::default()
        .with_public_key_type(PublicKeyType::Secp256k1)
        .with_hrp("cosmos");

    let delegate = Proto::mod_Message::Delegate {
        delegator_address: "cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02".into(),
        validator_address: "cosmosvaloper1zkupr83hrzkn3up5elktzcq3tuft8nxsmwdqgp".into(),
        amount: Some(make_amount("muon", "10")),
        type_prefix: "unreal/MsgDelegate".into(),
    };
    let input = Proto::SigningInput {
        account_number: 1037,
        chain_id: "gaia-13003".into(),
        sequence: 7,
        fee: Some(make_fee(101721, make_amount("muon", "1018"))),
        private_key: account_1037_private_key(),
        messages: vec![make_message(MessageEnum::stake_message(delegate))],
        mode: Proto::BroadcastMode::ASYNC,
        ..Proto::SigningInput::default()
    };

    // This transaction hasn't been broadcasted.
    // Check if the custom type_prefix doesn't affect Protobuf serialization.
    test_sign_protobuf::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input: input.clone(),
        tx: r#"{"mode":"BROADCAST_MODE_ASYNC","tx_bytes":"CpsBCpgBCiMvY29zbW9zLnN0YWtpbmcudjFiZXRhMS5Nc2dEZWxlZ2F0ZRJxCi1jb3Ntb3MxaHNrNmpyeXlxamZocDVkaGM1NXRjOWp0Y2t5Z3gwZXBoNmRkMDISNGNvc21vc3ZhbG9wZXIxemt1cHI4M2hyemtuM3VwNWVsa3R6Y3EzdHVmdDhueHNtd2RxZ3AaCgoEbXVvbhICMTASZgpQCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohAlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3FEgQKAggBGAcSEgoMCgRtdW9uEgQxMDE4ENmaBhpA8O9Jm/kL6Za2I3poDs5vpMowYJgNvYCJBRU/vxAjs0lNZYsq40qpTbwOTbORjJA5UjQ6auc40v6uCFT4q4z+uA=="}"#,
        signature: "f0ef499bf90be996b6237a680ece6fa4ca3060980dbd808905153fbf1023b3494d658b2ae34aa94dbc0e4db3918c903952343a6ae738d2feae0854f8ab8cfeb8",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"8O9Jm/kL6Za2I3poDs5vpMowYJgNvYCJBRU/vxAjs0lNZYsq40qpTbwOTbORjJA5UjQ6auc40v6uCFT4q4z+uA=="}]"#,
    });

    // This transaction hasn't been broadcasted.
    test_sign_json::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input: input.clone(),
        tx: r#"{"mode":"async","tx":{"fee":{"amount":[{"amount":"1018","denom":"muon"}],"gas":"101721"},"memo":"","msg":[{"type":"unreal/MsgDelegate","value":{"amount":{"amount":"10","denom":"muon"},"delegator_address":"cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02","validator_address":"cosmosvaloper1zkupr83hrzkn3up5elktzcq3tuft8nxsmwdqgp"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"D9OufIm3RGkoGU/OO7eNHe17Yg4q0OBU0pHnqtULWXIm3J1eSUxsVI6OCbGAMYEqHUB9i5b1YGrueaDYFOH9xQ=="}]}}"#,
        signature: "0fd3ae7c89b7446928194fce3bb78d1ded7b620e2ad0e054d291e7aad50b597226dc9d5e494c6c548e8e09b18031812a1d407d8b96f5606aee79a0d814e1fdc5",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"D9OufIm3RGkoGU/OO7eNHe17Yg4q0OBU0pHnqtULWXIm3J1eSUxsVI6OCbGAMYEqHUB9i5b1YGrueaDYFOH9xQ=="}]"#,
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
        private_key: account_1037_private_key(),
        messages: vec![make_message(MessageEnum::unstake_message(undelegate))],
        mode: Proto::BroadcastMode::SYNC,
        ..Proto::SigningInput::default()
    };

    test_sign_protobuf::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input: input.clone(),
        tx: r#"{"mode":"BROADCAST_MODE_SYNC","tx_bytes":"Cp0BCpoBCiUvY29zbW9zLnN0YWtpbmcudjFiZXRhMS5Nc2dVbmRlbGVnYXRlEnEKLWNvc21vczFoc2s2anJ5eXFqZmhwNWRoYzU1dGM5anRja3lneDBlcGg2ZGQwMhI0Y29zbW9zdmFsb3BlcjF6a3VwcjgzaHJ6a24zdXA1ZWxrdHpjcTN0dWZ0OG54c213ZHFncBoKCgRtdW9uEgIxMBJmClAKRgofL2Nvc21vcy5jcnlwdG8uc2VjcDI1NmsxLlB1YktleRIjCiECVyhuw/N9M1V7u6oACyd0SskCOqmWfK51oYHR/5H6ncUSBAoCCAEYBxISCgwKBG11b24SBDEwMTgQ2ZoGGkBhlxHFnjBERxLtjLbMCKXcrDctaSZ9djtWCa3ely1bpV6m+6aAFjpr8aEZH+q2AtjJSEdgpQRJxP+9/gQsRTnZ"}"#,
        signature: "619711c59e30444712ed8cb6cc08a5dcac372d69267d763b5609adde972d5ba55ea6fba680163a6bf1a1191feab602d8c9484760a50449c4ffbdfe042c4539d9",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"YZcRxZ4wREcS7Yy2zAil3Kw3LWkmfXY7Vgmt3pctW6VepvumgBY6a/GhGR/qtgLYyUhHYKUEScT/vf4ELEU52Q=="}]"#,
    });

    test_sign_json::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input,
        tx: r#"{"mode":"sync","tx":{"fee":{"amount":[{"amount":"1018","denom":"muon"}],"gas":"101721"},"memo":"","msg":[{"type":"cosmos-sdk/MsgUndelegate","value":{"amount":{"amount":"10","denom":"muon"},"delegator_address":"cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02","validator_address":"cosmosvaloper1zkupr83hrzkn3up5elktzcq3tuft8nxsmwdqgp"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"j4WpUVohGIHa6/s0bCvuyjq1wtQGqbOtQCz92qPQjisTN44Tz++Ozx1lAP6F0M4+eTA03XerqQ8hZCeAfL/3nw=="}]}}"#,
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
        private_key: account_1037_private_key(),
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
        private_key: account_1037_private_key(),
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
        private_key: account_1037_private_key(),
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
