// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::borrow::Cow;
use tw_any_coin::test_utils::sign_utils::{AnySignerHelper, CompilerHelper, PreImageHelper};
use tw_any_coin::test_utils::transaction_decode_utils::TransactionDecoderHelper;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::base58::{self, Alphabet};
use tw_encoding::base64::{self, STANDARD};
use tw_proto::Common::Proto::SigningError;
use tw_proto::Solana::Proto;
use tw_proto::Solana::Proto::mod_RawMessage as raw_message;
use tw_proto::Solana::Proto::mod_RawMessage::OneOfmessage as MessageType;

fn b58(s: &str) -> Cow<'static, [u8]> {
    base58::decode(s, Alphabet::Bitcoin).unwrap().into()
}

fn check_and_update_recent_blockhash(
    tx: &mut Proto::RawMessage,
    expected_prev: &str,
    updated: &str,
) {
    let recent_blockhash = match tx.message {
        MessageType::legacy(ref mut legacy) => &mut legacy.recent_blockhash,
        MessageType::v0(ref mut v0) => &mut v0.recent_blockhash,
        MessageType::None => unreachable!(),
    };
    assert_eq!(recent_blockhash, expected_prev);
    *recent_blockhash = updated.to_string().into();
}

const PREV_RECENT_BLOCKHASH: &str = "H4gZ56AdmHfZj1F36oWrxDJMUJ8ph7XdTHtmsbtHZshG";
const NEW_RECENT_BLOCKHASH: &str = "CyPYVsYWrsJNfVpi8aazu7WsrswNFuDd385z6GNoBGUg";

const SENDER_PRIVATE_KEY: &str = "9YtuoD4sH4h88CVM8DSnkfoAaLY7YeGC2TarDJ8eyMS5";
const SENDER_PUBLIC_KEY: &str = "B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V";
const FEE_PAYER_PRIVATE_KEY: &str = "66ApBuKpo2uSzpjGBraHq7HP8UZMUJzp3um8FdEjkC9c";
const FEE_PAYER_PUBLIC_KEY: &str = "Eg5jqooyG6ySaXKbQUu4Lpvu2SqUPZrNkM4zXs9iUDLJ";

const PREV_SENDER_SIGNATURE: &str =
    "37UT8U6DdqaWqV6yQuHcRN3JpMefDgVna8LJJPmmDNKYMwmefEgvcreSg9AqSsT7cU2rMBNyDktEtDU19ZqqZvML";
const NEW_SENDER_SIGNATURE: &str =
    "3aqBe5B9uj9PBdhLwbdDkH5X9Wa2E49bDvjaMJyZWj68TeQvAavw9dizTuUkRLLqucgA4NP9SDtT261bPXoHAHEj";
const PREV_FEE_PAYER_SIGNATURE: &str =
    "3KbvREZUat76wgWMtnJfWbJL74Vzh4U2eabVJa3Z3bb2fPtW8AREP5pbmRwUrxZCESbTomWpL41PeKDcPGbojsej";
const NEW_FEE_PAYER_SIGNATURE: &str =
    "2DaVFTx5i3fuvBvyt3s3ti4FajmYa8DCcur5Bjf9cgoFA9CnQRM6RNLTrMQ537WmRkQw9Qw3iojfxVrzQy7jWxT7";

