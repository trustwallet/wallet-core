// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::test_utils::sign_utils::{CompilerHelper, PreImageHelper};
use tw_coin_entry::test_utils::test_context::TestCoinContext;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex;
use tw_keypair::tw::PrivateKey;
use tw_proto::Common::Proto::SigningError;
use tw_proto::Tezos::Proto;
use tw_proto::Tezos::Proto::mod_Operation::OperationKind;
use tw_proto::TxCompiler::Proto as CompilerProto;
use tw_tezos::compiler::TezosCompiler;
use tw_tezos::signer::TezosSigner;

#[test]
fn test_tezos_compile_with_signatures() {
    let private_key = PrivateKey::new(
        hex::decode("2e8905819b8723fe2c1d161860e5ee1830318dbf49a83bd451cfb8440c28bd6f").unwrap(),
    )
    .unwrap();
    let reveal_key =
        hex::decode("311f002e899cdd9a52d96cb8be18ea2bbab867c505da2b44ce10906f511cff95").unwrap();

    let mut input = Proto::SigningInput::default();
    let mut operations = Proto::OperationList::default();
    operations.branch = "BL8euoCWqNCny9AR3AKjnpi38haYMxjei1ZqNHuXMn19JSQnoWp".into();

    let mut reveal = Proto::Operation::default();
    reveal.source = "tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW".into();
    reveal.fee = 1272;
    reveal.counter = 30738;
    reveal.gas_limit = 10100;
    reveal.storage_limit = 257;
    reveal.kind = OperationKind::REVEAL;
    reveal.operation_data = Proto::mod_Operation::OneOfoperation_data::reveal_operation_data(
        Proto::RevealOperationData {
            public_key: reveal_key.into(),
        },
    );
    operations.operations.push(reveal);

    let mut transaction = Proto::Operation::default();
    transaction.source = "tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW".into();
    transaction.fee = 1272;
    transaction.counter = 30739;
    transaction.gas_limit = 10100;
    transaction.storage_limit = 257;
    transaction.kind = OperationKind::TRANSACTION;
    transaction.operation_data =
        Proto::mod_Operation::OneOfoperation_data::transaction_operation_data(
            Proto::TransactionOperationData {
                amount: 1,
                destination: "tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW".into(),
                encoded_parameter: vec![].into(),
                parameters: None,
            },
        );
    operations.operations.push(transaction);

    input.operation_list = Some(operations);

    // Step 2: Obtain preimage hash
    let mut pre_imager = PreImageHelper::<CompilerProto::PreSigningOutput>::default();
    let preimage_output = pre_imager.pre_image_hashes(CoinType::Tezos, &input);
    assert_eq!(preimage_output.error, SigningError::OK);
    assert_eq!(
        hex::encode(&preimage_output.data_hash, false),
        "12e4f8b17ad3b316a5a56960db76c7d6505dbf2fff66106be75c8d6753daac0e"
    );

    let signature = hex::decode("0217034271b815e5f0c0a881342838ce49d7b48cdf507c72b1568c69a10db70c98774cdad1a74df760763e25f760ff13afcbbf3a1f2c833a0beeb9576a579c05").unwrap();

    // Step 3: Compile transaction info
    let expected_tx = "3756ef37b1be849e3114643f0aa5847cabf9a896d3bfe4dd51448de68e91da016b0081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80992f001f44e810200311f002e899cdd9a52d96cb8be18ea2bbab867c505da2b44ce10906f511cff956c0081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80993f001f44e810201000081faa75f741ef614b0e35fcc8c90dfa3b0b95721000217034271b815e5f0c0a881342838ce49d7b48cdf507c72b1568c69a10db70c98774cdad1a74df760763e25f760ff13afcbbf3a1f2c833a0beeb9576a579c05";

    let mut compiler = CompilerHelper::<Proto::SigningOutput>::default();
    let output = compiler.compile(CoinType::Tezos, &input, vec![signature.clone()], vec![]);
    assert_eq!(output.error, SigningError::OK);
    assert_eq!(hex::encode(&output.encoded, false), expected_tx);

    // Double check with direct signing
    input.private_key = private_key.bytes().into();
    let signed_output = TezosSigner::sign(&TestCoinContext::default(), input.clone());
    assert_eq!(hex::encode(&signed_output.encoded, false), expected_tx);

    // Negative: inconsistent signatures & publicKeys
    let output = TezosCompiler::compile(
        &TestCoinContext::default(),
        input,
        vec![signature.clone(), signature],
        vec![],
    );
    assert_eq!(output.encoded.len(), 0);
    assert_eq!(output.error, SigningError::Error_signatures_count);
}
