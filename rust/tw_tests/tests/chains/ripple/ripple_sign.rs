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

const SELL_NFTOKEN_FLAG: u64 = 0x00000001;

#[test]
fn test_ripple_sign_xrp_payment_0() {
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
fn test_ripple_sign_xrp_payment_1() {
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
fn test_ripple_sign_xrp_payment_with_source_tag() {
    let private_key = "6da2485443b6856cef6414d45d880434371522cdceb5baf7bd7114e135d71424"
        .decode_hex()
        .unwrap();

    let payment = Proto::OperationPayment {
        amount_oneof: AmountType::amount(20000),
        destination: "rp1ygcibUauhqdTrzo7y3QEJ3WL2sYnSpC".into(),
        ..Proto::OperationPayment::default()
    };
    let input = Proto::SigningInput {
        fee: 12,
        sequence: 93_933_582,
        last_ledger_sequence: 93_938_147,
        account: "rUFSa2D2JYGn5AiRZiQxSupZxV5KcHECG".into(),
        private_key: private_key.into(),
        operation_oneof: OperationType::op_payment(payment),
        // `First Ledger` source tag.
        source_tag: 74_920_348,
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::XRP, input);
    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);

    // https://xrpscan.com/tx/7ED44A908AD150EDEB3D022F3BF1B69A8C06999E143A914E0F3E7D59299693F1
    assert_eq!(
        output.encoded.to_hex(),
        "1200002200000000230477319c240599500e201b059961e3614000000000004e2068400000000000000c732103df650aab92e1b0a95cbda6a5a0fc3bfdbe991901e5b1cdfcd238b769cb4934a77446304402204f104f0fcb8f6ab6d2de9051e99e15af285c6946f01ee53cc630b8ff00b70b840220015d1c5e09b398d830b1255358b4b504628628f36c4b82825442456d8fe3d1b98114023c2b9f15b95198d270b1bf92a4700c40272ca48314149c3d92a8c04140f028ebdb6fac746824384c94"
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
fn test_ripple_sign_token_payment_0() {
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

#[test]
fn test_ripple_sign_escrow_create_0() {
    let private_key = "a3cf20a85b25be4c955f0814718cc7a02eae9195159bd72ede5dd5c4e60d22c4"
        .decode_hex()
        .unwrap();

    let escrow_create = Proto::OperationEscrowCreate {
        amount: 21_300,
        destination: "rEeSXUWEYyEADhDHvi3mtahkFVn7dYNH2G".into(),
        destination_tag: 67,
        cancel_after: 755_015_907,
        finish_after: 755_015_897,
        ..Proto::OperationEscrowCreate::default()
    };
    let input = Proto::SigningInput {
        fee: 12,
        sequence: 84_363_229,
        last_ledger_sequence: 84_363_920,
        account: "rnXwGtLDXXcV63CnRoNaesSsJCZZkJwo9w".into(),
        private_key: private_key.into(),
        operation_oneof: OperationType::op_escrow_create(escrow_create),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::XRP, input);
    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);

    // https://xrpscan.com/tx/3576E5D413CBDC228D13F281BB66304C1EE9DDEAA5563F1783EDB1848266D739
    assert_eq!(
        output.encoded.to_hex(),
        "120001220000000024050747dd2e00000043201b05074a9020242d00a0e320252d00a0d961400000000000533468400000000000000c7321029b557f4db390d68d39d3457204c225d4a68ed86854567a1da99d3e2cd640717374473045022100e62d5005401f1d2b1d9eaa42e0fdbb8b8a433d0cfe71455e782882aa6ab0656f02207b589489b4f344e87a956382e5ede6a55fbfc7e38701364c1fe7d056e9a3253a81143194b932f389b95922fba31662f3c8a606fedfd68314a0a67483ad4d51b2524eb304c0fcef6b2025b865"
    );
}

#[test]
fn test_ripple_sign_escrow_create_1() {
    let private_key = "f157cf7951908b9a2b28d6c5817a3212c3971d8c05a1e964bbafaa5ad7529cb0"
        .decode_hex()
        .unwrap();

    let escrow_create = Proto::OperationEscrowCreate {
        amount: 345_941_506,
        destination: "rNS1tYfynXoKC3eX52gvVnSyU9mqWXvCgh".into(),
        destination_tag: 2_467,
        cancel_after: 0,
        finish_after: 750_095_491,
        ..Proto::OperationEscrowCreate::default()
    };
    let input = Proto::SigningInput {
        fee: 12,
        sequence: 41_874_843,
        last_ledger_sequence: 41_874_865,
        account: "rL6iE1bbAHekMavpGot6gRxqkQKm6yfoQ6".into(),
        private_key: private_key.into(),
        operation_oneof: OperationType::op_escrow_create(escrow_create),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::XRP, input);
    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);

    // https://testnet.xrpl.org/transactions/3F581927C742D5FAE65FB0759D0F04EF3B64B4A087911B07975816ECCB59915B
    assert_eq!(
        output.encoded.to_hex(),
        "120001220000000024027ef59b2e000009a3201b027ef5b120252cb58c836140000000149ea60268400000000000000c7321021846a49ea81238d03dff5a89a9da82eb06b23a276af9a06b45d4aba39713311f744630440220176318f29d2b815f599072230690397f91262c1f801bafada9820d89c719359c0220756eb74d815e20e86f6748c6821d3204f93221a95b4481a572a10530f5776c698114d8242542e6108fccf75a7f5bb0059cfae6d155378314937e838cb1033342c72acfae58fe2e3875ce7693"
    );
}

/// `EscrowCreate` with cancel after > 0x7fffffff.
#[test]
fn test_ripple_sign_escrow_create_2() {
    let private_key = "8b488ed9b9875174140a97cad53cd8c652789889612f94a9006b7ced18a1c6ef"
        .decode_hex()
        .unwrap();

    let escrow_create = Proto::OperationEscrowCreate {
        amount: 88_941_506,
        destination: "rfC73DuBhDqF3Zw1K3uxaQNCkwT8pPKyf5".into(),
        destination_tag: 0,
        cancel_after: 2_147_483_648,
        finish_after: 750_097_108,
        ..Proto::OperationEscrowCreate::default()
    };
    let input = Proto::SigningInput {
        fee: 12,
        sequence: 41_875_372,
        last_ledger_sequence: 41_875_394,
        account: "rEE4PdEYhEikJ1bvQjdE9HdjBV8yp8FsGC".into(),
        private_key: private_key.into(),
        operation_oneof: OperationType::op_escrow_create(escrow_create),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::XRP, input);
    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);

    assert_eq!(
        output.encoded.to_hex(),
        "120001220000000024027ef7ac201b027ef7c220248000000020252cb592d46140000000054d23c268400000000000000c73210211cfeb81bc410e694e98c6a0f17c9c89d85e2b89bc17d2699063c0920217ab0574463044022038d27cd842422d8ee72d5cab11734ce128aef21d7cec17654d21c27d0556d23e0220059f913178a4c65a5d3289896876989e0fcaf3add9769459fb232ab94398368a81149c4970a2b763b9484e3b65d67f3d9b7b1698cb7f83144917342345fbe5cef1e22d3f1353fc468bf696ac"
    );
}

#[test]
fn test_ripple_sign_escrow_create_with_condition_0() {
    let private_key = "a3cf20a85b25be4c955f0814718cc7a02eae9195159bd72ede5dd5c4e60d22c4"
        .decode_hex()
        .unwrap();

    let escrow_create = Proto::OperationEscrowCreate {
        amount: 37_000,
        destination: "rEeSXUWEYyEADhDHvi3mtahkFVn7dYNH2G".into(),
        destination_tag: 0,
        cancel_after: 755_014_300,
        finish_after: 0,
        condition: "a0258020c26add2db64dd6d5700a5e2721c1e908d599901627b8dc82f25b3e035ec4004b810120"
            .into(),
        ..Proto::OperationEscrowCreate::default()
    };
    let input = Proto::SigningInput {
        fee: 12,
        sequence: 84_363_226,
        last_ledger_sequence: 84_363_509,
        account: "rnXwGtLDXXcV63CnRoNaesSsJCZZkJwo9w".into(),
        private_key: private_key.into(),
        operation_oneof: OperationType::op_escrow_create(escrow_create),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::XRP, input);
    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);

    // https://xrpscan.com/tx/77E01FD30A788BFC96F28960F099D4076255252F33FCD31EEBBCBB61E3318544
    assert_eq!(
        output.encoded.to_hex(),
        "120001220000000024050747da201b050748f520242d009a9c61400000000000908868400000000000000c7321029b557f4db390d68d39d3457204c225d4a68ed86854567a1da99d3e2cd6407173744630440220307f4c91e91166db1428eb1ab8f65a84bd9b89542ed844045ffd040f5e13d12b022061120350b9685381e9941c7ec54ce154ca0ef0d01f630aeb3e78dd9fd087ff80701127a0258020c26add2db64dd6d5700a5e2721c1e908d599901627b8dc82f25b3e035ec4004b81012081143194b932f389b95922fba31662f3c8a606fedfd68314a0a67483ad4d51b2524eb304c0fcef6b2025b865"
    );
}

