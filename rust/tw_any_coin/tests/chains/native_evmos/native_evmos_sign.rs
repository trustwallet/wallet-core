// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::borrow::Cow;
use tw_any_coin::ffi::tw_any_signer::tw_any_signer_sign;
use tw_coin_entry::error::SigningErrorType;
use tw_cosmos_sdk::test_utils::proto_utils::{make_amount, make_fee, make_message};
use tw_encoding::hex::DecodeHex;
use tw_memory::test_utils::tw_data_helper::TWDataHelper;
use tw_proto::Cosmos::Proto;
use tw_proto::Cosmos::Proto::mod_Message::OneOfmessage_oneof as MessageEnum;
use tw_proto::{deserialize, serialize};

const NATIVE_EVMOS_COIN_TYPE: u32 = 20009001;

fn account_1037_private_key() -> Cow<'static, [u8]> {
    "80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005"
        .decode_hex()
        .unwrap()
        .into()
}

fn account_2139877_private_key() -> Cow<'static, [u8]> {
    "79bcbded1a5678ab34e6d9db9ad78e4e480e7b22723cc5fbf59e843732e1a8e5"
        .decode_hex()
        .unwrap()
        .into()
}

#[test]
fn test_sign_native_evmos_tx_json() {
    let send_msg = Proto::mod_Message::Send {
        from_address: "evmos1hsk6jryyqjfhp5dhc55tc9jtckygx0ep4mur4z".into(),
        to_address: "evmos1zt50azupanqlfam5afhv3hexwyutnuke45f6ye".into(),
        amounts: vec![make_amount("muon", "1")],
        ..Proto::mod_Message::Send::default()
    };
    let input = Proto::SigningInput {
        signing_mode: Proto::SigningMode::JSON,
        account_number: 1037,
        chain_id: "evmos_9001-2".into(),
        sequence: 8,
        fee: Some(make_fee(200000, make_amount("muon", "200"))),
        private_key: account_1037_private_key(),
        messages: vec![make_message(MessageEnum::send_coins_message(send_msg))],
        ..Proto::SigningInput::default()
    };

    let input_data = TWDataHelper::create(serialize(&input).unwrap());

    let output =
        TWDataHelper::wrap(unsafe { tw_any_signer_sign(input_data.ptr(), NATIVE_EVMOS_COIN_TYPE) })
            .to_vec()
            .expect("!tw_any_signer_sign returned nullptr");

    let output: Proto::SigningOutput = deserialize(&output).unwrap();
    assert_eq!(output.error, SigningErrorType::OK);
    assert!(output.error_message.is_empty());

    assert_eq!(
        output.json,
        r#"{"mode":"block","tx":{"fee":{"amount":[{"amount":"200","denom":"muon"}],"gas":"200000"},"memo":"","msg":[{"type":"cosmos-sdk/MsgSend","value":{"amount":[{"amount":"1","denom":"muon"}],"from_address":"evmos1hsk6jryyqjfhp5dhc55tc9jtckygx0ep4mur4z","to_address":"evmos1zt50azupanqlfam5afhv3hexwyutnuke45f6ye"}}],"signatures":[{"pub_key":{"type":"ethermint/PubKeyEthSecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"1hMFtRqKjB8tiuyHYVYZundPdomebIIvHLC1gj9uXtFc+iO3UAHBysBjFB4brd9AD5yriS3uUDTAqqfg6fNGNg=="}]}}"#
    );
    assert_eq!(
        output.signature_json,
        r#"[{"pub_key":{"type":"ethermint/PubKeyEthSecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"1hMFtRqKjB8tiuyHYVYZundPdomebIIvHLC1gj9uXtFc+iO3UAHBysBjFB4brd9AD5yriS3uUDTAqqfg6fNGNg=="}]"#
    );
}

