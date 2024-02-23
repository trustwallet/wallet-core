// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::borrow::Cow;
use tw_any_coin::test_utils::transaction_decode_utils::TransactionDecoderHelper;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::base58::Alphabet;
use tw_encoding::hex::DecodeHex;
use tw_encoding::{base58, base64};
use tw_proto::Common::Proto::SigningError;
use tw_proto::Solana::Proto;
use tw_proto::Solana::Proto::mod_RawMessage as raw_message;
use tw_proto::Solana::Proto::mod_RawMessage::OneOfmessage as MessageType;

fn base58_decode(s: &str) -> Cow<'static, [u8]> {
    base58::decode(s, Alphabet::BITCOIN).unwrap().into()
}

#[test]
fn test_solana_decode_transaction() {
    let encoded_tx = base64::decode("AQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACAAQAHEIoR5xuWyrvjIW4xU7CWlPOfyFAiy8B295hGo6tNjBmRCgUkQaFYTleMcAX2p74eBXQZd1dwDyQZAPJfSv2KGc5kcFLJj5qd2BVMaSNGVPfVBm74GbLwUq5/U1Ccdqc2gokZQxRDpMq7aeToP3nRaWIP4RXMxN+LJetccXMPq/QumgOqt7kkqk07cyPCKgYoQ4fQtOqqZn5sEqjWHYj3CDS5ha48uggePWu090s1ff4yoCjAvULeZ+cqYFn+Adk5Teyfw71W3u/F6VTnLQEPW96gJr5Kcm3bGi08n224JyF++PTko52VL0CIM2xtl0WkvNslD6Wawxr7yd9HYllN4Lz8lFwXilWGgyJdOq1qqBuZbE49glHeCO/sJHNnIHC0BgAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAwZGb+UhFzL/7K26csOb57yM5bvF9xJrLEObOkAAAAAEedVb8jHAbu50xW7OaBUH/bGy3qP0jlECsc2iVrwTjwbd9uHXZaGT2cvhRs7reawctIXtX1s3kTqM9YV+/wCpjJclj04kifG7PRApFI4NgwtaE5na/xCEBI572Nvp+Fm0P/on9df2SnTAmx8pWHneSwmrNt/J3VFLMhqns4zl6OL4d+g9rsaIj0Orta57MRu3jDSWCJf85ae4LBbiD/GXvOojZjsHekJrpRUuPggLJr943hDVD5UareeEucjCvaoHCgAFAsBcFQAKAAkDBBcBAAAAAAANBgAGACMJDAEBCQIABgwCAAAAAMqaOwAAAAAMAQYBEQs1DA8ABgEFAiMhCwsOCx0MDxoBGQcYBAgDJBscDB4PBwUQEhEfFR8UFwcFISITHw8MDCAfFgstwSCbM0HWnIEAAwAAABEBZAABCh0BAyZHAQMAypo7AAAAAJaWFAYAAAAAMgAADAMGAAABCQPZoILFk7gfE2y5bt3AC+g/4OwNzdiHKBhIbdeYvYFEjQPKyMkExMUkx0R25UNa/g5KsG0vfUwdUJ8e8HecK/Jkd3qm9XefBOB0BaD1+J+dBJz09vfyGuRYZH09HfdE/kL8v6Ql+H03+tO+9lMmmVg8O1c6gAN6eX0Cbn4=", false).unwrap();

    let mut decoder = TransactionDecoderHelper::<Proto::DecodingTransactionOutput>::default();
    let output = decoder.decode(CoinType::Solana, encoded_tx);

    assert_eq!(output.error, SigningError::OK);

    let expected_msg = raw_message::MessageV0 {
        header: Some(raw_message::MessageHeader {
            num_required_signatures: 1,
            num_readonly_signed_accounts: 0,
            num_readonly_unsigned_accounts: 7,
        }),
        account_keys: vec![
            "AHy6YZA8BsHgQfVkk7MbwpAN94iyN7Nf1zN4nPqUN32Q".into(),
            "g7dD1FHSemkUQrX1Eak37wzvDjscgBW2pFCENwjLdMX".into(),
            "7m57LBTxtzhWn6WdFxKtnoJLBQXyNERLYebebXLVaKy3".into(),
            "AEBCPtV8FFkWFAKxrz7mbYvobpkZuWaRWQCyJVRaheUD".into(),
            "BND2ehwWVeHVA5EtMm2b7Vu51AT8f2PNWusS9KQX5moy".into(),
            "DVCeozFGbe6ew3eWTnZByjHeYqTq1cvbrB7JJhkLxaRJ".into(),
            "GvgWmk8iPACw1AEMt47WzkuTkKoSGbn4Xk3aLM8vdbJD".into(),
            "HkphEpUqnFBxBuCPEq5j1HA9L8EwmsmRT6UcFKziptM1".into(),
            "Hzxx6b5a7dmmJeDXLQzr4dTrc2HGK9ar5YRakZgr3ZZ7".into(),
            "11111111111111111111111111111111".into(),
            "ComputeBudget111111111111111111111111111111".into(),
            "JUP6LkbZbjS1jKKwapdHNy74zcZ3tLUZoi5QNyVTaV4".into(),
            "TokenkegQfeZyiNwAJbNbGKPFXCWuBvf9Ss623VQ5DA".into(),
            "ATokenGPvbdGVxr1b2hvZbsiqW5xWH25efTNsLJA8knL".into(),
            "D8cy77BBepLMngZx6ZukaTff5hCt1HrWyKk3Hnd9oitf".into(),
            "GGztQqQ6pCPaJQnNpXBgELr5cs3WwDakRbh1iEMzjgSJ".into(),
        ],
        recent_blockhash: "DiSimxK2z1cRa6yD4goqte3rDMmghJAD8WDUZEab2CzD".into(),
        instructions: vec![
            raw_message::Instruction {
                program_id: 10,
                accounts: vec![],
                program_data: base58_decode("K1FDJ7"),
            },
            raw_message::Instruction {
                program_id: 10,
                accounts: vec![],
                program_data: base58_decode("3E9ErJ5MrzbZ"),
            },
            raw_message::Instruction {
                program_id: 13,
                accounts: vec![0, 6, 0, 35, 9, 12],
                program_data: base58_decode("2"),
            },
            raw_message::Instruction {
                program_id: 9,
                accounts: vec![0, 6],
                program_data: base58_decode("3Bxs3zzLZLuLQEYX"),
            },
            raw_message::Instruction {
                program_id: 12,
                accounts: vec![6],
                program_data: base58_decode("J"),
            },
            raw_message::Instruction {
                program_id: 11,
                accounts: vec![
                    12, 15, 0, 6, 1, 5, 2, 35, 33, 11, 11, 14, 11, 29, 12, 15, 26, 1, 25, 7, 24, 4,
                    8, 3, 36, 27, 28, 12, 30, 15, 7, 5, 16, 18, 17, 31, 21, 31, 20, 23, 7, 5, 33,
                    34, 19, 31, 15, 12, 12, 32, 31, 22, 11,
                ],
                program_data: base58_decode(
                    "5n9zLuyvSGkuf4iDD6PfDvzvzehUkDghmApUkZSXSx57jF9RGSH5Y23tzFJDG3",
                ),
            },
            raw_message::Instruction {
                program_id: 12,
                accounts: vec![6, 0, 0],
                program_data: base58_decode("A"),
            },
        ],
        address_table_lookups: vec![
            raw_message::MessageAddressTableLookup {
                account_key: "FeXRmSWmwChZbB2EC7Qjw9XKk28yBrPj3k3nzT1DKfak".into(),
                writable_indexes: vec![202, 200, 201],
                readonly_indexes: vec![196, 197, 36, 199],
            },
            raw_message::MessageAddressTableLookup {
                account_key: "5cFsmTCEfmvpBUBHqsWZnf9n5vTWLYH2LT8X7HdShwxP".into(),
                writable_indexes: vec![160, 245, 248, 159, 157],
                readonly_indexes: vec![156, 244, 246, 247],
            },
            raw_message::MessageAddressTableLookup {
                account_key: "HJ5StCvsDU4JsvK39VcsHjaoTRTtQU749MQ9qUsJaG1m".into(),
                writable_indexes: vec![122, 121, 125],
                readonly_indexes: vec![110, 126],
            },
        ],
    };
    let expected = Proto::RawMessage {
        signatures: vec![
            raw_message::PubkeySignature {
                pubkey: "AHy6YZA8BsHgQfVkk7MbwpAN94iyN7Nf1zN4nPqUN32Q".into(),
                signature: "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
                    .decode_hex()
                    .unwrap()
                    .into()
            }
        ],
        message: MessageType::v0(expected_msg),
    };

    assert_eq!(output.transaction, Some(expected));
}
