// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::ffi::tw_transaction_compiler::{
    tw_transaction_compiler_compile, tw_transaction_compiler_pre_image_hashes,
};
use tw_coin_entry::error::prelude::*;
use tw_cosmos_sdk::test_utils::proto_utils::{make_amount, make_fee, make_message};
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_memory::test_utils::tw_data_helper::TWDataHelper;
use tw_memory::test_utils::tw_data_vector_helper::TWDataVectorHelper;
use tw_proto::Cosmos::Proto;
use tw_proto::Cosmos::Proto::mod_Message::OneOfmessage_oneof as MessageEnum;
use tw_proto::TxCompiler::Proto as CompilerProto;
use tw_proto::{deserialize, serialize};

const NATIVE_INJECTIVE_COIN_TYPE: u32 = 10000060;
const ACCOUNT_88701_PUBLIC_KEY: &str = "04088ac2919987d927368cb2be2ade44cd0ed3616745a9699cae264b3fc5a7c3607d99f441b8340990ee990cb3eaf560f1f0bafe600c7e94a4be8392166984f728";

fn send_tx_input() -> Proto::SigningInput<'static> {
    let send_msg = Proto::mod_Message::Send {
        from_address: "inj1d0jkrsd09c7pule43y3ylrul43lwwcqaky8w8c".into(),
        to_address: "inj1xmpkmxr4as00em23tc2zgmuyy2gr4h3wgcl6vd".into(),
        amounts: vec![make_amount("inj", "10000000000")],
        ..Proto::mod_Message::Send::default()
    };
    Proto::SigningInput {
        account_number: 88701,
        chain_id: "injective-1".into(),
        sequence: 0,
        fee: Some(make_fee(110000, make_amount("inj", "100000000000000"))),
        public_key: ACCOUNT_88701_PUBLIC_KEY.decode_hex().unwrap().into(),
        messages: vec![make_message(MessageEnum::send_coins_message(send_msg))],
        ..Proto::SigningInput::default()
    }
}

#[test]
fn test_compile_native_injective_tx_protobuf() {
    let input = Proto::SigningInput {
        signing_mode: Proto::SigningMode::Protobuf,
        ..send_tx_input()
    };
    let input_data = TWDataHelper::create(serialize(&input).unwrap());

    // Step 1: Obtain preimage hash

    let preimage_output_data = TWDataHelper::wrap(unsafe {
        tw_transaction_compiler_pre_image_hashes(NATIVE_INJECTIVE_COIN_TYPE, input_data.ptr())
    })
    .to_vec()
    .expect("!tw_any_signer_sign returned nullptr");

    let preimage_output: CompilerProto::PreSigningOutput =
        deserialize(&preimage_output_data).unwrap();
    assert_eq!(preimage_output.error, SigningErrorType::OK);
    assert!(preimage_output.error_message.is_empty());

    let expected_preimage = "0a8f010a8c010a1c2f636f736d6f732e62616e6b2e763162657461312e4d736753656e64126c0a2a696e6a3164306a6b7273643039633770756c6534337933796c72756c34336c77776371616b7938773863122a696e6a31786d706b6d78723461733030656d32337463327a676d7579793267723468337767636c3676641a120a03696e6a120b3130303030303030303030129c010a7c0a740a2d2f696e6a6563746976652e63727970746f2e763162657461312e657468736563703235366b312e5075624b657912430a4104088ac2919987d927368cb2be2ade44cd0ed3616745a9699cae264b3fc5a7c3607d99f441b8340990ee990cb3eaf560f1f0bafe600c7e94a4be8392166984f72812040a020801121c0a160a03696e6a120f31303030303030303030303030303010b0db061a0b696e6a6563746976652d3120fdb405";
    assert_eq!(preimage_output.data.to_hex(), expected_preimage);
    let expected_prehash = "57dc10c3d1893ff16e1f5a47fa4b2e96f37b9c57d98a42d88c971debb4947ec9";
    assert_eq!(preimage_output.data_hash.to_hex(), expected_prehash);

    // Step 2: Compile transaction info

    // Simulate signature, normally obtained from signature server
    let signature = "f7a9ec0a521170bb5566ca973d3c73a1b69b162d99ce022059189991ec440637333394ff1c9e75fad84eb114393969f20989b036f1dfed28949e906dc0077421".decode_hex().unwrap();
    let public_key = ACCOUNT_88701_PUBLIC_KEY.decode_hex().unwrap();

    let signatures = TWDataVectorHelper::create([signature.clone()]);
    let public_keys = TWDataVectorHelper::create([public_key]);

    let input_data = TWDataHelper::create(serialize(&input).unwrap());
    let output_data = TWDataHelper::wrap(unsafe {
        tw_transaction_compiler_compile(
            NATIVE_INJECTIVE_COIN_TYPE,
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
    let expected_encoded = r#"{"mode":"BROADCAST_MODE_BLOCK","tx_bytes":"Co8BCowBChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEmwKKmluajFkMGprcnNkMDljN3B1bGU0M3kzeWxydWw0M2x3d2NxYWt5OHc4YxIqaW5qMXhtcGtteHI0YXMwMGVtMjN0YzJ6Z211eXkyZ3I0aDN3Z2NsNnZkGhIKA2luahILMTAwMDAwMDAwMDASnAEKfAp0Ci0vaW5qZWN0aXZlLmNyeXB0by52MWJldGExLmV0aHNlY3AyNTZrMS5QdWJLZXkSQwpBBAiKwpGZh9knNoyyvireRM0O02FnRalpnK4mSz/Fp8NgfZn0Qbg0CZDumQyz6vVg8fC6/mAMfpSkvoOSFmmE9ygSBAoCCAESHAoWCgNpbmoSDzEwMDAwMDAwMDAwMDAwMBCw2wYaQPep7ApSEXC7VWbKlz08c6G2mxYtmc4CIFkYmZHsRAY3MzOU/xyedfrYTrEUOTlp8gmJsDbx3+0olJ6QbcAHdCE="}"#;
    assert_eq!(output.serialized, expected_encoded);
    assert_eq!(output.signature.to_hex(), signature.to_hex());
}
