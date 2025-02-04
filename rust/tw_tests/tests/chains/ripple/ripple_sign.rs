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
fn test_ripple_sign_trust_set() {
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

#[test]
fn test_ripple_sign_trust_set_with_currency_amount() {
    let private_key = "574e99f7946cfa2a6ca9368ca72fd37e42583cddb9ecc746aa4cb194ef4b2480"
        .decode_hex()
        .unwrap();

    let trust_set = Proto::OperationTrustSet {
        limit_amount: Some(Proto::CurrencyAmount {
            currency: "524C555344000000000000000000000000000000".into(),
            value: "1000000000".into(),
            issuer: "rMxCKbEDwqr76QuheSUMdEGf4B9xJ8m5De".into(),
        }),
        ..Proto::OperationTrustSet::default()
    };
    let input = Proto::SigningInput {
        fee: 500,
        sequence: 93_674_950,
        last_ledger_sequence: 187_349_950,
        account: "rDgEGKXWkHHr1HYq2ETnNAs9MdV4R8Gyt".into(),
        private_key: private_key.into(),
        operation_oneof: OperationType::op_trust_set(trust_set),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::XRP, input);
    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);

    // https://livenet.xrpl.org/transactions/31ABD41ECAD459BCD008DBA4377047413AEE7A965517DB240016B66A3F4A97E1
    assert_eq!(
        output.encoded.to_hex(),
        "12001422000000002405955dc6201b0b2abbbe63d6c38d7ea4c68000524c555344000000000000000000000000000000e5e961c6a025c9404aa7b662dd1df975be75d13e6840000000000001f47321039c77e9329017ced5f8673ebafcd29687a1fff181140c030062fa77865688fc5d74473045022100aa5f7ffc2e11008a3fe98173c66360937cd3a72cb0951aa1b46ba32675c36b2d02206bc02de3a609e5c4b9e1510a6431a7d7efc0fba4ab9586d6595b86047e46bac281140265c09d122fab2a261a80ee59f1f4cd8fba8cf8"
    );
}

#[test]
fn test_ripple_sign_token_payment() {
    let private_key = "4ba5fd2ebf0f5d7e579b3c354c263ebb39cda4093845125786a280301af14e21"
        .decode_hex()
        .unwrap();

    let token_payment = Proto::OperationPayment {
        amount_oneof: AmountType::currency_amount(Proto::CurrencyAmount {
            currency: "USD".into(),
            value: "10".into(),
            issuer: "rf1BiGeXwwQoi8Z2ueFYTEXSwuJYfV2Jpn".into(),
        }),
        destination: "rU893viamSnsfP3zjzM2KPxjqZjXSXK6VF".into(),
        ..Proto::OperationPayment::default()
    };
    let input = Proto::SigningInput {
        fee: 10,
        sequence: 32_268_645,
        last_ledger_sequence: 32_268_666,
        account: "raPAA61ca99bdwNiZs5JJukR5rvkHWvkBX".into(),
        private_key: private_key.into(),
        operation_oneof: OperationType::op_payment(token_payment),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::XRP, input);
    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);

    // https://testnet.xrpl.org/transactions/8F7820892294598B58CFA2E1101D15ED98C179B25A2BA6DAEB4F5B727CB00D4E
    assert_eq!(
        output.encoded.to_hex(),
        "12000022000000002401ec6165201b01ec617a61d4c38d7ea4c6800000000000000000000000000055534400000000004b4e9c06f24296074f7bc48f92a97916c6dc5ea968400000000000000a7321020652a477b0cca8b74d6e68a6a386a836b226101617481b95180eaffbe841b3227446304402203e925caeb05006afb135254e9ae4e46de2019db6c6f68614ef969885063a777602206af110fc29775256fcad8b14974c6a838141d82193192d3b57324fe1079afa1781143b2fa4f36553e5b7a4f54ff9e6883e44b4b0dbb383148132e4e20aecf29090ac428a9c43f230a829220d"
    );
}

