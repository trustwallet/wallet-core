// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::thorchain::test_cases::send_fd0445af::{
    signing_input, JSON_SIGNING_SIGNATURE, JSON_SIGNING_SIGNATURE_JSON, JSON_TX, JSON_TX_PREIMAGE,
    PRIVATE_KEY,
};
use tw_any_coin::ffi::tw_transaction_compiler::{
    tw_transaction_compiler_compile, tw_transaction_compiler_pre_image_hashes,
};
use tw_coin_entry::error::SigningErrorType;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::ToHex;
use tw_hash::H256;
use tw_keypair::ecdsa::secp256k1;
use tw_keypair::traits::{KeyPairTrait, SigningKeyTrait};
use tw_memory::test_utils::tw_data_helper::TWDataHelper;
use tw_memory::test_utils::tw_data_vector_helper::TWDataVectorHelper;
use tw_misc::traits::ToBytesVec;
use tw_proto::Cosmos::Proto;
use tw_proto::TxCompiler::Proto as CompilerProto;
use tw_proto::{deserialize, serialize};

#[test]
fn test_any_signer_compile_thorchain() {
    let private_key = secp256k1::KeyPair::try_from(PRIVATE_KEY).unwrap();
    let public_key = private_key.public().to_vec();

    let input = Proto::SigningInput {
        signing_mode: Proto::SigningMode::JSON,
        public_key: public_key.clone().into(),
        ..signing_input()
    };

    // Step 2: Obtain preimage hash
    let input_data = TWDataHelper::create(serialize(&input).unwrap());
    let preimage_data = TWDataHelper::wrap(unsafe {
        tw_transaction_compiler_pre_image_hashes(CoinType::THORChain as u32, input_data.ptr())
    })
    .to_vec()
    .expect("!tw_transaction_compiler_pre_image_hashes returned nullptr");

    let preimage: CompilerProto::PreSigningOutput =
        deserialize(&preimage_data).expect("Coin entry returned an invalid output");

    assert_eq!(preimage.error, SigningErrorType::OK);
    assert!(preimage.error_message.is_empty());
    let tx_preimage = String::from_utf8(preimage.data.to_vec())
        .expect("Invalid transaction preimage. Expected a JSON object");
    assert_eq!(tx_preimage, JSON_TX_PREIMAGE);

    // Step 3: Sign the data "externally"

    let tx_hash = H256::try_from(preimage.data_hash.as_ref()).expect("Invalid Transaction Hash");

    let signature = private_key
        .sign(tx_hash)
        .expect("Error signing data")
        .to_vec();
    assert!(
        signature.to_hex().contains(JSON_SIGNING_SIGNATURE),
        "{} must contain {}",
        signature.to_hex(),
        JSON_SIGNING_SIGNATURE
    );

    // Step 4: Compile transaction info

    let signatures = TWDataVectorHelper::create([signature]);
    let public_keys = TWDataVectorHelper::create([public_key]);

    let input_data = TWDataHelper::create(serialize(&input).unwrap());
    let output_data = TWDataHelper::wrap(unsafe {
        tw_transaction_compiler_compile(
            CoinType::THORChain as u32,
            input_data.ptr(),
            signatures.ptr(),
            public_keys.ptr(),
        )
    })
    .to_vec()
    .expect("!tw_transaction_compiler_compile returned nullptr");

    let output: Proto::SigningOutput =
        deserialize(&output_data).expect("Coin entry returned an invalid output");

    assert_eq!(output.error, SigningErrorType::OK);
    assert!(output.error_message.is_empty());

    // https://viewblock.io/thorchain/tx/FD0445AFFC4ED9ACCB7B5D3ADE361DAE4596EA096340F1360F1020381EA454AF
    assert_eq!(output.json, JSON_TX);
    assert_eq!(output.signature.to_hex(), JSON_SIGNING_SIGNATURE);
    assert_eq!(output.signature_json, JSON_SIGNING_SIGNATURE_JSON);
}
