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

    // https://testnet.xrpl.org/transactions/A202034796F37F38D1D20F2025DECECB1623FC801F041FC694199C0D0E49A739
    assert_eq!(
        output.encoded.to_hex(),
        "12000022000000002401ec5fd8201b01ec5fed61400000000000000a68400000000000000a732103d13e1152965a51a4a9fd9a8b4ea3dd82a4eba6b25fcad5f460a2342bb650333f74463044022037d32835c9394f39b2cfd4eaf5b0a80e0db397ace06630fa2b099ff73e425dbc02205288f780330b7a88a1980fa83c647b5908502ad7de9a44500c08f0750b0d9e8481144c55f5a78067206507580be7bb2686c8460adff983148132e4e20aecf29090ac428a9c43f230a829220d"
    );
}

#[test]
fn test_ripple_sign_xrp_payment_main() {
    let private_key = "acf1bbf6264e699da0cc65d17ac03fcca6ded1522d19529df7762db46097ff9f"
        .decode_hex()
        .unwrap();

    let payment = Proto::OperationPayment {
        amount_oneof: AmountType::amount(1000000),
        destination: "rNLpgsBTCwiaZAnHe2ZViAN1GcXZtYW6rg".into(),
        ..Proto::OperationPayment::default()
    };
    let input = Proto::SigningInput {
        fee: 10,
        sequence: 75_674_534,
        last_ledger_sequence: 75_674_797,
        account: "rGV1v1xw23PHcRn4Km4tF8R2mfh6yTZkcP".into(),
        private_key: private_key.into(),
        operation_oneof: OperationType::op_payment(payment),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::XRP, input);
    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);

    // https://xrpscan.com/tx/4B9D022E8C77D798B7D11C41FDFDCF468F03A5564151C520EECA1E96FF1A1610
    assert_eq!(
        output.encoded.to_hex(),
        "1200002200000000240482b3a6201b0482b4ad6140000000000f424068400000000000000a7321027efc5f15071d2ae5e73ee09a0c17456c5d9170a41d67e3297c554829199be80b74473045022100e1c746c3aeebc8278c627ee4c2ce5cae97e3856292c7fe5388f803920230a37b02207d2eccb76cd35dd379d6b24c2cabd786e62d34a564cf083e863176109c5b6bb48114aa000c09c692ef1f82787e51e22833149941ea2083149232ef60695add51f0f84534cc4084e4fdfc698e"
    );
}

#[test]
fn test_ripple_sign_() {
    let private_key = "8753e78ee2963f301f82e5eeab2754f593fc242ce94273dd2fb0684e3b0f2b91"
        .decode_hex()
        .unwrap();

    let trust_set = Proto::OperationTrustSet {
        limit_amount: Some(Proto::CurrencyAmount {
            currency: "USD".into(),
            value: "10".into(),
            issuer: "rf1BiGeXwwQoi8Z2ueFYTEXSwuJYfV2Jpn".into(),
        }),
        ..Proto::OperationTrustSet::default()
    };
    let input = Proto::SigningInput {
        fee: 10,
        sequence: 32_268_473,
        last_ledger_sequence: 32_268_494,
        account: "rnRkLPni2Q5yMxSqyJSJEkKUfQNFkaAspS".into(),
        private_key: private_key.into(),
        operation_oneof: OperationType::op_trust_set(trust_set),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::XRP, input);
    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);

    // https://testnet.xrpl.org/transactions/31042345374CFF785B3F7E2A3716E3BAB7E2CAA30D40F5E488E67ABA116655B9
    assert_eq!(
        output.encoded.to_hex(),
        "12001422000000002401ec60b9201b01ec60ce63d4c38d7ea4c6800000000000000000000000000055534400000000004b4e9c06f24296074f7bc48f92a97916c6dc5ea968400000000000000a732103dc4a0dae2d550de7cace9c26c1a331a114e3e7efee5577204b476d27e2dc683a7446304402206ebcc7a689845df373dd2566cd3789862d426d9ad4e6a09c2d2772b57e82696a022066b1f217a0f0d834d167613a313f74097423a9ccd11f1ae7f90ffab0d2fc26b58114308ea8e515b64f2e6616a33b42e1bbb9fa00bbd2"
    );
}
