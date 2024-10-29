// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::pactus::test_cases::PRIVATE_KEY;
use crate::chains::pactus::test_cases::transfer::{
    pactus_sign_transfer_input, TRANSFER_SIGNATURE, TRANSACTION_TRANSFER_SIGNED_DATA, TRANSFER_TX_ID};
use crate::chains::pactus::test_cases::bond::{
    pactus_sign_bond_input, BOND_SIGNATURE, TRANSACTION_BOND_SIGNED_DATA, BOND_TX_ID};
use tw_any_coin::ffi::tw_any_signer::tw_any_signer_sign;
use tw_coin_entry::error::prelude::*;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_memory::test_utils::tw_data_helper::TWDataHelper;
use tw_proto::Pactus::Proto;
use tw_proto::{deserialize, serialize};

// Successfully broadcast transfer:
// https://pacviewer.com/transaction/1b6b7226f7935a15f05371d1a1fefead585a89704ce464b7cc1d453d299d235f
#[test]
fn test_pactus_sign_transfer() {
    let input = Proto::SigningInput {
        private_key: PRIVATE_KEY.decode_hex().unwrap().into(),
        ..pactus_sign_transfer_input()
    };

    let input_data = TWDataHelper::create(serialize(&input).unwrap());

    let output = TWDataHelper::wrap(unsafe {
        tw_any_signer_sign(input_data.ptr(), CoinType::Pactus as u32)
    })
    .to_vec()
    .expect("!tw_any_signer_sign returned nullptr");

    let output: Proto::SigningOutput = deserialize(&output).unwrap();

    assert_eq!(output.error, SigningErrorType::OK);
    assert!(output.error_message.is_empty());
    assert_eq!(output.transaction_id.to_hex(), TRANSFER_TX_ID);
    assert_eq!(output.signature.to_hex(), TRANSFER_SIGNATURE);
    assert_eq!(
        output.signed_transaction_data.to_hex(),
        TRANSACTION_TRANSFER_SIGNED_DATA
    );
}

// Successfully broadcast bond:
// https://pacviewer.com/transaction/d194b445642a04ec78ced4448696e50b733f2f0b517a23871882c0eefaf1c28f
#[test]
fn test_pactus_sign_bond() {
    let input = Proto::SigningInput {
        private_key: PRIVATE_KEY.decode_hex().unwrap().into(),
        ..pactus_sign_bond_input()
    };

    let input_data = TWDataHelper::create(serialize(&input).unwrap());

    let output = TWDataHelper::wrap(unsafe {
        tw_any_signer_sign(input_data.ptr(), CoinType::Pactus as u32)
    })
        .to_vec()
        .expect("!tw_any_signer_sign returned nullptr");

    let output: Proto::SigningOutput = deserialize(&output).unwrap();

    assert_eq!(output.error, SigningErrorType::OK);
    assert!(output.error_message.is_empty());
    assert_eq!(output.transaction_id.to_hex(), BOND_TX_ID);
    assert_eq!(output.signature.to_hex(), BOND_SIGNATURE);
    assert_eq!(
        output.signed_transaction_data.to_hex(),
        TRANSACTION_BOND_SIGNED_DATA
    );
}