#[test]
fn test_ripple_sign_escrow_create_with_condition_1() {
    let private_key = "a3cf20a85b25be4c955f0814718cc7a02eae9195159bd72ede5dd5c4e60d22c4"
        .decode_hex()
        .unwrap();

    let escrow_create = Proto::OperationEscrowCreate {
        amount: 30_941_506,
        destination: "rEeSXUWEYyEADhDHvi3mtahkFVn7dYNH2G".into(),
        destination_tag: 0,
        cancel_after: 750_090_371,
        finish_after: 0,
        condition: "a0258020b3dda5c580919ce0fd6acdf013c337461951946e54b41446467961568cdd9e7b810120"
            .into(),
        ..Proto::OperationEscrowCreate::default()
    };
    let input = Proto::SigningInput {
        fee: 12,
        sequence: 41_872_968,
        last_ledger_sequence: 41_873_012,
        account: "rnXwGtLDXXcV63CnRoNaesSsJCZZkJwo9w".into(),
        private_key: private_key.into(),
        operation_oneof: OperationType::op_escrow_create(escrow_create),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::XRP, input);
    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);

    // https://testnet.xrpl.org/transactions/A8EE35E26CD09E3D6A415DDEFEA6723CA5AFEB1838C5FE06835937FA49DEF3A0
    assert_eq!(
        output.encoded.to_hex(),
        "120001220000000024027eee48201b027eee7420242cb57883614000000001d8214268400000000000000c7321029b557f4db390d68d39d3457204c225d4a68ed86854567a1da99d3e2cd640717374473045022100931b3a6634471fa22f709417d7280b76564a8f3a700cf51a50a2c1b1e0162d570220217c0f2e3922e9bc5b2175712c0e244f2f05bf42ccd1e632b06476f66704203f701127a0258020b3dda5c580919ce0fd6acdf013c337461951946e54b41446467961568cdd9e7b81012081143194b932f389b95922fba31662f3c8a606fedfd68314a0a67483ad4d51b2524eb304c0fcef6b2025b865"
    );
}