/// https://explorer.solana.com/tx/3KbvREZUat76wgWMtnJfWbJL74Vzh4U2eabVJa3Z3bb2fPtW8AREP5pbmRwUrxZCESbTomWpL41PeKDcPGbojsej?cluster=devnet
const PREV_ENCODED_TX: &str = "AnQTYwZpkm3fs4SdLxnV6gQj3hSLsyacpxDdLMALYWObm722f79IfYFTbZeFK9xHtMumiDOWAM2hHQP4r/GtbARpncaXgOVFv7OgbRLMbuCEJHO1qwcdCbtH72VzyzU8yw9sqqHIAaCUE8xaQTgT6Z5IyZfeyMe2QGJIfOjz65UPAgACBssq8Im1alV3N7wXGODL8jLPWwLhTuCqfGZ1Iz9fb5tXlMOJD6jUvASrKmdtLK/qXNyJns2Vqcvlk+nfJYdZaFpIWiT/tAcEYbttfxyLdYxrLckAKdVRtf1OrNgtZeMCII4SAn6SYaaidrX/AN3s/aVn/zrlEKW0cEUIatHVDKtXO0Qss5EhV/E6kz0BNCgtAytf/s0Botvxt3kGCN8ALqcG3fbh12Whk9nL4UbO63msHLSF7V9bN5E6jPWFfv8Aqe6sdLXiXSDILEtzckCjkjchiSf6zVGpMYiAE5BE2IqHAQUEAgQDAQoMoA8AAAAAAAAG";
const NEW_ENCODED_TX: &str = "Ajzc/Tke0CG8Cew5qFa6xZI/7Ya3DN0M8Ige6tKPsGzhg8Bw9DqL18KUrEZZ1F4YqZBo4Rv+FsDT8A7Nss7p4A6BNVZzzGprCJqYQeNg0EVIbmPc6mDitNniHXGeKgPZ6QZbM4FElw9O7IOFTpOBPvQFeqy0vZf/aayncL8EK/UEAgACBssq8Im1alV3N7wXGODL8jLPWwLhTuCqfGZ1Iz9fb5tXlMOJD6jUvASrKmdtLK/qXNyJns2Vqcvlk+nfJYdZaFpIWiT/tAcEYbttfxyLdYxrLckAKdVRtf1OrNgtZeMCII4SAn6SYaaidrX/AN3s/aVn/zrlEKW0cEUIatHVDKtXO0Qss5EhV/E6kz0BNCgtAytf/s0Botvxt3kGCN8ALqcG3fbh12Whk9nL4UbO63msHLSF7V9bN5E6jPWFfv8AqbHiki6ThNH3auuyZPQpJntnN0mA//56nMpK/6HIuu8xAQUEAgQDAQoMoA8AAAAAAAAG";
const ENCODED_UNSIGNED_MSG: &str = "AgACBssq8Im1alV3N7wXGODL8jLPWwLhTuCqfGZ1Iz9fb5tXlMOJD6jUvASrKmdtLK/qXNyJns2Vqcvlk+nfJYdZaFpIWiT/tAcEYbttfxyLdYxrLckAKdVRtf1OrNgtZeMCII4SAn6SYaaidrX/AN3s/aVn/zrlEKW0cEUIatHVDKtXO0Qss5EhV/E6kz0BNCgtAytf/s0Botvxt3kGCN8ALqcG3fbh12Whk9nL4UbO63msHLSF7V9bN5E6jPWFfv8AqbHiki6ThNH3auuyZPQpJntnN0mA//56nMpK/6HIuu8xAQUEAgQDAQoMoA8AAAAAAAAG";

