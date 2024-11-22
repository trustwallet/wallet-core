// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::pactus::test_cases::PRIVATE_KEY;
use crate::chains::pactus::test_cases::TEST_CASES;
use tw_any_coin::ffi::tw_any_signer::tw_any_signer_sign;
use tw_coin_entry::error::prelude::*;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_memory::test_utils::tw_data_helper::TWDataHelper;
use tw_proto::Pactus::Proto;
use tw_proto::{deserialize, serialize};

#[test]
fn test_pactus_sign_transactions() {
    for case in TEST_CASES.iter() {
        let input = Proto::SigningInput {
            private_key: PRIVATE_KEY.decode_hex().unwrap().into(),
            ..(case.sign_input_fn)()
        };

        let input_data = TWDataHelper::create(serialize(&input).unwrap());

        let output = TWDataHelper::wrap(unsafe {
            tw_any_signer_sign(input_data.ptr(), CoinType::Pactus as u32)
        })
        .to_vec()
        .expect("!tw_any_signer_sign returned nullptr");

        let output: Proto::SigningOutput = deserialize(&output).unwrap();

        assert_eq!(output.error, SigningErrorType::OK);
        assert!(output.error_message.is_empty());
        assert_eq!(output.transaction_id.to_hex(), case.transaction_id);
        assert_eq!(output.signature.to_hex(), case.signature);
        assert_eq!(output.signed_transaction_data.to_hex(), case.signed_data);
    }
}
