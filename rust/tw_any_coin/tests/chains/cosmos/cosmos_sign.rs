// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

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

/// Tests if it is possible to sign a transaction for a custom chain using `CoinType::Cosmos`.
/// Successfully broadcasted (testnet):
/// https://explorer.zetachain.com/cosmos/tx/A2FC8816657856ED274C4418C3CAEAEE645561275F6C63AB5F8B1DCFB37341A0
#[test]
fn test_any_signer_sign_custom_chain() {
    use tw_proto::Cosmos::Proto;
    use tw_proto::Cosmos::Proto::mod_Message::OneOfmessage_oneof as MessageEnum;

    let send_msg = Proto::mod_Message::Send {
        from_address: "zeta14py36sx57ud82t9yrks9z6hdsrpn5x6kmxs0ne".into(),
        to_address: "zeta1cscf4ldnkkz7f0wpveur6dpd0d6p2zxnsuu70y".into(),
        amounts: vec![Proto::Amount {
            denom: "azeta".into(),
            // 0.3 ZETA
            amount: "300000000000000000".into(),
        }],
        ..Proto::mod_Message::Send::default()
    };

    let input = Proto::SigningInput {
        signing_mode: Proto::SigningMode::Protobuf,
        account_number: 2726346,
        chain_id: "athens_7001-1".into(),
        sequence: 2,
        fee: Some(Proto::Fee {
            gas: 200000,
            amounts: vec![],
        }),
        private_key: "8d2a3bd62d300a148c89dc8635f87b7a24a951bd1c4e78675fe40e1a640d46ed"
            .decode_hex()
            .unwrap()
            .into(),
        messages: vec![Proto::Message {
            message_oneof: MessageEnum::send_coins_message(send_msg),
        }],
        // Use a different Transaction hashing algorithm.
        tx_hasher: Proto::TxHasher::Keccak256,
        signer_info: Some(Proto::SignerInfo {
            // Zetachain requires a compressed public key to sign a transaction,
            // however an uncompressed public key is used to generate address.
            public_key_type: Proto::SignerPublicKeyType::Secp256k1,
            json_type: "ethermint/PubKeyEthSecp256k1".into(),
            protobuf_type: "/ethermint.crypto.v1.ethsecp256k1.PubKey".into(),
        }),
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

    assert_eq!(
        output.serialized,
        r#"{"mode":"BROADCAST_MODE_BLOCK","tx_bytes":"CpoBCpcBChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEncKK3pldGExNHB5MzZzeDU3dWQ4MnQ5eXJrczl6Nmhkc3JwbjV4NmtteHMwbmUSK3pldGExY3NjZjRsZG5ra3o3ZjB3cHZldXI2ZHBkMGQ2cDJ6eG5zdXU3MHkaGwoFYXpldGESEjMwMDAwMDAwMDAwMDAwMDAwMBJhClkKTwooL2V0aGVybWludC5jcnlwdG8udjEuZXRoc2VjcDI1NmsxLlB1YktleRIjCiECho5+FjRBfbKt/Z/jggW/oP6gGJin/TBWXRP3BWo3wGUSBAoCCAEYAhIEEMCaDBpAgGvqca0w2N9wnHnnxS9HiVud4aQ9lNCumzgNIW6wOR4kvPScacGS1G3kwCw7wyI2NJL8M1eVYjafFIt2FpKl3w=="}"#
    );
}
