// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::borrow::Cow;

use crate::chains::sui::test_cases::{transfer_d4ay9tdb, PRIVATE_KEY_54E80D76, SENDER_54E80D76};
use tw_any_coin::test_utils::sign_utils::{CompilerHelper, PreImageHelper};
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_keypair::ed25519;
use tw_keypair::traits::{KeyPairTrait, SigningKeyTrait};
use tw_misc::traits::ToBytesVec;
use tw_proto::Common::Proto::SigningError;
use tw_proto::Sui::Proto::{self, mod_SigningInput::OneOftransaction_payload as TransactionType};
use tw_proto::TxCompiler::Proto as CompilerProto;

struct SuiCompileArgs<'a> {
    input: Proto::SigningInput<'a>,
    private_key: &'a str,
    tx_hash: &'a str,
    unsigned_tx_data: &'a str,
    signature: &'a str,
}

fn test_sui_compile_impl(args: SuiCompileArgs) {
    // Step 2: Obtain preimage hash
    let mut pre_imager = PreImageHelper::<CompilerProto::PreSigningOutput>::default();
    let preimage_output = pre_imager.pre_image_hashes(CoinType::Sui, &args.input);

    assert_eq!(preimage_output.error, SigningError::OK);
    assert_eq!(preimage_output.data_hash.to_hex(), args.tx_hash);

    // Step 3: Compile transaction info

    // Simulate external signing.
    let key_pair = ed25519::sha512::KeyPair::try_from(args.private_key).unwrap();
    let public_key_bytes = key_pair.public().to_vec();
    let signature_bytes = key_pair
        .sign(preimage_output.data_hash.to_vec())
        .unwrap()
        .to_vec();

    let mut compiler = CompilerHelper::<Proto::SigningOutput>::default();
    let output = compiler.compile(
        CoinType::Sui,
        &args.input,
        vec![signature_bytes],
        vec![public_key_bytes],
    );

    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.unsigned_tx, args.unsigned_tx_data);
    assert_eq!(output.signature, args.signature);
}

#[test]
fn test_sui_compile_transfer() {
    let input = Proto::SigningInput {
        signer: SENDER_54E80D76.into(),
        ..transfer_d4ay9tdb::sui_transfer_input()
    };

    // Successfully broadcasted: https://suiscan.xyz/mainnet/tx/D4Ay9TdBJjXkGmrZSstZakpEWskEQHaWURP6xWPRXbAm
    test_sui_compile_impl(SuiCompileArgs {
        input,
        private_key: PRIVATE_KEY_54E80D76,
        tx_hash: transfer_d4ay9tdb::TX_HASH,
        unsigned_tx_data: transfer_d4ay9tdb::UNSIGNED_TX,
        signature: transfer_d4ay9tdb::SIGNATURE,
    });
}

#[test]
fn test_sui_compile_direct_transfer() {
    let direct = Proto::SignDirect {
        unsigned_tx_msg: transfer_d4ay9tdb::UNSIGNED_TX.into(),
    };

    let input = Proto::SigningInput {
        transaction_payload: TransactionType::sign_direct_message(direct),
        private_key: PRIVATE_KEY_54E80D76.decode_hex().unwrap().into(),
        ..Proto::SigningInput::default()
    };

    // Successfully broadcasted: https://suiscan.xyz/mainnet/tx/D4Ay9TdBJjXkGmrZSstZakpEWskEQHaWURP6xWPRXbAm
    test_sui_compile_impl(SuiCompileArgs {
        input,
        private_key: PRIVATE_KEY_54E80D76,
        tx_hash: transfer_d4ay9tdb::TX_HASH,
        unsigned_tx_data: transfer_d4ay9tdb::UNSIGNED_TX,
        signature: transfer_d4ay9tdb::SIGNATURE,
    });
}

#[test]
fn test_sui_compile_raw_json() {
    let raw_json = Cow::Borrowed(include_str!("fixtures/aftermath_tx_1.json"));
    let input = Proto::SigningInput {
        transaction_payload: TransactionType::raw_json(raw_json),
        private_key: PRIVATE_KEY_54E80D76.decode_hex().unwrap().into(),
        ..Proto::SigningInput::default()
    };

    let expected_json = include_str!("./fixtures/aftermath_tx_1_serialized.json"); // Generated via aftermath-sdk
    let expected: serde_json::Value = serde_json::from_str(expected_json).unwrap();
    let expected_unsigned_tx_data = expected["serializedTransaction"].as_str().unwrap();
    test_sui_compile_impl(SuiCompileArgs {
        input,
        private_key: PRIVATE_KEY_54E80D76,
        tx_hash: "75fe8ed844ab7e84c18051e808693b22c63a7e291d66b2e3a9336cc20730ac0e",
        unsigned_tx_data: expected_unsigned_tx_data,
        signature: "ABl18CtTKml1sbI+HC1ciDlew7NiizEUK2KYfOgEDFVvrCcYbV2TSQI6lBkT710s+L+HrASGVvxVj/igpgB+dAyF69FEH+T5VPvl3GB3vwCOEZpeJpKXxvcIPQAdKsh2/g=="
    });
}
