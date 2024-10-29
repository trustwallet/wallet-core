// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::pactus::test_cases::transfer::{
    pactus_sign_transfer_input, TRANSFER_SIGNATURE, TRANSFER_SIGN_BYTES, TRANSACTION_TRANSFER_SIGNED_DATA, TRANSFER_TX_ID,
};
use tw_any_coin::ffi::tw_transaction_compiler::{
    tw_transaction_compiler_compile, tw_transaction_compiler_pre_image_hashes,
};
use tw_coin_entry::error::prelude::*;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::ToHex;
use tw_keypair::ed25519;
use tw_keypair::traits::{KeyPairTrait, SigningKeyTrait};
use tw_memory::test_utils::tw_data_helper::TWDataHelper;
use tw_memory::test_utils::tw_data_vector_helper::TWDataVectorHelper;
use tw_misc::traits::ToBytesVec;
use tw_proto::Pactus::Proto;
use tw_proto::TxCompiler::Proto as CompilerProto;
use tw_proto::{deserialize, serialize};
use crate::chains::pactus::test_cases::PRIVATE_KEY;

#[test]
fn test_pactus_compile() {
    // Step 1: Create signing input.
    let input = pactus_sign_transfer_input();

    // Step 2: Obtain preimage hash
    let input_data = TWDataHelper::create(serialize(&input).unwrap());
    let preimage_data = TWDataHelper::wrap(unsafe {
        tw_transaction_compiler_pre_image_hashes(CoinType::Pactus as u32, input_data.ptr())
    })
    .to_vec()
    .expect("!tw_transaction_compiler_pre_image_hashes returned nullptr");

    let preimage: CompilerProto::PreSigningOutput =
        deserialize(&preimage_data).expect("Coin entry returned an invalid output");

    assert_eq!(preimage.error, SigningErrorType::OK);
    assert!(preimage.error_message.is_empty());
    assert_eq!(preimage.data.to_hex(), TRANSFER_SIGN_BYTES);

    // Step 3: Sign the data "externally"
    let private_key = ed25519::sha512::KeyPair::try_from(PRIVATE_KEY).unwrap();
    let public_key = private_key.public().to_vec();

    let signature = private_key
        .sign(preimage.data.to_vec())
        .expect("Error signing data")
        .to_vec();
    assert_eq!(signature.to_hex(), TRANSFER_SIGNATURE);

    // Step 4: Compile transaction info

    let signatures = TWDataVectorHelper::create([signature]);
    let public_keys = TWDataVectorHelper::create([public_key]);

    let input_data = TWDataHelper::create(serialize(&input).unwrap());
    let output_data = TWDataHelper::wrap(unsafe {
        tw_transaction_compiler_compile(
            CoinType::Pactus as u32,
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
    assert_eq!(output.transaction_id.to_hex(), TRANSFER_TX_ID);
    assert_eq!(output.signature.to_hex(), TRANSFER_SIGNATURE);
    assert_eq!(
        output.signed_transaction_data.to_hex(),
        TRANSACTION_TRANSFER_SIGNED_DATA
    );
}
