// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::test_utils::sign_utils::AnySignerHelper;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_proto::Common::Proto::SigningError;
use tw_proto::Ripple::Proto;
use tw_proto::Ripple::Proto::mod_OperationPayment::OneOfamount_oneof as AmountType;
use tw_proto::Ripple::Proto::mod_SigningInput::OneOfoperation_oneof as OperationType;

#[test]
fn test_ripple_sign_xrp_payment() {
    let private_key = "a5576c0f63da10e584568c8d134569ff44017b0a249eb70657127ae04f38cc77"
        .decode_hex()
        .unwrap();

    let payment = Proto::OperationPayment {
        amount_oneof: AmountType::amount(10),
        destination: "rU893viamSnsfP3zjzM2KPxjqZjXSXK6VF".into(),
        ..Proto::OperationPayment::default()
    };
    let input = Proto::SigningInput {
        fee: 10,
        sequence: 32_268_248,
        last_ledger_sequence: 32_268_269,
        account: "rfxdLwsZnoespnTDDb1Xhvbc8EFNdztaoq".into(),
        private_key: private_key.into(),
        operation_oneof: OperationType::op_payment(payment),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::XRP, input);
    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);
    assert_eq!(
        output.encoded.to_hex(),
        "12000022000000002401ec5fd8201b01ec5fed61400000000000000a68400000000000000a732103d13e1152965a51a4a9fd9a8b4ea3dd82a4eba6b25fcad5f460a2342bb650333f74463044022037d32835c9394f39b2cfd4eaf5b0a80e0db397ace06630fa2b099ff73e425dbc02205288f780330b7a88a1980fa83c647b5908502ad7de9a44500c08f0750b0d9e8481144c55f5a78067206507580be7bb2686c8460adff983148132e4e20aecf29090ac428a9c43f230a829220d"
    );
}
