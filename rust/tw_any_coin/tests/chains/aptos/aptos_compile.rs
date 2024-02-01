// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::aptos::test_cases::transfer_b4d62afd::{
    aptos_sign_transfer_input, expected_json, DATA_TO_SIGN, ENCODED, PRIVATE_KEY, RAW_TXN,
    SIGNATURE,
};
use crate::chains::aptos::APTOS_COIN_TYPE;
use tw_any_coin::ffi::tw_transaction_compiler::{
    tw_transaction_compiler_compile, tw_transaction_compiler_pre_image_hashes,
};
use tw_coin_entry::error::SigningErrorType;
use tw_encoding::hex::ToHex;
use tw_keypair::ed25519;
use tw_keypair::traits::{KeyPairTrait, SigningKeyTrait};
use tw_memory::test_utils::tw_data_helper::TWDataHelper;
use tw_memory::test_utils::tw_data_vector_helper::TWDataVectorHelper;
use tw_misc::assert_eq_json;
use tw_misc::traits::ToBytesVec;
use tw_proto::Aptos::Proto;
use tw_proto::TxCompiler::Proto as CompilerProto;
use tw_proto::{deserialize, serialize};

#[test]
fn test_any_signer_compile_aptos() {
    let input = aptos_sign_transfer_input();

    // Step 2: Obtain preimage hash
    let input_data = TWDataHelper::create(serialize(&input).unwrap());
    let preimage_data = TWDataHelper::wrap(unsafe {
        tw_transaction_compiler_pre_image_hashes(APTOS_COIN_TYPE, input_data.ptr())
    })
    .to_vec()
    .expect("!tw_transaction_compiler_pre_image_hashes returned nullptr");

    let preimage: CompilerProto::PreSigningOutput =
        deserialize(&preimage_data).expect("Coin entry returned an invalid output");

    assert_eq!(preimage.error, SigningErrorType::OK);
    assert!(preimage.error_message.is_empty());
    assert_eq!(preimage.data.to_hex(), DATA_TO_SIGN);

    // Step 3: Sign the data "externally"

    let private_key = ed25519::sha512::KeyPair::try_from(PRIVATE_KEY).unwrap();
    let public_key = private_key.public().to_vec();

    let signature = private_key
        .sign(preimage.data.to_vec())
        .expect("Error signing data")
        .to_vec();
    assert_eq!(signature.to_hex(), SIGNATURE);

    // Step 4: Compile transaction info

    let signatures = TWDataVectorHelper::create([signature]);
    let public_keys = TWDataVectorHelper::create([public_key]);

    let input_data = TWDataHelper::create(serialize(&input).unwrap());
    let output_data = TWDataHelper::wrap(unsafe {
        tw_transaction_compiler_compile(
            APTOS_COIN_TYPE,
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

    let authenticator = output.authenticator.unwrap();
    assert_eq!(authenticator.signature.to_hex(), SIGNATURE);
    assert_eq!(output.raw_txn.to_hex(), RAW_TXN);
    assert_eq!(output.encoded.to_hex(), ENCODED);

    assert_eq_json!(output.json, expected_json());
}