#[test]
fn test_ripple_sign_token_payment_1() {
    let private_key = "4041882ce8c2ceea6f4cfe1a067b927c1e1eb2f5eb025eaf2f429479a7ec3738"
        .decode_hex()
        .unwrap();

    let token_payment = Proto::OperationPayment {
        amount_oneof: AmountType::currency_amount(Proto::CurrencyAmount {
            currency: "USD".into(),
            value: "29.3e-1".into(),
            issuer: "rf1BiGeXwwQoi8Z2ueFYTEXSwuJYfV2Jpn".into(),
        }),
        destination: "rU893viamSnsfP3zjzM2KPxjqZjXSXK6VF".into(),
        ..Proto::OperationPayment::default()
    };
    let input = Proto::SigningInput {
        fee: 10,
        sequence: 32_268_768,
        last_ledger_sequence: 32_268_789,
        account: "raJe5XVt99649qn5Pg7cKdmgEYdN3d4Mky".into(),
        private_key: private_key.into(),
        operation_oneof: OperationType::op_payment(token_payment),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::XRP, input);
    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);

    // https://testnet.xrpl.org/transactions/14606DAAFA54DB29B738000DFC133312B341FFC1D22D57AE0C8D54C9C56E19D8
    assert_eq!(
        output.encoded.to_hex(),
        "12000022000000002401ec61e0201b01ec61f561d48a68d1c931200000000000000000000000000055534400000000004b4e9c06f24296074f7bc48f92a97916c6dc5ea968400000000000000a73210348c331ab218ba964150490c83875b06ccad2100b1f5707f296764712738cf1ca74473045022100a938783258d33e2e3e6099d1ab68fd85c3fd21adfa00e136a67bed8fddec6c9a02206cc6784c1f212f19dc939207643d361ceaa8334eb366722cf33b24dc7669dd7a81143a2f2f189d05abb8519cc9dee0e2dbc6fa53924183148132e4e20aecf29090ac428a9c43f230a829220d"
    );
}

#[test]
fn test_ripple_sign_token_payment_with_currency_amount() {
    let private_key = "574e99f7946cfa2a6ca9368ca72fd37e42583cddb9ecc746aa4cb194ef4b2480"
        .decode_hex()
        .unwrap();

    let token_payment = Proto::OperationPayment {
        amount_oneof: AmountType::currency_amount(Proto::CurrencyAmount {
            currency: "524C555344000000000000000000000000000000".into(),
            value: "1".into(),
            issuer: "rMxCKbEDwqr76QuheSUMdEGf4B9xJ8m5De".into(),
        }),
        destination: "r4oPb529jpRA1tVTDARmBuZPYB2CJjKFac".into(),
        ..Proto::OperationPayment::default()
    };
    let input = Proto::SigningInput {
        fee: 12,
        sequence: 93_674_951,
        last_ledger_sequence: 187_349_950,
        account: "rDgEGKXWkHHr1HYq2ETnNAs9MdV4R8Gyt".into(),
        private_key: private_key.into(),
        operation_oneof: OperationType::op_payment(token_payment),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::XRP, input);
    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);

    // https://livenet.xrpl.org/transactions/6A1229450BB795E450C4AFAA7B72B58962621C0B8760372634796B3941718BFB
    assert_eq!(
        output.encoded.to_hex(),
        "12000022000000002405955dc7201b0b2abbbe61d4838d7ea4c68000524c555344000000000000000000000000000000e5e961c6a025c9404aa7b662dd1df975be75d13e68400000000000000c7321039c77e9329017ced5f8673ebafcd29687a1fff181140c030062fa77865688fc5d744630440220552e90f417c2cabe39368bb45cf7495ba6ebe395f259a6509c9f3a7296e76a0d02201b37dae0c4c77fa70a451cd4a61c10575c8b052c282c082a32c229e7624a05e381140265c09d122fab2a261a80ee59f1f4cd8fba8cf88314ef20a3d93b00cc729eec11a3058d3d1feb4465e0"
    );
}