#[test]
fn test_solana_decode_transaction() {
    let encoded_tx = base64::decode("AQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACAAQAHEIoR5xuWyrvjIW4xU7CWlPOfyFAiy8B295hGo6tNjBmRCgUkQaFYTleMcAX2p74eBXQZd1dwDyQZAPJfSv2KGc5kcFLJj5qd2BVMaSNGVPfVBm74GbLwUq5/U1Ccdqc2gokZQxRDpMq7aeToP3nRaWIP4RXMxN+LJetccXMPq/QumgOqt7kkqk07cyPCKgYoQ4fQtOqqZn5sEqjWHYj3CDS5ha48uggePWu090s1ff4yoCjAvULeZ+cqYFn+Adk5Teyfw71W3u/F6VTnLQEPW96gJr5Kcm3bGi08n224JyF++PTko52VL0CIM2xtl0WkvNslD6Wawxr7yd9HYllN4Lz8lFwXilWGgyJdOq1qqBuZbE49glHeCO/sJHNnIHC0BgAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAwZGb+UhFzL/7K26csOb57yM5bvF9xJrLEObOkAAAAAEedVb8jHAbu50xW7OaBUH/bGy3qP0jlECsc2iVrwTjwbd9uHXZaGT2cvhRs7reawctIXtX1s3kTqM9YV+/wCpjJclj04kifG7PRApFI4NgwtaE5na/xCEBI572Nvp+Fm0P/on9df2SnTAmx8pWHneSwmrNt/J3VFLMhqns4zl6OL4d+g9rsaIj0Orta57MRu3jDSWCJf85ae4LBbiD/GXvOojZjsHekJrpRUuPggLJr943hDVD5UareeEucjCvaoHCgAFAsBcFQAKAAkDBBcBAAAAAAANBgAGACMJDAEBCQIABgwCAAAAAMqaOwAAAAAMAQYBEQs1DA8ABgEFAiMhCwsOCx0MDxoBGQcYBAgDJBscDB4PBwUQEhEfFR8UFwcFISITHw8MDCAfFgstwSCbM0HWnIEAAwAAABEBZAABCh0BAyZHAQMAypo7AAAAAJaWFAYAAAAAMgAADAMGAAABCQPZoILFk7gfE2y5bt3AC+g/4OwNzdiHKBhIbdeYvYFEjQPKyMkExMUkx0R25UNa/g5KsG0vfUwdUJ8e8HecK/Jkd3qm9XefBOB0BaD1+J+dBJz09vfyGuRYZH09HfdE/kL8v6Ql+H03+tO+9lMmmVg8O1c6gAN6eX0Cbn4=", STANDARD).unwrap();

    // Step 1: Decode the transaction.

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
                program_data: b58("K1FDJ7"),
            },
            raw_message::Instruction {
                program_id: 10,
                accounts: vec![],
                program_data: b58("3E9ErJ5MrzbZ"),
            },
            raw_message::Instruction {
                program_id: 13,
                accounts: vec![0, 6, 0, 35, 9, 12],
                program_data: b58("2"),
            },
            raw_message::Instruction {
                program_id: 9,
                accounts: vec![0, 6],
                program_data: b58("3Bxs3zzLZLuLQEYX"),
            },
            raw_message::Instruction {
                program_id: 12,
                accounts: vec![6],
                program_data: b58("J"),
            },
            raw_message::Instruction {
                program_id: 11,
                accounts: vec![
                    12, 15, 0, 6, 1, 5, 2, 35, 33, 11, 11, 14, 11, 29, 12, 15, 26, 1, 25, 7, 24, 4,
                    8, 3, 36, 27, 28, 12, 30, 15, 7, 5, 16, 18, 17, 31, 21, 31, 20, 23, 7, 5, 33,
                    34, 19, 31, 15, 12, 12, 32, 31, 22, 11,
                ],
                program_data: b58("5n9zLuyvSGkuf4iDD6PfDvzvzehUkDghmApUkZSXSx57jF9RGSH5Y23tzFJDG3"),
            },
            raw_message::Instruction {
                program_id: 12,
                accounts: vec![6, 0, 0],
                program_data: b58("A"),
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
        signatures: vec![Proto::PubkeySignature {
            pubkey: "AHy6YZA8BsHgQfVkk7MbwpAN94iyN7Nf1zN4nPqUN32Q".into(),
            signature: "1111111111111111111111111111111111111111111111111111111111111111".into(),
        }],
        message: MessageType::v0(expected_msg),
    };

    assert_eq!(output.transaction, Some(expected));
}

#[test]
fn test_solana_decode_transaction_update_blockhash_and_sign_with_external_fee_payer() {
    // https://explorer.solana.com/tx/3KbvREZUat76wgWMtnJfWbJL74Vzh4U2eabVJa3Z3bb2fPtW8AREP5pbmRwUrxZCESbTomWpL41PeKDcPGbojsej?cluster=devnet
    let encoded_tx = base64::decode(PREV_ENCODED_TX, STANDARD).unwrap();

    // Step 1: Decode the transaction.
    let mut decoder = TransactionDecoderHelper::<Proto::DecodingTransactionOutput>::default();
    let output = decoder.decode(CoinType::Solana, encoded_tx);

    assert_eq!(output.error, SigningError::OK);
    let mut decoded_tx = output.transaction.unwrap();

    let expected_signatures = vec![
        Proto::PubkeySignature {
            pubkey: FEE_PAYER_PUBLIC_KEY.into(),
            signature: PREV_FEE_PAYER_SIGNATURE.into(),
        },
        Proto::PubkeySignature {
            pubkey: SENDER_PUBLIC_KEY.into(),
            signature: PREV_SENDER_SIGNATURE.into(),
        },
    ];
    assert_eq!(decoded_tx.signatures, expected_signatures);

    // Step 2. Update the recent-blockhash.

    check_and_update_recent_blockhash(&mut decoded_tx, PREV_RECENT_BLOCKHASH, NEW_RECENT_BLOCKHASH);

    // Step 3. Re-sign the updated transaction.

    let input = Proto::SigningInput {
        private_key: b58(SENDER_PRIVATE_KEY),
        fee_payer_private_key: b58(FEE_PAYER_PRIVATE_KEY),
        raw_message: Some(decoded_tx),
        tx_encoding: Proto::Encoding::Base64,
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Solana, input);

    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.encoded, NEW_ENCODED_TX);
}