/// CompoundingAuthz
#[test]
fn test_sign_native_evmos_tx_protobuf() {
    use Proto::mod_Message::mod_AuthGrant::OneOfgrant_type as ProtoGrantType;
    use Proto::mod_Message::mod_StakeAuthorization::OneOfvalidators as ProtoValidatorsType;

    let allow_list = Proto::mod_Message::mod_StakeAuthorization::Validators {
        address: vec!["evmosvaloper1umk407eed7af6anvut6llg2zevnf0dn0feqqny".into()],
    };
    let stake_authorization = Proto::mod_Message::StakeAuthorization {
        authorization_type: Proto::mod_Message::AuthorizationType::DELEGATE,
        validators: ProtoValidatorsType::allow_list(allow_list),
        ..Proto::mod_Message::StakeAuthorization::default()
    };
    let auth_grant = Proto::mod_Message::AuthGrant {
        granter: "evmos12m9grgas60yk0kult076vxnsrqz8xpjy9rpf3e".into(),
        grantee: "evmos18fzq4nac28gfma6gqfvkpwrgpm5ctar2z9mxf3".into(),
        grant_type: ProtoGrantType::grant_stake(stake_authorization),
        expiration: 1692309600,
    };
    let input = Proto::SigningInput {
        signing_mode: Proto::SigningMode::Protobuf,
        account_number: 2139877,
        chain_id: "evmos_9001-2".into(),
        sequence: 3,
        fee: Some(make_fee(180859, make_amount("aevmos", "4521475000000000"))),
        private_key: account_2139877_private_key(),
        messages: vec![make_message(MessageEnum::auth_grant(auth_grant))],
        ..Proto::SigningInput::default()
    };

    let input_data = TWDataHelper::create(serialize(&input).unwrap());

    let output =
        TWDataHelper::wrap(unsafe { tw_any_signer_sign(input_data.ptr(), NATIVE_EVMOS_COIN_TYPE) })
            .to_vec()
            .expect("!tw_any_signer_sign returned nullptr");

    let output: Proto::SigningOutput = deserialize(&output).unwrap();
    assert_eq!(output.error, SigningErrorType::OK);
    assert!(output.error_message.is_empty());

    // Original test: https://github.com/trustwallet/wallet-core/blob/a60033f797e33628e557af7c66be539c8d78bc61/tests/chains/Evmos/SignerTests.cpp#L91-L124
    // Please note the signature has been updated according to the serialization of the `StakeAuthorization` message.
    // Previous: CvUBCvIBCh4vY29zbW9zLmF1dGh6LnYxYmV0YTEuTXNnR3JhbnQSzwEKLGV2bW9zMTJtOWdyZ2FzNjB5azBrdWx0MDc2dnhuc3Jxejh4cGp5OXJwZjNlEixldm1vczE4ZnpxNG5hYzI4Z2ZtYTZncWZ2a3B3cmdwbTVjdGFyMno5bXhmMxpxCmcKKi9jb3Ntb3Muc3Rha2luZy52MWJldGExLlN0YWtlQXV0aG9yaXphdGlvbhI5EjUKM2V2bW9zdmFsb3BlcjF1bWs0MDdlZWQ3YWY2YW52dXQ2bGxnMnpldm5mMGRuMGZlcXFueSABEgYI4LD6pgYSfQpZCk8KKC9ldGhlcm1pbnQuY3J5cHRvLnYxLmV0aHNlY3AyNTZrMS5QdWJLZXkSIwohA4B2WHbj6sH/GWE7z/YW5PRnXYFGaGRAov7gZZI2Fv2nEgQKAggBGAMSIAoaCgZhZXZtb3MSEDQ1MjE0NzUwMDAwMDAwMDAQ+4QLGkAm17CZgB7m+CPVlITnrHosklMTL9zrUeGRs8FL8N0GcRami9zdJ+e3xuXOtJmwP7G6QNh85CRYjFj8a8lpmmJM
    assert_eq!(
        output.serialized,
        r#"{"mode":"BROADCAST_MODE_BLOCK","tx_bytes":"CvUBCvIBCh4vY29zbW9zLmF1dGh6LnYxYmV0YTEuTXNnR3JhbnQSzwEKLGV2bW9zMTJtOWdyZ2FzNjB5azBrdWx0MDc2dnhuc3Jxejh4cGp5OXJwZjNlEixldm1vczE4ZnpxNG5hYzI4Z2ZtYTZncWZ2a3B3cmdwbTVjdGFyMno5bXhmMxpxCmcKKi9jb3Ntb3Muc3Rha2luZy52MWJldGExLlN0YWtlQXV0aG9yaXphdGlvbhI5IAESNQozZXZtb3N2YWxvcGVyMXVtazQwN2VlZDdhZjZhbnZ1dDZsbGcyemV2bmYwZG4wZmVxcW55EgYI4LD6pgYSfQpZCk8KKC9ldGhlcm1pbnQuY3J5cHRvLnYxLmV0aHNlY3AyNTZrMS5QdWJLZXkSIwohA4B2WHbj6sH/GWE7z/YW5PRnXYFGaGRAov7gZZI2Fv2nEgQKAggBGAMSIAoaCgZhZXZtb3MSEDQ1MjE0NzUwMDAwMDAwMDAQ+4QLGkBXaTo3nk5EMFW9Euheez5ADx2bWo7XisNJ5vuGj1fKXh6CGNJGfJj/q1XUkBzaCvPNg+EcFHgtJdVSyF4cJZTg"}"#
    );
}