#[test]
fn test_ripple_sign_escrow_create_with_condition_2() {
    let private_key = "be60f33cbeb2b5ee688dcb1e93986f2522d8ad76b3c48398bf2be02a6699e781"
        .decode_hex()
        .unwrap();

    let escrow_create = Proto::OperationEscrowCreate {
        amount: 28_941_506,
        destination: "r9YD31TAtbS8EPwEt2gzGDjsaMDyV1s5QE".into(),
        destination_tag: 2_467,
        cancel_after: 750_094_604,
        finish_after: 0,
        condition: "a0258020ffecf1ae6182f10efebe0c0896cd6b044df7b27d33b05030033ef63d47e2b250810120"
            .into(),
        ..Proto::OperationEscrowCreate::default()
    };
    let input = Proto::SigningInput {
        fee: 12,
        sequence: 41_874_370,
        last_ledger_sequence: 41_874_392,
        account: "rpLGh11T9B6b4UjAU1WRCJowLw8uk7vS44".into(),
        private_key: private_key.into(),
        operation_oneof: OperationType::op_escrow_create(escrow_create),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::XRP, input);
    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);

    // https://testnet.xrpl.org/transactions/25AE9F7CBC9944B140A4BE338A47DD8C2C29313B44694533D9D47CD758A60A8F
    assert_eq!(
        output.encoded.to_hex(),
        "120001220000000024027ef3c22e000009a3201b027ef3d820242cb5890c614000000001b99cc268400000000000000c7321035e6cd73289f9b1a796fba572f7a2732aae23b2a9ea6b0ec239d5b9feb388774074473045022100c4bb3b65acd5d30aa8f85ea2a0d2c0e18d2025a005a827722059a9a636eb1bca02207d73b4a64d679e605a6cb31881d7ea3642c1e54e3bf38d13d0dd4219c27d1420701127a0258020ffecf1ae6182f10efebe0c0896cd6b044df7b27d33b05030033ef63d47e2b25081012081140e9c9b31b826671aaa387555cdeccab82a78402083145da8080d21fecf98f24ea2223482e5d24f107799"
    );
}

