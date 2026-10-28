// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

//! Input deduplication and argument validation of `ProgrammableTransactionBuilder`.

use serde::Serialize;
use std::str::FromStr;
use tw_coin_entry::error::prelude::*;
use tw_encoding::bcs;
use tw_sui::address::SuiAddress;
use tw_sui::transaction::command::{Argument, Command};
use tw_sui::transaction::programmable_transaction::ProgrammableTransactionBuilder;
use tw_sui::transaction::sui_types::{
    CallArg, ObjectArg, ObjectDigest, ObjectID, ObjectRef, SequenceNumber,
};

const ALICE: &str = "0x3d498ee6ab43423781b5509957d6e75ffb30c4b98933ee0dddaafa52a7a7a634";
const OBJECT_1: &str = "0x636020b3a7dc7b11c3aa6f419b17f8a9c12e7f79a31d1bdd2de670b4edd63005";
const OBJECT_2: &str = "0x5ef77d20c7d6745d3d9b5f69e7825aae733fa5c8a3f82f7192749e3169791c8c";
const DIGEST: &str = "2eKuWbZSVfpFVfg8FXY9wP6W5AFXnTchSoUdp7obyYZ5";

fn object_id(hex: &str) -> ObjectID {
    ObjectID(SuiAddress::from_str(hex).unwrap().into_inner())
}

fn object_ref(hex: &str, version: u64) -> ObjectRef {
    (
        object_id(hex),
        SequenceNumber(version),
        ObjectDigest::from_str(DIGEST).unwrap(),
    )
}

fn shared(hex: &str, initial_shared_version: u64, mutable: bool) -> ObjectArg {
    ObjectArg::SharedObject {
        id: object_id(hex),
        initial_shared_version: SequenceNumber(initial_shared_version),
        mutable,
    }
}

fn bcs_of<T: Serialize>(value: &T) -> Vec<u8> {
    bcs::encode(value).unwrap()
}

fn error_type<T>(result: SigningResult<T>) -> SigningErrorType {
    *result.err().expect("must be rejected").error_type()
}

#[test]
fn test_the_same_shared_object_is_one_input_that_becomes_mutable_when_any_use_needs_it() {
    let mut builder = ProgrammableTransactionBuilder::default();
    let first = builder.obj(shared(OBJECT_1, 7, false)).unwrap();
    let second = builder.obj(shared(OBJECT_1, 7, true)).unwrap();
    let third = builder.obj(shared(OBJECT_1, 7, false)).unwrap();
    for argument in [&first, &second, &third] {
        assert_eq!(bcs_of(argument), bcs_of(&Argument::Input(0)));
    }

    let ptb = builder.finish();
    assert_eq!(ptb.inputs.len(), 1);
    assert_eq!(
        bcs_of(&ptb.inputs[0]),
        bcs_of(&CallArg::Object(shared(OBJECT_1, 7, true)))
    );
}

#[test]
fn test_the_same_owned_object_is_deduplicated() {
    let mut builder = ProgrammableTransactionBuilder::default();
    let owned = || ObjectArg::ImmOrOwnedObject(object_ref(OBJECT_1, 3));
    let first = builder.obj(owned()).unwrap();
    let again = builder.obj(owned()).unwrap();
    assert_eq!(bcs_of(&first), bcs_of(&again));
    assert_eq!(builder.finish().inputs.len(), 1);
}

#[test]
fn test_one_object_cannot_be_used_in_two_incompatible_ways() {
    let mut builder = ProgrammableTransactionBuilder::default();
    builder
        .obj(ObjectArg::ImmOrOwnedObject(object_ref(OBJECT_1, 3)))
        .unwrap();

    // A different version of the same owned object.
    let other_version = builder.obj(ObjectArg::ImmOrOwnedObject(object_ref(OBJECT_1, 4)));
    assert_eq!(error_type(other_version), SigningErrorType::Error_internal);
    // The same id as a shared object.
    let as_shared = builder.obj(shared(OBJECT_1, 3, true));
    assert_eq!(error_type(as_shared), SigningErrorType::Error_internal);
}

#[test]
fn test_a_shared_object_cannot_change_its_initial_version() {
    let mut builder = ProgrammableTransactionBuilder::default();
    builder.obj(shared(OBJECT_1, 7, false)).unwrap();
    let result = builder.obj(shared(OBJECT_1, 8, false));
    assert_eq!(error_type(result), SigningErrorType::Error_internal);
}

#[test]
fn test_make_obj_vec_puts_every_object_into_one_vector() {
    let mut builder = ProgrammableTransactionBuilder::default();
    let vector = builder
        .make_obj_vec([
            ObjectArg::ImmOrOwnedObject(object_ref(OBJECT_1, 1)),
            ObjectArg::ImmOrOwnedObject(object_ref(OBJECT_2, 2)),
        ])
        .unwrap();
    assert_eq!(bcs_of(&vector), bcs_of(&Argument::Result(0)));

    let ptb = builder.finish();
    assert_eq!(ptb.inputs.len(), 2);
    assert_eq!(
        bcs_of(&ptb.commands),
        bcs_of(&vec![Command::MakeMoveVec(
            None,
            vec![Argument::Input(0), Argument::Input(1)]
        )])
    );
}

#[test]
fn test_pay_validates_its_arguments() {
    let alice = SuiAddress::from_str(ALICE).unwrap();

    let mut builder = ProgrammableTransactionBuilder::default();
    let no_coins = builder.pay(vec![], vec![alice], vec![1]);
    assert_eq!(error_type(no_coins), SigningErrorType::Error_invalid_params);

    let mut builder = ProgrammableTransactionBuilder::default();
    let mismatch = builder.pay(vec![object_ref(OBJECT_1, 1)], vec![alice], vec![]);
    assert_eq!(error_type(mismatch), SigningErrorType::Error_invalid_params);

    // Nothing to send: the coin is only registered, no split is built.
    let mut builder = ProgrammableTransactionBuilder::default();
    builder
        .pay(vec![object_ref(OBJECT_1, 1)], vec![], vec![])
        .unwrap();
    assert!(builder.finish().commands.is_empty());
}

#[test]
fn test_a_receiving_object_is_an_input_keyed_by_its_id() {
    let mut builder = ProgrammableTransactionBuilder::default();
    let first = builder
        .obj(ObjectArg::Receiving(object_ref(OBJECT_1, 1)))
        .unwrap();
    let again = builder
        .obj(ObjectArg::Receiving(object_ref(OBJECT_1, 1)))
        .unwrap();
    assert_eq!(bcs_of(&first), bcs_of(&again));
    assert_eq!(builder.finish().inputs.len(), 1);
}

#[test]
fn test_forced_separate_pure_inputs_are_not_deduplicated() {
    let mut builder = ProgrammableTransactionBuilder::default();
    let first = builder.pure_bytes(vec![1], true);
    let second = builder.pure_bytes(vec![1], true);
    let shared = builder.pure_bytes(vec![1], false);
    assert_eq!(bcs_of(&first), bcs_of(&Argument::Input(0)));
    assert_eq!(bcs_of(&second), bcs_of(&Argument::Input(1)));
    assert_eq!(bcs_of(&shared), bcs_of(&Argument::Input(2)));
    assert_eq!(builder.finish().inputs.len(), 3);
}
