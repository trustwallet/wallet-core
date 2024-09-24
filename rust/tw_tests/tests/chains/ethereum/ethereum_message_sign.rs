// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::ffi::tw_message_signer::{
    tw_message_signer_pre_image_hashes, tw_message_signer_sign, tw_message_signer_verify,
};
use tw_coin_entry::error::prelude::*;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_memory::test_utils::tw_data_helper::TWDataHelper;
use tw_proto::{deserialize, serialize, Ethereum, TxCompiler};

#[test]
fn test_tw_message_signer_sign() {
    let input = Ethereum::Proto::MessageSigningInput {
        private_key: "03a9ca895dca1623c7dfd69693f7b4111f5d819d2e145536e0b03c136025a25d"
            .decode_hex()
            .unwrap()
            .into(),
        message: "Foo".into(),
        chain_id: None,
        message_type: Ethereum::Proto::MessageType::MessageType_legacy,
    };

    let input_data = TWDataHelper::create(serialize(&input).unwrap());
    let output = TWDataHelper::wrap(unsafe {
        tw_message_signer_sign(input_data.ptr(), CoinType::Ethereum as u32)
    })
    .to_vec()
    .expect("!tw_message_signer_sign returned nullptr");

    let output: Ethereum::Proto::MessageSigningOutput = deserialize(&output).unwrap();
    assert_eq!(output.error, SigningErrorType::OK);
    assert!(output.error_message.is_empty());
    assert_eq!(output.signature, "21a779d499957e7fd39392d49a079679009e60e492d9654a148829be43d2490736ec72bc4a5644047d979c3cf4ebe2c1c514044cf436b063cb89fc6676be71101b");
}

#[test]
fn test_tw_message_signer_verify() {
    let input = Ethereum::Proto::MessageVerifyingInput {
        message: "Foo".into(),
        public_key: "0349d0134ef2c798c02879379a1760baa49c4e25e2324cd128f11e559f073bcc6f".decode_hex().unwrap().into(),
        signature: "21a779d499957e7fd39392d49a079679009e60e492d9654a148829be43d2490736ec72bc4a5644047d979c3cf4ebe2c1c514044cf436b063cb89fc6676be71101b".into(),
    };

    let input_data = TWDataHelper::create(serialize(&input).unwrap());
    let verified = unsafe { tw_message_signer_verify(input_data.ptr(), CoinType::Ethereum as u32) };
    assert!(verified);
}

#[test]
fn test_tw_message_signer_verify_invalid() {
    let input = Ethereum::Proto::MessageVerifyingInput {
        message: "Foo".into(),
        public_key: "0349d0134ef2c798c02879379a1760baa49c4e25e2324cd128f11e559f073bcc6f".decode_hex().unwrap().into(),
        signature: "21a779d499957e7fd39392d49a079679009e60e492d9654a148829be43d2490736ec72bc4a5644047d979c3cf4ebe2c1c514044cf436b063cb89fc6676be71101c".into(),
    };

    let input_data = TWDataHelper::create(serialize(&input).unwrap());
    let verified = unsafe { tw_message_signer_verify(input_data.ptr(), CoinType::Ethereum as u32) };
    assert!(!verified);
}

#[test]
fn test_tw_message_signer_pre_image_hashes() {
    let input = Ethereum::Proto::MessageSigningInput {
        private_key: "03a9ca895dca1623c7dfd69693f7b4111f5d819d2e145536e0b03c136025a25d"
            .decode_hex()
            .unwrap()
            .into(),
        message: "Foo".into(),
        chain_id: None,
        message_type: Ethereum::Proto::MessageType::MessageType_legacy,
    };

    let input_data = TWDataHelper::create(serialize(&input).unwrap());
    let output = TWDataHelper::wrap(unsafe {
        tw_message_signer_pre_image_hashes(input_data.ptr(), CoinType::Ethereum as u32)
    })
    .to_vec()
    .expect("!tw_message_signer_sign returned nullptr");

    let output: TxCompiler::Proto::PreSigningOutput = deserialize(&output).unwrap();
    assert_eq!(output.error, SigningErrorType::OK);
    assert!(output.error_message.is_empty());
    assert_eq!(
        output.data_hash.to_hex(),
        "0af844076e792f9685560b2e597967da7403b00a5339b5801ea251ddde375f8a"
    );
}