/// The test is similar to `test_solana_decode_transaction_update_blockhash_and_resign_with_external_fee_payer`,
/// but with only difference that the external fee payer private key is not specified,
/// and the corresponding signature is set at [`RawMessage::signatures`].
#[test]
fn test_solana_decode_transaction_update_blockhash_and_sign_with_external_fee_payer_signature() {
    // https://explorer.solana.com/tx/3KbvREZUat76wgWMtnJfWbJL74Vzh4U2eabVJa3Z3bb2fPtW8AREP5pbmRwUrxZCESbTomWpL41PeKDcPGbojsej?cluster=devnet
    let encoded_tx = base64::decode(PREV_ENCODED_TX, STANDARD).unwrap();

    // Step 1: Decode the transaction.
    let mut decoder = TransactionDecoderHelper::<Proto::DecodingTransactionOutput>::default();
    let output = decoder.decode(CoinType::Solana, encoded_tx);

    assert_eq!(output.error, SigningError::OK);
    let mut decoded_tx = output.transaction.unwrap();

    let expected_signatures = vec![
        Proto::PubkeySignature {
            pubkey: FEE_PAYER_PUBLIC_KEY.into(),
            signature: PREV_FEE_PAYER_SIGNATURE.into(),
        },
        Proto::PubkeySignature {
            pubkey: SENDER_PUBLIC_KEY.into(),
            signature: PREV_SENDER_SIGNATURE.into(),
        },
    ];
    assert_eq!(decoded_tx.signatures, expected_signatures);

    // Step 2. Update the recent-blockhash and external fee payer signature.

    decoded_tx.signatures[0].signature = NEW_FEE_PAYER_SIGNATURE.into();

    check_and_update_recent_blockhash(&mut decoded_tx, PREV_RECENT_BLOCKHASH, NEW_RECENT_BLOCKHASH);

    // Step 3. Re-sign the updated transaction.

    let input = Proto::SigningInput {
        private_key: b58(SENDER_PRIVATE_KEY),
        // Do not set the fee payer private key to test if the [`RawMessage::signatures`] is used instead.
        // fee_payer_private_key: b58(FEE_PAYER_PRIVATE_KEY),
        raw_message: Some(decoded_tx),
        tx_encoding: Proto::Encoding::Base64,
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Solana, input);

    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.encoded, NEW_ENCODED_TX);
}

