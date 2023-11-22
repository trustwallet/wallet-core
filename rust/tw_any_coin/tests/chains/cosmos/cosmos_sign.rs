// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_any_coin::ffi::tw_any_signer::tw_any_signer_sign;
use tw_coin_entry::error::SigningErrorType;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::DecodeHex;
use tw_memory::test_utils::tw_data_helper::TWDataHelper;
use tw_proto::{deserialize, serialize};

#[test]
fn test_any_signer_sign_cosmos() {
    use tw_proto::Cosmos::Proto;
    use tw_proto::Cosmos::Proto::mod_Message::OneOfmessage_oneof as MessageEnum;

    let private_key = "8bbec3772ddb4df68f3186440380c301af116d1422001c1877d6f5e4dba8c8af"
        .decode_hex()
        .unwrap();

    let send_msg = Proto::mod_Message::Send {
        from_address: "cosmos1mky69cn8ektwy0845vec9upsdphktxt03gkwlx".into(),
        to_address: "cosmos18s0hdnsllgcclweu9aymw4ngktr2k0rkygdzdp".into(),
        amounts: vec![Proto::Amount {
            denom: "uatom".into(),
            amount: "400000".into(),
        }],
        ..Proto::mod_Message::Send::default()
    };
    let input = Proto::SigningInput {
        signing_mode: Proto::SigningMode::Protobuf,
        account_number: 546179,
        chain_id: "cosmoshub-4".into(),
        sequence: 0,
        fee: Some(Proto::Fee {
            gas: 200000,
            amounts: vec![Proto::Amount {
                denom: "uatom".into(),
                amount: "1000".into(),
            }],
        }),
        private_key: private_key.into(),
        messages: vec![Proto::Message {
            message_oneof: MessageEnum::send_coins_message(send_msg),
        }],
        ..Proto::SigningInput::default()
    };

    let input_data = TWDataHelper::create(serialize(&input).unwrap());

    let output = TWDataHelper::wrap(unsafe {
        tw_any_signer_sign(input_data.ptr(), CoinType::Cosmos as u32)
    })
    .to_vec()
    .expect("!tw_any_signer_sign returned nullptr");

    let output: Proto::SigningOutput = deserialize(&output).unwrap();
    assert_eq!(output.error, SigningErrorType::OK);
    assert!(output.error_message.is_empty());

    let expected = r#"{"mode":"BROADCAST_MODE_BLOCK","tx_bytes":"CpIBCo8BChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEm8KLWNvc21vczFta3k2OWNuOGVrdHd5MDg0NXZlYzl1cHNkcGhrdHh0MDNna3dseBItY29zbW9zMThzMGhkbnNsbGdjY2x3ZXU5YXltdzRuZ2t0cjJrMHJreWdkemRwGg8KBXVhdG9tEgY0MDAwMDASZQpOCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohAuzvXOQ3owLGf5VGjeSzHzbpEfRn1+alK0HB4T4dVjZJEgQKAggBEhMKDQoFdWF0b20SBDEwMDAQwJoMGkCvvVE6d29P30cO9/lnXyGunWMPxNY12NuqDcCnFkNM0H4CUQdl1Gc9+ogIJbro5nyzZzlv9rl2/GsZox/JXoCX"}"#;
    assert_eq!(output.serialized, expected);
}