#[test]
fn test_ripple_sign_escrow_cancel_0() {
    let private_key = "a3cf20a85b25be4c955f0814718cc7a02eae9195159bd72ede5dd5c4e60d22c4"
        .decode_hex()
        .unwrap();

    let escrow_cancel = Proto::OperationEscrowCancel {
        owner: "rnXwGtLDXXcV63CnRoNaesSsJCZZkJwo9w".into(),
        offer_sequence: 84_363_227,
    };
    let input = Proto::SigningInput {
        fee: 12,
        sequence: 84_363_228,
        last_ledger_sequence: 84_363_740,
        account: "rnXwGtLDXXcV63CnRoNaesSsJCZZkJwo9w".into(),
        private_key: private_key.into(),
        operation_oneof: OperationType::op_escrow_cancel(escrow_cancel),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::XRP, input);
    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);

    // https://xrpscan.com/tx/949B3C3D8B4528C95D07654BBA10B08ABA65FFD339E31706BC93CB0824427F97
    assert_eq!(
        output.encoded.to_hex(),
        "120004220000000024050747dc2019050747db201b050749dc68400000000000000c7321029b557f4db390d68d39d3457204c225d4a68ed86854567a1da99d3e2cd64071737446304402202c0416934dbf3a0c42d0b0da9e893cec69e42c81f41424f4a388c3ba8862e65a02201781e22ef85b251902e918f6d923769993757a79b865a62ecdebc1a015368f1f81143194b932f389b95922fba31662f3c8a606fedfd682143194b932f389b95922fba31662f3c8a606fedfd6"
    );
}

#[test]
fn test_ripple_sign_escrow_cancel_1() {
    let private_key = "bf9810cc4f7cc5e6dea8a0c29f3389d9d511e795d467b402a870e71d93243705"
        .decode_hex()
        .unwrap();

    let escrow_cancel = Proto::OperationEscrowCancel {
        owner: "rE16pf2ZQUZBDLKAyTFF9Q1b3YY1nc7v2J".into(),
        offer_sequence: 41_875_229,
    };
    let input = Proto::SigningInput {
        fee: 12,
        sequence: 41_875_230,
        last_ledger_sequence: 41_875_263,
        account: "rE16pf2ZQUZBDLKAyTFF9Q1b3YY1nc7v2J".into(),
        private_key: private_key.into(),
        operation_oneof: OperationType::op_escrow_cancel(escrow_cancel),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::XRP, input);
    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);

    // https://testnet.xrpl.org/transactions/5B0F8766FFBDE7D3A9ACAA63361BF00FE0739DC8718507776EB2C1AD980BC965
    assert_eq!(
        output.encoded.to_hex(),
        "120004220000000024027ef71e2019027ef71d201b027ef73f68400000000000000c73210277314966f72e9520199faa3941bd45b89e444f7eabf203e805527f880de80b8674473045022100ec04d05db5725ce154a511f93056fde0b825b7e0bb4a59b4d4264a008eafdcfe0220676f30f916c6ea0644c11c0bcafcfa8209a083041742d672a726a5c8d99230ea8114a327f724d30f2732f78a4ec6744db298e827ba2b8214a327f724d30f2732f78a4ec6744db298e827ba2b"
    );
}

