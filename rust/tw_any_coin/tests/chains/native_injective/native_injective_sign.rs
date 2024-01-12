// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::borrow::Cow;
use tw_any_coin::ffi::tw_any_signer::tw_any_signer_sign;
use tw_coin_entry::error::SigningErrorType;
use tw_cosmos_sdk::test_utils::proto_utils::{make_amount, make_fee, make_message};
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_memory::test_utils::tw_data_helper::TWDataHelper;
use tw_proto::Cosmos::Proto;
use tw_proto::Cosmos::Proto::mod_Message::OneOfmessage_oneof as MessageEnum;
use tw_proto::{deserialize, serialize};

const NATIVE_INJECTIVE_COIN_TYPE: u32 = 10000060;

fn account_17396_private_key() -> Cow<'static, [u8]> {
    "9ee18daf8e463877aaf497282abc216852420101430482a28e246c179e2c5ef1"
        .decode_hex()
        .unwrap()
        .into()
}

fn send_tx_input() -> Proto::SigningInput<'static> {
    let send_msg = Proto::mod_Message::Send {
        from_address: "inj13u6g7vqgw074mgmf2ze2cadzvkz9snlwcrtq8a".into(),
        to_address: "inj1xmpkmxr4as00em23tc2zgmuyy2gr4h3wgcl6vd".into(),
        amounts: vec![make_amount("inj", "10000000000")],
        ..Proto::mod_Message::Send::default()
    };
    Proto::SigningInput {
        account_number: 17396,
        chain_id: "injective-1".into(),
        sequence: 1,
        fee: Some(make_fee(110000, make_amount("inj", "100000000000000"))),
        private_key: account_17396_private_key(),
        messages: vec![make_message(MessageEnum::send_coins_message(send_msg))],
        ..Proto::SigningInput::default()
    }
}

#[test]
fn test_sign_native_injective_tx_protobuf() {
    let input = Proto::SigningInput {
        signing_mode: Proto::SigningMode::Protobuf,
        ..send_tx_input()
    };
    let input_data = TWDataHelper::create(serialize(&input).unwrap());

    let output = TWDataHelper::wrap(unsafe {
        tw_any_signer_sign(input_data.ptr(), NATIVE_INJECTIVE_COIN_TYPE)
    })
    .to_vec()
    .expect("!tw_any_signer_sign returned nullptr");

    let output: Proto::SigningOutput = deserialize(&output).unwrap();
    assert_eq!(output.error, SigningErrorType::OK);
    assert!(output.error_message.is_empty());

    // https://www.mintscan.io/injective/txs/135DD2C4A1910E4334A9C0F15125DA992E724EBF23FEB9638FCB71218BB064A5
    assert_eq!(
        output.serialized,
        r#"{"mode":"BROADCAST_MODE_BLOCK","tx_bytes":"Co8BCowBChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEmwKKmluajEzdTZnN3ZxZ3cwNzRtZ21mMnplMmNhZHp2a3o5c25sd2NydHE4YRIqaW5qMXhtcGtteHI0YXMwMGVtMjN0YzJ6Z211eXkyZ3I0aDN3Z2NsNnZkGhIKA2luahILMTAwMDAwMDAwMDASngEKfgp0Ci0vaW5qZWN0aXZlLmNyeXB0by52MWJldGExLmV0aHNlY3AyNTZrMS5QdWJLZXkSQwpBBFoMa4O4vZgn5QcnDK20mbfjqQlSRvaiITKB94PYd8mLJWdCdBsGOfMXdo/k9MJ2JmDCESKDp2hdgVUH3uMikXMSBAoCCAEYARIcChYKA2luahIPMTAwMDAwMDAwMDAwMDAwELDbBhpAx2vkplmzeK7n3puCFGPWhLd0l/ZC/CYkGl+stH+3S3hiCvIe7uwwMpUlNaSwvT8HwF1kNUp+Sx2m0Uo1x5xcFw=="}"#
    );
    assert_eq!(output.signature.to_hex(), "c76be4a659b378aee7de9b821463d684b77497f642fc26241a5facb47fb74b78620af21eeeec3032952535a4b0bd3f07c05d64354a7e4b1da6d14a35c79c5c17");
}

#[test]
fn test_sign_native_injective_tx_json() {
    let input = Proto::SigningInput {
        signing_mode: Proto::SigningMode::JSON,
        ..send_tx_input()
    };
    let input_data = TWDataHelper::create(serialize(&input).unwrap());

    let output = TWDataHelper::wrap(unsafe {
        tw_any_signer_sign(input_data.ptr(), NATIVE_INJECTIVE_COIN_TYPE)
    })
    .to_vec()
    .expect("!tw_any_signer_sign returned nullptr");

    let output: Proto::SigningOutput = deserialize(&output).unwrap();
    assert_eq!(output.error, SigningErrorType::OK);
    assert!(output.error_message.is_empty());

    // This transaction hasn't been broadcasted.
    assert_eq!(
        output.json,
        r#"{"mode":"block","tx":{"fee":{"amount":[{"amount":"100000000000000","denom":"inj"}],"gas":"110000"},"memo":"","msg":[{"type":"cosmos-sdk/MsgSend","value":{"amount":[{"amount":"10000000000","denom":"inj"}],"from_address":"inj13u6g7vqgw074mgmf2ze2cadzvkz9snlwcrtq8a","to_address":"inj1xmpkmxr4as00em23tc2zgmuyy2gr4h3wgcl6vd"}}],"signatures":[{"pub_key":{"type":"injective/PubKeyEthSecp256k1","value":"BFoMa4O4vZgn5QcnDK20mbfjqQlSRvaiITKB94PYd8mLJWdCdBsGOfMXdo/k9MJ2JmDCESKDp2hdgVUH3uMikXM="},"signature":"7wwedceebL95DwbClz5AzEp2Z74itHC7raiV976DcacfjrJ58oDfjbAO5UOZQAlihiYBP7PpyISFQ72FPRhdZA=="}]}}"#
    );
    assert_eq!(
        output.signature_json,
        r#"[{"pub_key":{"type":"injective/PubKeyEthSecp256k1","value":"BFoMa4O4vZgn5QcnDK20mbfjqQlSRvaiITKB94PYd8mLJWdCdBsGOfMXdo/k9MJ2JmDCESKDp2hdgVUH3uMikXM="},"signature":"7wwedceebL95DwbClz5AzEp2Z74itHC7raiV976DcacfjrJ58oDfjbAO5UOZQAlihiYBP7PpyISFQ72FPRhdZA=="}]"#
    );
}
