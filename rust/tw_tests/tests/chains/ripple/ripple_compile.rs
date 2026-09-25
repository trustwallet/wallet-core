// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::test_utils::sign_utils::{CompilerHelper, PreImageHelper};
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::ToHex;
use tw_hash::H256;
use tw_keypair::ecdsa::secp256k1;
use tw_keypair::traits::SigningKeyTrait;
use tw_misc::traits::ToBytesVec;
use tw_proto::Common::Proto::SigningError;
use tw_proto::Ripple::Proto;
use tw_proto::Ripple::Proto::mod_OperationPayment::OneOfamount_oneof as AmountType;
use tw_proto::Ripple::Proto::mod_SigningInput::OneOfoperation_oneof as OperationType;
use tw_proto::TxCompiler::Proto as CompilerProto;

#[test]
fn test_ripple_compile() {
    let private_key = secp256k1::PrivateKey::try_from(
        "acf1bbf6264e699da0cc65d17ac03fcca6ded1522d19529df7762db46097ff9f",
    )
    .unwrap();
    let public_key = private_key.public();

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
        public_key: public_key.compressed().to_vec().into(),
        operation_oneof: OperationType::op_payment(payment),
        ..Proto::SigningInput::default()
    };

    // Step 2: Obtain preimage hash
    let mut pre_imager = PreImageHelper::<CompilerProto::PreSigningOutput>::default();
    let preimage_output = pre_imager.pre_image_hashes(CoinType::XRP, &input);

    assert_eq!(preimage_output.error, SigningError::OK);
    assert_eq!(
        preimage_output.data.to_hex(),
        "535458001200002200000000240482b3a6201b0482b4ad6140000000000f424068400000000000000a7321027efc5f15071d2ae5e73ee09a0c17456c5d9170a41d67e3297c554829199be80b8114aa000c09c692ef1f82787e51e22833149941ea2083149232ef60695add51f0f84534cc4084e4fdfc698e"
    );
    assert_eq!(
        preimage_output.data_hash.to_hex(),
        "86ef78df7a4aad29e6b3730f7965c1bd5ccd2439426cb738d7c494a64cfaf4af"
    );

    // Step 3: Compile transaction info

    // Simulate signature, normally obtained from signature server.
    let data_hash = H256::try_from(preimage_output.data_hash.as_ref()).unwrap();
    let signature = private_key.sign(data_hash).unwrap();

    let mut compiler = CompilerHelper::<Proto::SigningOutput>::default();
    let output = compiler.compile(
        CoinType::XRP,
        &input,
        vec![signature.to_vec()],
        vec![public_key.compressed().to_vec()],
    );

    assert_eq!(output.error, SigningError::OK);
    assert_eq!(
        output.encoded.to_hex(),
        "1200002200000000240482b3a6201b0482b4ad6140000000000f424068400000000000000a7321027efc5f15071d2ae5e73ee09a0c17456c5d9170a41d67e3297c554829199be80b74473045022100e1c746c3aeebc8278c627ee4c2ce5cae97e3856292c7fe5388f803920230a37b02207d2eccb76cd35dd379d6b24c2cabd786e62d34a564cf083e863176109c5b6bb48114aa000c09c692ef1f82787e51e22833149941ea2083149232ef60695add51f0f84534cc4084e4fdfc698e"
    );
}