#[test]
fn test_ripple_sign_escrow_finish_with_condition_0() {
    let private_key = "f60edca8e4bb25f9916017c9c7fe93e633b800550f86cf305a2e99271d7cede5"
        .decode_hex()
        .unwrap();

    let escrow_finish = Proto::OperationEscrowFinish {
        owner: "rnXwGtLDXXcV63CnRoNaesSsJCZZkJwo9w".into(),
        offer_sequence: 84_363_226,
        condition: "a0258020c26add2db64dd6d5700a5e2721c1e908d599901627b8dc82f25b3e035ec4004b810120"
            .into(),
        fulfillment: "a02280205d729ac237c4c7976403817b6409be7190efbfad49af2cf974b9582a854e8794"
            .into(),
        ..Proto::OperationEscrowFinish::default()
    };
    let input = Proto::SigningInput {
        fee: 423,
        sequence: 84_363_473,
        last_ledger_sequence: 84_363_511,
        account: "rEeSXUWEYyEADhDHvi3mtahkFVn7dYNH2G".into(),
        private_key: private_key.into(),
        operation_oneof: OperationType::op_escrow_finish(escrow_finish),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::XRP, input);
    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);

    // https://xrpscan.com/tx/7E9AC2C8286E3EC0410784920A0F8048C79257EDF19B392F98A31F62E3CF4FAD
    assert_eq!(
        output.encoded.to_hex(),
        "120002220000000024050748d12019050747da201b050748f76840000000000001a77321025a4c754a3f836ebe18520e7d3861c6e38a4adfe466465d5db6cbb2d745d27ee574473045022100f06a6ac18efc1280f9d26cbb47c31f7ecd72ed200f9d05c6c762ffcf18b53534022049c6bb4ac8e79c478939a55dd1cb571d56ec929e5200332e410fd69c0fe1ef48701024a02280205d729ac237c4c7976403817b6409be7190efbfad49af2cf974b9582a854e8794701127a0258020c26add2db64dd6d5700a5e2721c1e908d599901627b8dc82f25b3e035ec4004b8101208114a0a67483ad4d51b2524eb304c0fcef6b2025b86582143194b932f389b95922fba31662f3c8a606fedfd6"
    );
}

#[test]
fn test_ripple_sign_escrow_finish_with_condition_1() {
    let private_key = "4bae9219cf9c58e5db0c395900085f07fc06709e1b2223ccac40191fbcbdab2a"
        .decode_hex()
        .unwrap();

    let escrow_finish = Proto::OperationEscrowFinish {
        owner: "rpLGh11T9B6b4UjAU1WRCJowLw8uk7vS44".into(),
        offer_sequence: 41_874_370,
        condition: "a0258020ffecf1ae6182f10efebe0c0896cd6b044df7b27d33b05030033ef63d47e2b250810120"
            .into(),
        fulfillment: "a022802049b9ab20ca85b55d0c12b948ec7c524f843c77be1ef1561a42b7167dce174b7a"
            .into(),
        ..Proto::OperationEscrowFinish::default()
    };
    let input = Proto::SigningInput {
        fee: 423,
        sequence: 41_874_372,
        last_ledger_sequence: 41_874_394,
        account: "r9YD31TAtbS8EPwEt2gzGDjsaMDyV1s5QE".into(),
        private_key: private_key.into(),
        operation_oneof: OperationType::op_escrow_finish(escrow_finish),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::XRP, input);
    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);

    // https://testnet.xrpl.org/transactions/4A49D4AD05FBDC4A354E31C7453829509F59DD2B51CDE560C4350155F5DBFD86
    assert_eq!(
        output.encoded.to_hex(),
        "120002220000000024027ef3c42019027ef3c2201b027ef3da6840000000000001a773210277c5d02c3c774c96017234a532dae12023ac8fb499c5d90a56488900ecc746d07446304402206698c1d296bf1493c97beb64945558724c6c88474cd3e0b90e9dc9e7313ac1970220175fef60c48646be934be28a964af0cc55843fb6e6ef17c886716a03af849f74701024a022802049b9ab20ca85b55d0c12b948ec7c524f843c77be1ef1561a42b7167dce174b7a701127a0258020ffecf1ae6182f10efebe0c0896cd6b044df7b27d33b05030033ef63d47e2b25081012081145da8080d21fecf98f24ea2223482e5d24f10779982140e9c9b31b826671aaa387555cdeccab82a784020"
    );
}

