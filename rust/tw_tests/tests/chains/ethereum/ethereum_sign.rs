// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::borrow::Cow;
use tw_any_coin::ffi::tw_any_signer::tw_any_signer_sign;
use tw_coin_entry::error::prelude::*;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_memory::test_utils::tw_data_helper::TWDataHelper;
use tw_number::U256;
use tw_proto::{deserialize, serialize};

#[test]
fn test_any_signer_sign_eth() {
    use tw_proto::Ethereum::Proto;

    let private = "0x4646464646464646464646464646464646464646464646464646464646464646"
        .decode_hex()
        .unwrap();

    let transfer = Proto::mod_Transaction::Transfer {
        amount: U256::encode_be_compact(1_000_000_000_000_000_000),
        data: Cow::default(),
    };

    let input = Proto::SigningInput {
        chain_id: U256::encode_be_compact(1),
        nonce: U256::encode_be_compact(9),
        gas_price: U256::encode_be_compact(20_000_000_000),
        gas_limit: U256::encode_be_compact(21_000),
        to_address: "0x3535353535353535353535353535353535353535".into(),
        transaction: Some(Proto::Transaction {
            transaction_oneof: Proto::mod_Transaction::OneOftransaction_oneof::transfer(transfer),
        }),
        private_key: private.into(),
        ..Proto::SigningInput::default()
    };

    let input_data = TWDataHelper::create(serialize(&input).unwrap());

    let output = TWDataHelper::wrap(unsafe {
        tw_any_signer_sign(input_data.ptr(), CoinType::Ethereum as u32)
    })
    .to_vec()
    .expect("!tw_any_signer_sign returned nullptr");

    let output: Proto::SigningOutput = deserialize(&output).unwrap();
    assert_eq!(output.error, SigningErrorType::OK);
    assert!(output.error_message.is_empty());

    let expected = "f86c098504a817c800825208943535353535353535353535353535353535353535880de0b6b3a76400008025a028ef61340bd939bc2195fe537567866003e1a15d3c71ff63e1590620aa636276a067cbe9d8997f761aecb703304b3800ccf555c9f3dc64214b297fb1966a3b6d83";
    assert_eq!(output.encoded.to_hex(), expected);
}