#[test]
fn test_solana_decode_transaction_update_blockhash_and_sign_no_matching_pubkey() {
    let encoded_tx = base64::decode("AQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACAAQAHEIoR5xuWyrvjIW4xU7CWlPOfyFAiy8B295hGo6tNjBmRCgUkQaFYTleMcAX2p74eBXQZd1dwDyQZAPJfSv2KGc5kcFLJj5qd2BVMaSNGVPfVBm74GbLwUq5/U1Ccdqc2gokZQxRDpMq7aeToP3nRaWIP4RXMxN+LJetccXMPq/QumgOqt7kkqk07cyPCKgYoQ4fQtOqqZn5sEqjWHYj3CDS5ha48uggePWu090s1ff4yoCjAvULeZ+cqYFn+Adk5Teyfw71W3u/F6VTnLQEPW96gJr5Kcm3bGi08n224JyF++PTko52VL0CIM2xtl0WkvNslD6Wawxr7yd9HYllN4Lz8lFwXilWGgyJdOq1qqBuZbE49glHeCO/sJHNnIHC0BgAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAwZGb+UhFzL/7K26csOb57yM5bvF9xJrLEObOkAAAAAEedVb8jHAbu50xW7OaBUH/bGy3qP0jlECsc2iVrwTjwbd9uHXZaGT2cvhRs7reawctIXtX1s3kTqM9YV+/wCpjJclj04kifG7PRApFI4NgwtaE5na/xCEBI572Nvp+Fm0P/on9df2SnTAmx8pWHneSwmrNt/J3VFLMhqns4zl6OL4d+g9rsaIj0Orta57MRu3jDSWCJf85ae4LBbiD/GXvOojZjsHekJrpRUuPggLJr943hDVD5UareeEucjCvaoHCgAFAsBcFQAKAAkDBBcBAAAAAAANBgAGACMJDAEBCQIABgwCAAAAAMqaOwAAAAAMAQYBEQs1DA8ABgEFAiMhCwsOCx0MDxoBGQcYBAgDJBscDB4PBwUQEhEfFR8UFwcFISITHw8MDCAfFgstwSCbM0HWnIEAAwAAABEBZAABCh0BAyZHAQMAypo7AAAAAJaWFAYAAAAAMgAADAMGAAABCQPZoILFk7gfE2y5bt3AC+g/4OwNzdiHKBhIbdeYvYFEjQPKyMkExMUkx0R25UNa/g5KsG0vfUwdUJ8e8HecK/Jkd3qm9XefBOB0BaD1+J+dBJz09vfyGuRYZH09HfdE/kL8v6Ql+H03+tO+9lMmmVg8O1c6gAN6eX0Cbn4=", STANDARD).unwrap();

    // Step 1: Decode the transaction.
    let mut decoder = TransactionDecoderHelper::<Proto::DecodingTransactionOutput>::default();
    let output = decoder.decode(CoinType::Solana, encoded_tx);

    assert_eq!(output.error, SigningError::OK);
    let mut decoded_tx = output.transaction.unwrap();

    // Step 2. Clear previous signatures.
    decoded_tx.signatures.clear();

    // Step 3. Re-sign the updated transaction.

    let input = Proto::SigningInput {
        // There is no matching pubkey in the transaction account keys.
        private_key: b58("A7psj2GW7ZMdY4E5hJq14KMeYg7HFjULSsWSrTXZLvYr"),
        raw_message: Some(decoded_tx),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Solana, input);

    assert_eq!(output.error, SigningError::Error_missing_private_key);
}

#[test]
fn test_solana_decode_transaction_update_blockhash_preimage_hash_and_compile() {
    // https://explorer.solana.com/tx/3KbvREZUat76wgWMtnJfWbJL74Vzh4U2eabVJa3Z3bb2fPtW8AREP5pbmRwUrxZCESbTomWpL41PeKDcPGbojsej?cluster=devnet
    let encoded_tx = base64::decode(PREV_ENCODED_TX, STANDARD).unwrap();

    // Step 1: Decode the transaction.
    let mut decoder = TransactionDecoderHelper::<Proto::DecodingTransactionOutput>::default();
    let decode_output = decoder.decode(CoinType::Solana, encoded_tx);

    assert_eq!(decode_output.error, SigningError::OK);
    let mut decoded_tx = decode_output.transaction.unwrap();

    // Step 2. Update recent blockhash.
    check_and_update_recent_blockhash(&mut decoded_tx, PREV_RECENT_BLOCKHASH, NEW_RECENT_BLOCKHASH);

    // Step 3. Pre-image hash the transaction.

    let input = Proto::SigningInput {
        raw_message: Some(decoded_tx),
        tx_encoding: Proto::Encoding::Base64,
        ..Proto::SigningInput::default()
    };

    let mut preimager = PreImageHelper::<Proto::PreSigningOutput>::default();
    let preimage_output = preimager.pre_image_hashes(CoinType::Solana, &input);

    assert_eq!(preimage_output.error, SigningError::OK);
    let expected_unsigned_msg = base64::decode(ENCODED_UNSIGNED_MSG, STANDARD).unwrap();
    assert_eq!(preimage_output.data, expected_unsigned_msg);

    // Step 4. Compile the updated transaction with signatures.

    let fee_payer_pubkey = b58(FEE_PAYER_PUBLIC_KEY).to_vec();
    let sender_pubkey = b58(SENDER_PUBLIC_KEY).to_vec();

    let fee_payer_signature = b58(NEW_FEE_PAYER_SIGNATURE).to_vec();
    let sender_signature = b58(NEW_SENDER_SIGNATURE).to_vec();

    let mut compiler = CompilerHelper::<Proto::SigningOutput>::default();
    let compile_output = compiler.compile(
        CoinType::Solana,
        &input,
        vec![fee_payer_signature, sender_signature],
        vec![fee_payer_pubkey, sender_pubkey],
    );

    assert_eq!(compile_output.error, SigningError::OK);
    assert_eq!(compile_output.encoded, NEW_ENCODED_TX);
}