#[test]
fn test_ripple_sign_nftoken_burn() {
    let private_key = "7c2ea5c7b1fd7dfc62d879918b7fc779cdff6bf6391d02ec99854297e916318e"
        .decode_hex()
        .unwrap();

    let burn = Proto::OperationNFTokenBurn {
        nftoken_id: "000b013a95f14b0044f78a264e41713c64b5f89242540ee208c3098e00000d65".into(),
    };
    let input = Proto::SigningInput {
        fee: 10,
        sequence: 22_858_395,
        last_ledger_sequence: 22_858_416,
        account: "rhR1mTXkg4iSGhz7zsEKBLbV3MkopivPVF".into(),
        private_key: private_key.into(),
        operation_oneof: OperationType::op_nftoken_burn(burn),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::XRP, input);
    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);

    // https://devnet.xrpl.org/transactions/37DA90BE3C30016B3A2C3D47D9677278A3F6D4141B318793CE6AA467A6530E2D
    assert_eq!(
        output.encoded.to_hex(),
        "12001a220000000024015cca9b201b015ccab05a000b013a95f14b0044f78a264e41713c64b5f89242540ee208c3098e00000d6568400000000000000a73210254fc876043109af1ff11b832320be4436ef51dcc344da5970c9b6c6d1fbcddcf744730450221008b4d437bc92aa4643b275b17c0f88a1bef2c1c160ece5faf93b03e2d31b8278602207640e7e35426352deaafecf61e2b401a4ea1fc645839280370a72fa3c41aea7d8114259cbcf9635360bc302f27d0ce72c18d4dbe9c8d"
    );
}

#[test]
fn test_ripple_sign_nftoken_create_offer() {
    let private_key = "1963884da4a4da79ad7681d106b2c55fb652c68ca7b288dd12bb86cd40b9d940"
        .decode_hex()
        .unwrap();

    let create_offer = Proto::OperationNFTokenCreateOffer {
        nftoken_id: "000b013a95f14b0044f78a264e41713c64b5f89242540ee208c3098e00000d65".into(),
        destination: "rDxTa8vhigDUCq9nmZY8jAkFne5XrcYbxG".into(),
    };
    let input = Proto::SigningInput {
        fee: 10,
        sequence: 22_857_522,
        last_ledger_sequence: 22_857_543,
        account: "rJdxtrVoL3Tak74EzN8SdMxFF6RP9smjJJ".into(),
        flags: SELL_NFTOKEN_FLAG,
        private_key: private_key.into(),
        operation_oneof: OperationType::op_nftoken_create_offer(create_offer),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::XRP, input);
    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);

    // https://devnet.xrpl.org/transactions/E61D66E261DB89CEAAB4F54ECF792B329296CB524E8B40EA99D27CF5E16DD27D
    assert_eq!(
        output.encoded.to_hex(),
        "12001b220000000124015cc732201b015cc7475a000b013a95f14b0044f78a264e41713c64b5f89242540ee208c3098e00000d6561400000000000000068400000000000000a7321022707066e4f8b87b749ef802338be064065dc978f0ea52ea9c8c8ea0a6145571974473045022100a148140469b8e9e2f9aa43631f3101e532d161d49a05e739cd3494ea208bd657022029a9752df3fc0d23b8fdb46d2274e69ab198ce6f373aeb7cdd0d81ab05aff6f48114c177c23ed1f5d175f42fd7970ece74ac18d61c4d83148e1e2ca343165bf30e96abead961f7a34510ad93"
    );
}

#[test]
fn test_ripple_sign_nftoken_accept_offer() {
    let private_key = "3c01b3458d2b2a4b86a5699d11682d791b5c3136692c5594f7a8ca7f3967e7ae"
        .decode_hex()
        .unwrap();

    let accept_offer = Proto::OperationNFTokenAcceptOffer {
        sell_offer: "000b013a95f14b0044f78a264e41713c64b5f89242540ee208c3098e00000d65".into(),
    };
    let input = Proto::SigningInput {
        fee: 10,
        sequence: 22_857_743,
        last_ledger_sequence: 22_857_764,
        account: "rPa2KsEuSuZnmjosds99nhgsoiKtw85j6Z".into(),
        private_key: private_key.into(),
        operation_oneof: OperationType::op_nftoken_accept_offer(accept_offer),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::XRP, input);
    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);

    // https://devnet.xrpl.org/transactions/6BB00A7BABB8797D60E3AB0E52DB64562524D014833977D87B04CA9FA3F56AD7
    assert_eq!(
        output.encoded.to_hex(),
        "12001d220000000024015cc80f201b015cc824501d000b013a95f14b0044f78a264e41713c64b5f89242540ee208c3098e00000d6568400000000000000a73210331c298cb86428b9126bd4af6a952870cfe3fe5065dc093cf97f3edbb27e9dd15744630440220797922caaa593c4e91fa6b63a38c92ef9f5e2183128918dda166f4292882e137022057702b668d7463ef1d01dad5ee6633bd36f0aa358dacc90d6b68d248672a400f8114f260a758132d3ed27e52d7f55ef0481606f090d4"
    );
}

