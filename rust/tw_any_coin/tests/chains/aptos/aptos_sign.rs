// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::aptos::test_cases::transfer_b4d62afd::{
    aptos_sign_transfer_input, expected_json, ENCODED, PRIVATE_KEY, RAW_TXN, SIGNATURE,
};
use crate::chains::aptos::APTOS_COIN_TYPE;
use tw_any_coin::ffi::tw_any_signer::tw_any_signer_sign;
use tw_coin_entry::error::SigningErrorType;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_memory::test_utils::tw_data_helper::TWDataHelper;
use tw_misc::assert_eq_json;
use tw_proto::Aptos::Proto;
use tw_proto::{deserialize, serialize};

#[test]
fn test_any_signer_sign_aptos() {
    let input = Proto::SigningInput {
        private_key: PRIVATE_KEY.decode_hex().unwrap().into(),
        ..aptos_sign_transfer_input()
    };

    let input_data = TWDataHelper::create(serialize(&input).unwrap());

    let output =
        TWDataHelper::wrap(unsafe { tw_any_signer_sign(input_data.ptr(), APTOS_COIN_TYPE) })
            .to_vec()
            .expect("!tw_any_signer_sign returned nullptr");

    let output: Proto::SigningOutput = deserialize(&output).unwrap();
    assert_eq!(output.error, SigningErrorType::OK);
    assert!(output.error_message.is_empty());

    let authenticator = output.authenticator.unwrap();
    assert_eq!(authenticator.signature.to_hex(), SIGNATURE);
    assert_eq!(output.raw_txn.to_hex(), RAW_TXN);
    assert_eq!(output.encoded.to_hex(), ENCODED);

    assert_eq_json!(output.json, expected_json());
}
