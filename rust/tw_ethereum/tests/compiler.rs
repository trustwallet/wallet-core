// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use std::borrow::Cow;
use tw_coin_entry::coin_entry_ext::CoinEntryExt;
use tw_coin_entry::modules::input_builder::BuildSigningInputArgs;
use tw_coin_entry::test_helpers::dummy_context::DummyCoinContext;
use tw_ethereum::entry::EthereumEntry;
use tw_number::U256;
use tw_proto::deserialize;
use tw_proto::Ethereum::Proto;

#[test]
fn test_external_signature_sign() {
    let args = BuildSigningInputArgs {
        from: "0x9d8A62f656a8d1615C1294fd71e9CFb3E4855A4F".to_string(),
        to: "0x3535353535353535353535353535353535353535".to_string(),
        amount: "1000000000000000000".to_string(),
        asset: "ETH".to_string(),
        memo: "".to_string(),
        chain_id: "".to_string(),
    };
    let res = EthereumEntry
        .build_signing_input(&DummyCoinContext, args.clone())
        .expect("!build_signing_input")
        .expect("'build_signing_input' should return something");
    let input: Proto::SigningInput = deserialize(res.as_slice()).unwrap();

    let transfer = Proto::mod_Transaction::Transfer {
        amount: U256::encode_be_compact(1_000_000_000_000_000_000),
        data: Cow::default(),
    };
    let expected = Proto::SigningInput {
        chain_id: U256::encode_be_compact(1),
        to_address: Cow::from(args.to),
        transaction: Some(Proto::Transaction {
            transaction_oneof: Proto::mod_Transaction::OneOftransaction_oneof::transfer(transfer),
        }),
        ..Proto::SigningInput::default()
    };
    assert_eq!(input, expected);

    // TODO continue the test.
}