#[test]
fn test_ripple_sign_nftoken_cancel_offer() {
    let private_key = "3e50cc102d8c96abd55f047a536b6425154514ba8abdf5f09335a7c644176c5d"
        .decode_hex()
        .unwrap();

    let cancel_offer = Proto::OperationNFTokenCancelOffer {
        token_offers: vec![
            "000b013a95f14b0044f78a264e41713c64b5f89242540ee208c3098e00000d65".into(),
        ],
    };
    let input = Proto::SigningInput {
        fee: 10,
        sequence: 22_857_838,
        last_ledger_sequence: 22_857_859,
        account: "rPqczdU9bzow966hQKQejsXrMJspM7G4CC".into(),
        private_key: private_key.into(),
        operation_oneof: OperationType::op_nftoken_cancel_offer(cancel_offer),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::XRP, input);
    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);

    // https://devnet.xrpl.org/transactions/CBA148308A0D1561E5E8CDF1F2E8D5562C320C221AC4053AA5F495CEF4B5D5D4
    assert_eq!(
        output.encoded.to_hex(),
        "12001c220000000024015cc86e201b015cc88368400000000000000a7321022250f103fd045edf2e552df2d20aca01a52dc6aedd522d68767f1c744fedb39d74463044022015fff495fc5d61cd71e5815e4d23845ec26f4dc94adb85207feba2c97e19856502207297ec84afc0bb74aa8a20d7254025a82d9b9f177f648845d8c72ee62884ff618114fa84c77f2a5245ef774845d40428d2a6f9603415041320000b013a95f14b0044f78a264e41713c64b5f89242540ee208c3098e00000d65"
    );
}

#[test]
fn test_ripple_sign_raw_json() {
    let private_key = "6da2485443b6856cef6414d45d880434371522cdceb5baf7bd7114e135d71424"
        .decode_hex()
        .unwrap();

    let raw_json = r#"{
        "TransactionType": "Payment",
        "Destination": "rM27yzkCw6WA3T4g1sPaeC1kpxHUhuxRWn",
        "Amount": "100000",
        "Memos": [
            {
                "Memo": {
                    "MemoData": "7b2266726f6d546f6b656e223a22307865656565656565656565656565656565656565656565656565656565656565656565656565656565222c22746f546f6b656e223a225452587c783561763039222c2273656e646572223a2272554653613244324a59476e354169525a6951785375705a7856354b6348454347222c2264657374696e6174696f6e223a22544255437a6763323976796b6b7646614547326d675274784b76614b6536736b7758222c226d696e52657475726e416d6f756e74223a2231333239353831303030303030222c2266726f6d416d6f756e74223a22313030303030227d"
                }
            }
        ]
    }"#;

    let input = Proto::SigningInput {
        fee: 10,
        sequence: 93_933_585,
        last_ledger_sequence: 94_054_671,
        account: "rUFSa2D2JYGn5AiRZiQxSupZxV5KcHECG".into(),
        private_key: private_key.into(),
        raw_json: raw_json.into(),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::XRP, input);
    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);

    // https://devnet.xrpl.org/transactions/D93B0B6983134BC6C2880B27708E8C1E932CB9E6D9E78773AD31B797741944FF
    assert_eq!(output.encoded.to_hex(), "1200002405995011201b059b290f6140000000000186a068400000000000000a732103df650aab92e1b0a95cbda6a5a0fc3bfdbe991901e5b1cdfcd238b769cb4934a7744730450221008a5dba92a63fa82987a9ec3035febd1d5fe802f9a1baf3760090bb117281684e022056e4feca51231f719ca16cbfe370d312704e2cd7b94a28548f7f7886141d8bb28114023c2b9f15b95198d270b1bf92a4700c40272ca48314e1b799e72e5785c6a84af0f9c01424d4256b14aff9ea7dc1287b2266726f6d546f6b656e223a22307865656565656565656565656565656565656565656565656565656565656565656565656565656565222c22746f546f6b656e223a225452587c783561763039222c2273656e646572223a2272554653613244324a59476e354169525a6951785375705a7856354b6348454347222c2264657374696e6174696f6e223a22544255437a6763323976796b6b7646614547326d675274784b76614b6536736b7758222c226d696e52657475726e416d6f756e74223a2231333239353831303030303030222c2266726f6d416d6f756e74223a22313030303030227de1f1");
}
