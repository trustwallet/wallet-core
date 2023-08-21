mod common;
use common::txid_rev;

use tw_proto::Utxo::Proto;
use tw_utxo::compiler::{Compiler, StandardBitcoinContext};

#[test]
fn input_selector_all() {
    // Reusing the txid is fine here, although in production this would mark the transaction invalid.
    let txid = txid_rev("1e1cdc48aa990d7e154a161d5b5f1cad737742e97d2712ab188027bb42e6e47b");

    let tx1 = Proto::TxIn {
        txid: txid.as_slice().into(),
        amount: 1_000,
        ..Default::default()
    };
    let tx2 = Proto::TxIn {
        txid: txid.as_slice().into(),
        amount: 2_000,
        ..Default::default()
    };
    let tx3 = Proto::TxIn {
        txid: txid.as_slice().into(),
        amount: 3_000,
        ..Default::default()
    };

    let out1 = Proto::TxOut {
        value: 5_00,
        script_pubkey: Default::default(),
    };

    // We explicitly select all inputs.
    let signing = Proto::SigningInput {
        version: 2,
        lock_time: Default::default(),
        inputs: vec![tx1.clone(), tx2.clone(), tx3.clone()],
        outputs: vec![out1.clone()],
        input_selector: Proto::InputSelector::UseAll,
        weight_base: 1,
        change_script_pubkey: Default::default(),
        disable_change_output: true,
    };

    let output = Compiler::<StandardBitcoinContext>::preimage_hashes(signing);
    assert_eq!(output.error, Proto::Error::OK);
    assert_eq!(output.sighashes.len(), 3);

    assert_eq!(output.inputs.len(), 3);
    assert_eq!(output.inputs[0], tx1);
    assert_eq!(output.inputs[1], tx2);
    assert_eq!(output.inputs[2], tx3);

    assert_eq!(output.outputs.len(), 1);
    assert_eq!(output.outputs[0], out1);
}

#[test]
fn input_selector_less_than_output() {
    // Reusing the txid is fine here, although in production this would mark the transaction invalid.
    let txid = txid_rev("1e1cdc48aa990d7e154a161d5b5f1cad737742e97d2712ab188027bb42e6e47b");

    let tx1 = Proto::TxIn {
        txid: txid.as_slice().into(),
        amount: 1_000,
        ..Default::default()
    };
    let tx2 = Proto::TxIn {
        txid: txid.as_slice().into(),
        amount: 2_000,
        ..Default::default()
    };
    let tx3 = Proto::TxIn {
        txid: txid.as_slice().into(),
        amount: 3_000,
        ..Default::default()
    };

    let out1 = Proto::TxOut {
        value: 1_000,
        script_pubkey: Default::default(),
    };
    let out2 = Proto::TxOut {
        value: 1_000,
        script_pubkey: Default::default(),
    };

    // We explicitly select all inputs.
    let signing = Proto::SigningInput {
        version: 2,
        lock_time: Default::default(),
        inputs: vec![tx1.clone(), tx2.clone(), tx3.clone()],
        outputs: vec![out1.clone(), out2.clone()],
        input_selector: Proto::InputSelector::SelectAscending,
        weight_base: 1,
        change_script_pubkey: Default::default(),
        disable_change_output: true,
    };

    let output = Compiler::<StandardBitcoinContext>::preimage_hashes(signing);
    assert_eq!(output.error, Proto::Error::OK);
    assert_eq!(output.sighashes.len(), 2);

    // inputs (1_000 + 2_000) > outputs (1_000 + 1_000)
    assert_eq!(output.inputs.len(), 2);
    assert_eq!(output.inputs[0], tx1);
    assert_eq!(output.inputs[1], tx2);

    assert_eq!(output.outputs.len(), 2);
    assert_eq!(output.outputs[0], out1);
    assert_eq!(output.outputs[1], out2);
}

#[test]
fn input_selector_more_than_output() {
    // Reusing the txid is fine here, although in production this would mark the transaction invalid.
    let txid = txid_rev("1e1cdc48aa990d7e154a161d5b5f1cad737742e97d2712ab188027bb42e6e47b");

    let tx1 = Proto::TxIn {
        txid: txid.as_slice().into(),
        amount: 1_000,
        ..Default::default()
    };
    let tx2 = Proto::TxIn {
        txid: txid.as_slice().into(),
        amount: 2_000,
        ..Default::default()
    };
    let tx3 = Proto::TxIn {
        txid: txid.as_slice().into(),
        amount: 3_000,
        ..Default::default()
    };

    let out1 = Proto::TxOut {
        value: 500,
        script_pubkey: Default::default(),
    };
    let out2 = Proto::TxOut {
        value: 500,
        script_pubkey: Default::default(),
    };

    // We explicitly select all inputs.
    let signing = Proto::SigningInput {
        version: 2,
        lock_time: Default::default(),
        inputs: vec![tx1.clone(), tx2.clone(), tx3.clone()],
        outputs: vec![out1.clone(), out2.clone()],
        input_selector: Proto::InputSelector::SelectAscending,
        weight_base: 1,
        change_script_pubkey: Default::default(),
        disable_change_output: true,
    };

    let output = Compiler::<StandardBitcoinContext>::preimage_hashes(signing);
    assert_eq!(output.error, Proto::Error::OK);
    assert_eq!(output.sighashes.len(), 1);

    // inputs (1_000) > outputs (500 + 300)
    assert_eq!(output.inputs.len(), 1);
    assert_eq!(output.inputs[0], tx1);

    assert_eq!(output.outputs.len(), 2);
    assert_eq!(output.outputs[0], out1);
    assert_eq!(output.outputs[1], out2);
}

#[test]
fn input_selector_insufficient_inputs() {
    // TODO
}

#[test]
fn input_selector_empty_script_bufs() {
    // TODO
}
