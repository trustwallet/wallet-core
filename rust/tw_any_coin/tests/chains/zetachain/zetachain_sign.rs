// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::ffi::tw_any_signer::tw_any_signer_sign;
use tw_coin_entry::error::SigningErrorType;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_memory::test_utils::tw_data_helper::TWDataHelper;
use tw_proto::Cosmos::Proto::{self, mod_Message::OneOfmessage_oneof as MessageEnum};
use tw_proto::{deserialize, serialize};

const PRIVATE_KEY: &str = "8d2a3bd62d300a148c89dc8635f87b7a24a951bd1c4e78675fe40e1a640d46ed";

/// Successfully broadcasted (testnet):
/// https://explorer.zetachain.com/cosmos/tx/A2FC8816657856ED274C4418C3CAEAEE645561275F6C63AB5F8B1DCFB37341A0
#[test]
fn test_zetachain_sign_msg_send_testnet() {
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
        private_key: PRIVATE_KEY.decode_hex().unwrap().into(),
        messages: vec![Proto::Message {
            message_oneof: MessageEnum::send_coins_message(send_msg),
        }],
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
        tw_any_signer_sign(input_data.ptr(), CoinType::NativeZetaChain as u32)
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
    assert_eq!(output.signature.to_hex(), "806bea71ad30d8df709c79e7c52f47895b9de1a43d94d0ae9b380d216eb0391e24bcf49c69c192d46de4c02c3bc322363492fc33579562369f148b761692a5df");
    assert_eq!(
        output.signature_json,
        r#"[{"pub_key":{"type":"ethermint/PubKeyEthSecp256k1","value":"AoaOfhY0QX2yrf2f44IFv6D+oBiYp/0wVl0T9wVqN8Bl"},"signature":"gGvqca0w2N9wnHnnxS9HiVud4aQ9lNCumzgNIW6wOR4kvPScacGS1G3kwCw7wyI2NJL8M1eVYjafFIt2FpKl3w=="}]"#
    );
}
