mod common;
use common::{pubkey_hash_from_hex, txid_rev};

use bitcoin::ScriptBuf;
use tw_proto::Utxo::Proto;
use tw_utxo::compiler::{Compiler, StandardBitcoinContext};

const WEIGHT_BASE: u64 = 2;

// Convenience function, creates the change output script.
fn change_output() -> ScriptBuf {
    let pubkey_hash = pubkey_hash_from_hex("aabbccddeeff00112233445566778899aabbccdd");
    ScriptBuf::new_p2pkh(&pubkey_hash)
}

#[test]
fn input_selector_all() {
    // Reusing the txid is fine here, although in production this would mark the transaction invalid.
    let txid = txid_rev("1e1cdc48aa990d7e154a161d5b5f1cad737742e97d2712ab188027bb42e6e47b");

    let tx1 = Proto::TxIn {
        txid: txid.as_slice().into(),
        value: 1_000,
        sequence: u32::MAX,
        ..Default::default()
    };
    let tx2 = Proto::TxIn {
        txid: txid.as_slice().into(),
        value: 2_000,
        sequence: u32::MAX,
        ..Default::default()
    };
    let tx3 = Proto::TxIn {
        txid: txid.as_slice().into(),
        value: 3_000,
        sequence: u32::MAX,
        ..Default::default()
    };

    let out1 = Proto::TxOut {
        value: 500,
        script_pubkey: Default::default(),
    };

    // Generate prehashes without change output.
    let signing = Proto::SigningInput {
        version: 2,
        lock_time: Default::default(),
        inputs: vec![tx1.clone(), tx2.clone(), tx3.clone()],
        outputs: vec![out1.clone()],
        // Explicitly select all inputs.
        input_selector: Proto::InputSelector::UseAll,
        weight_base: WEIGHT_BASE,
        change_script_pubkey: Default::default(),
        // DISABLE change output.
        disable_change_output: true,
    };

    let output = Compiler::<StandardBitcoinContext>::preimage_hashes(signing);
    assert_eq!(output.error, Proto::Error::OK);
    assert_eq!(output.sighashes.len(), 3);

    // All inputs are used as mandated by the `input_selector`. Technically only
    // one input is needed.
    assert_eq!(output.inputs.len(), 3);
    assert_eq!(output.inputs[0], tx1);
    assert_eq!(output.inputs[1], tx2);
    assert_eq!(output.inputs[2], tx3);

    assert_eq!(output.outputs.len(), 1);
    assert_eq!(output.outputs[0], out1);

    // Generate prehashes WITH change output.
    let change_script = change_output();
    let signing = Proto::SigningInput {
        version: 2,
        lock_time: Default::default(),
        inputs: vec![tx1.clone(), tx2.clone(), tx3.clone()],
        outputs: vec![out1.clone()],
        // Explicitly select all inputs.
        input_selector: Proto::InputSelector::UseAll,
        weight_base: WEIGHT_BASE,
        change_script_pubkey: change_script.as_bytes().into(),
        // ENABLE change output.
        disable_change_output: false,
    };

    let output = Compiler::<StandardBitcoinContext>::preimage_hashes(signing);
    assert_eq!(output.error, Proto::Error::OK);
    assert_eq!(output.sighashes.len(), 3);

    assert_eq!(output.inputs.len(), 3);
    assert_eq!(output.inputs[0], tx1);
    assert_eq!(output.inputs[1], tx2);
    assert_eq!(output.inputs[2], tx3);

    // All inputs: 6_000, all outputs: 500
    let change_out = Proto::TxOut {
        value: 6_000 - 500 - output.fee_estimate,
        script_pubkey: change_script.as_bytes().into(),
    };

    assert_eq!(output.outputs.len(), 2);
    assert_eq!(output.outputs[0], out1);
    assert_eq!(output.outputs[1], change_out);
}

#[test]
fn input_selector_all_insufficient_inputs() {
    // Reusing the txid is fine here, although in production this would mark the transaction invalid.
    let txid = txid_rev("1e1cdc48aa990d7e154a161d5b5f1cad737742e97d2712ab188027bb42e6e47b");

    let tx1 = Proto::TxIn {
        txid: txid.as_slice().into(),
        value: 1_000,
        sequence: u32::MAX,
        ..Default::default()
    };
    let tx2 = Proto::TxIn {
        txid: txid.as_slice().into(),
        value: 2_000,
        sequence: u32::MAX,
        ..Default::default()
    };
    let tx3 = Proto::TxIn {
        txid: txid.as_slice().into(),
        value: 3_000,
        sequence: u32::MAX,
        ..Default::default()
    };

    let out1 = Proto::TxOut {
        value: 6_000,
        script_pubkey: Default::default(),
    };

    // Generate prehashes without change output.
    let signing = Proto::SigningInput {
        version: 2,
        lock_time: Default::default(),
        inputs: vec![tx1.clone(), tx2.clone(), tx3.clone()],
        outputs: vec![out1.clone()],
        // Explicitly select all inputs.
        input_selector: Proto::InputSelector::UseAll,
        weight_base: WEIGHT_BASE,
        change_script_pubkey: Default::default(),
        // DISABLE change output.
        disable_change_output: true,
    };

    let output = Compiler::<StandardBitcoinContext>::preimage_hashes(signing);
    // While the input covers all outputs, it does not
    // cover the projected fee.
    assert_eq!(output.error, Proto::Error::Error_insufficient_inputs);
    assert_eq!(output.sighashes.len(), 0);
    assert_eq!(output.inputs.len(), 0);
    assert_eq!(output.outputs.len(), 0);

    // Generate prehashes WITH change output (same outcome).
    let change_script = change_output();
    let signing = Proto::SigningInput {
        version: 2,
        lock_time: Default::default(),
        inputs: vec![tx1.clone(), tx2.clone(), tx3.clone()],
        outputs: vec![out1.clone()],
        // Explicitly select all inputs.
        input_selector: Proto::InputSelector::UseAll,
        weight_base: WEIGHT_BASE,
        change_script_pubkey: change_script.as_bytes().into(),
        // ENABLE change output.
        disable_change_output: false,
    };

    let output = Compiler::<StandardBitcoinContext>::preimage_hashes(signing);
    assert_eq!(output.error, Proto::Error::Error_insufficient_inputs);
    assert_eq!(output.sighashes.len(), 0);
    assert_eq!(output.inputs.len(), 0);
    assert_eq!(output.outputs.len(), 0);
}

#[test]
fn input_selector_one_input_required() {
    // Reusing the txid is fine here, although in production this would mark the transaction invalid.
    let txid = txid_rev("1e1cdc48aa990d7e154a161d5b5f1cad737742e97d2712ab188027bb42e6e47b");

    let tx1 = Proto::TxIn {
        txid: txid.as_slice().into(),
        value: 4_000,
        sequence: u32::MAX,
        ..Default::default()
    };
    let tx2 = Proto::TxIn {
        txid: txid.as_slice().into(),
        value: 4_000,
        sequence: u32::MAX,
        ..Default::default()
    };
    let tx3 = Proto::TxIn {
        txid: txid.as_slice().into(),
        value: 4_000,
        sequence: u32::MAX,
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

    // Generate sighashes without change output.
    let signing = Proto::SigningInput {
        version: 2,
        lock_time: Default::default(),
        inputs: vec![tx1.clone(), tx2.clone(), tx3.clone()],
        outputs: vec![out1.clone(), out2.clone()],
        input_selector: Proto::InputSelector::SelectInOrder,
        weight_base: WEIGHT_BASE,
        change_script_pubkey: Default::default(),
        // DISABLE change output.
        disable_change_output: true,
    };

    let output = Compiler::<StandardBitcoinContext>::preimage_hashes(signing);
    assert_eq!(output.error, Proto::Error::OK);
    assert_eq!(output.sighashes.len(), 1);

    // One inputs covers the full output.
    assert_eq!(output.inputs.len(), 1);
    assert_eq!(output.inputs[0], tx1);

    assert_eq!(output.outputs.len(), 2);
    assert_eq!(output.outputs[0], out1);
    assert_eq!(output.outputs[1], out2);

    // Generate sighashes WITH change output.
    let change_script = change_output();
    let signing = Proto::SigningInput {
        version: 2,
        lock_time: Default::default(),
        inputs: vec![tx1.clone(), tx2.clone(), tx3.clone()],
        outputs: vec![out1.clone(), out2.clone()],
        input_selector: Proto::InputSelector::SelectInOrder,
        weight_base: WEIGHT_BASE,
        change_script_pubkey: change_script.as_bytes().into(),
        // ENABLE change output.
        disable_change_output: false,
    };

    let output = Compiler::<StandardBitcoinContext>::preimage_hashes(signing);
    assert_eq!(output.error, Proto::Error::OK);
    assert_eq!(output.sighashes.len(), 1);

    // One inputs covers the full output.
    assert_eq!(output.inputs.len(), 1);
    assert_eq!(output.inputs[0], tx1);

    // All inputs: 4_000, all outputs: 2_000
    let change_out = Proto::TxOut {
        value: 4_000 - 1_000 - output.fee_estimate,
        script_pubkey: change_script.as_bytes().into(),
    };

    assert_eq!(output.outputs.len(), 3);
    assert_eq!(output.outputs[0], out1);
    assert_eq!(output.outputs[1], out2);
    assert_eq!(output.outputs[2], change_out);
}

#[test]
fn input_selector_two_inputs_required() {
    // Reusing the txid is fine here, although in production this would mark the transaction invalid.
    let txid = txid_rev("1e1cdc48aa990d7e154a161d5b5f1cad737742e97d2712ab188027bb42e6e47b");

    let tx1 = Proto::TxIn {
        txid: txid.as_slice().into(),
        value: 1_000,
        sequence: u32::MAX,
        ..Default::default()
    };
    let tx2 = Proto::TxIn {
        txid: txid.as_slice().into(),
        value: 3_000,
        sequence: u32::MAX,
        ..Default::default()
    };
    let tx3 = Proto::TxIn {
        txid: txid.as_slice().into(),
        value: 4_000,
        sequence: u32::MAX,
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

    // Generate sighashes without change output.
    let signing = Proto::SigningInput {
        version: 2,
        lock_time: Default::default(),
        inputs: vec![tx1.clone(), tx2.clone(), tx3.clone()],
        outputs: vec![out1.clone(), out2.clone()],
        // We only select the necessary value of inputs to cover the output
        // value.
        input_selector: Proto::InputSelector::SelectInOrder,
        weight_base: WEIGHT_BASE,
        change_script_pubkey: Default::default(),
        // DISABLE change output.
        disable_change_output: true,
    };

    let output = Compiler::<StandardBitcoinContext>::preimage_hashes(signing);
    assert_eq!(output.error, Proto::Error::OK);

    // Only two inputs are needed to cover outputs.
    assert_eq!(output.inputs.len(), 2);
    assert_eq!(output.inputs[0], tx1);
    assert_eq!(output.inputs[1], tx2);

    assert_eq!(output.outputs.len(), 2);
    assert_eq!(output.outputs[0], out1);
    assert_eq!(output.outputs[1], out2);

    // Generate sighashes WITH change output.
    let change_script = change_output();
    let signing = Proto::SigningInput {
        version: 2,
        lock_time: Default::default(),
        inputs: vec![tx1.clone(), tx2.clone(), tx3.clone()],
        outputs: vec![out1.clone(), out2.clone()],
        // We only select the necessary value of inputs to cover the output
        // value.
        input_selector: Proto::InputSelector::SelectInOrder,
        weight_base: WEIGHT_BASE,
        change_script_pubkey: change_script.as_bytes().into(),
        // ENABLE change output.
        disable_change_output: false,
    };

    let output = Compiler::<StandardBitcoinContext>::preimage_hashes(signing);
    assert_eq!(output.error, Proto::Error::OK);
    assert_eq!(output.sighashes.len(), 2);

    // Only two inputs are needed to cover outputs.
    assert_eq!(output.inputs.len(), 2);
    assert_eq!(output.inputs[0], tx1);
    assert_eq!(output.inputs[1], tx2);

    // All inputs: 4_000, all outputs: 2_000
    let change_out = Proto::TxOut {
        value: 4_000 - 2_000 - output.fee_estimate,
        script_pubkey: change_script.as_bytes().into(),
    };

    assert_eq!(output.outputs.len(), 3);
    assert_eq!(output.outputs[0], out1);
    assert_eq!(output.outputs[1], out2);
    assert_eq!(output.outputs[2], change_out);
}

#[test]
fn input_selector_one_input_cannot_cover_fees() {
    // Reusing the txid is fine here, although in production this would mark the transaction invalid.
    let txid = txid_rev("1e1cdc48aa990d7e154a161d5b5f1cad737742e97d2712ab188027bb42e6e47b");

    let tx1 = Proto::TxIn {
        txid: txid.as_slice().into(),
        value: 2_000,
        sequence: u32::MAX,
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

    // Generate sighashes without change output.
    let signing = Proto::SigningInput {
        version: 2,
        lock_time: Default::default(),
        inputs: vec![tx1.clone()],
        outputs: vec![out1.clone(), out2.clone()],
        input_selector: Proto::InputSelector::SelectInOrder,
        weight_base: WEIGHT_BASE,
        change_script_pubkey: Default::default(),
        // DISABLE change output.
        disable_change_output: true,
    };

    let output = Compiler::<StandardBitcoinContext>::preimage_hashes(signing);
    // While the input covers all outputs, it does not
    // cover the projected fee.
    assert_eq!(output.error, Proto::Error::Error_insufficient_inputs);
    assert_eq!(output.weight_estimate, 0);
    assert_eq!(output.fee_estimate, 0);
    assert_eq!(output.sighashes.len(), 0);
    assert_eq!(output.inputs.len(), 0);
    assert_eq!(output.outputs.len(), 0);

    // Generate sighashes WITH change output (same outcome).
    let change_script = change_output();
    let signing = Proto::SigningInput {
        version: 2,
        lock_time: Default::default(),
        inputs: vec![tx1.clone()],
        outputs: vec![out1.clone(), out2.clone()],
        input_selector: Proto::InputSelector::SelectInOrder,
        weight_base: WEIGHT_BASE,
        change_script_pubkey: change_script.as_bytes().into(),
        // ENABLE change output.
        disable_change_output: false,
    };

    let output = Compiler::<StandardBitcoinContext>::preimage_hashes(signing);
    assert_eq!(output.error, Proto::Error::Error_insufficient_inputs);
    assert_eq!(output.sighashes.len(), 0);
    assert_eq!(output.inputs.len(), 0);
    assert_eq!(output.outputs.len(), 0);
}

#[ignore]
#[test]
fn input_selector_exact_balance_no_change() {
    // Reusing the txid is fine here, although in production this would mark the transaction invalid.
    let txid = txid_rev("1e1cdc48aa990d7e154a161d5b5f1cad737742e97d2712ab188027bb42e6e47b");

    let tx1 = Proto::TxIn {
        txid: txid.as_slice().into(),
        // Covers the exact output value + projected fee.
        value: 2_000 + (302 + 3) / 4 * WEIGHT_BASE,
        sequence: u32::MAX,
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

    let change_script = change_output();
    let signing = Proto::SigningInput {
        version: 2,
        lock_time: Default::default(),
        inputs: vec![tx1.clone()],
        outputs: vec![out1.clone(), out2.clone()],
        input_selector: Proto::InputSelector::SelectInOrder,
        weight_base: WEIGHT_BASE,
        change_script_pubkey: change_script.as_bytes().into(),
        // ENABLE change output.
        disable_change_output: false,
    };

    let output = Compiler::<StandardBitcoinContext>::preimage_hashes(signing);
    assert_eq!(output.error, Proto::Error::OK);
    assert_eq!(output.sighashes.len(), 1);
    assert_eq!(output.weight_estimate, 302);
    assert_eq!(output.fee_estimate, (302 + 3) / 4 * WEIGHT_BASE);

    // One inputs covers the full output.
    assert_eq!(output.inputs.len(), 1);
    assert_eq!(output.inputs[0], tx1);

    // NO change output
    assert_eq!(output.outputs.len(), 2);
    assert_eq!(output.outputs[0], out1);
    assert_eq!(output.outputs[1], out2);
}

#[test]
fn input_selector_empty_script_bufs() {
    // Reusing the txid is fine here, although in production this would mark the transaction invalid.
    let txid = txid_rev("1e1cdc48aa990d7e154a161d5b5f1cad737742e97d2712ab188027bb42e6e47b");

    let tx1 = Proto::TxIn {
        txid: txid.as_slice().into(),
        value: 4_000,
        sequence: u32::MAX,
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

    let signing = Proto::SigningInput {
        version: 2,
        lock_time: Default::default(),
        inputs: vec![tx1.clone()],
        outputs: vec![out1.clone(), out2.clone()],
        input_selector: Proto::InputSelector::SelectInOrder,
        weight_base: WEIGHT_BASE,
        // NO change script_pubkey specified, results in an error.
        change_script_pubkey: Default::default(),
        // ENABLE change output.
        disable_change_output: false,
    };

    let output = Compiler::<StandardBitcoinContext>::preimage_hashes(signing);
    assert_eq!(
        output.error,
        Proto::Error::Error_missing_change_script_pubkey
    );
    assert_eq!(output.sighashes.len(), 0);
    assert_eq!(output.weight_estimate, 0);
    assert_eq!(output.fee_estimate, 0);
    assert_eq!(output.inputs.len(), 0);
    assert_eq!(output.outputs.len(), 0);
}

#[test]
fn input_selector_select_ascending() {
    // Reusing the txid is fine here, although in production this would mark the transaction invalid.
    let txid = txid_rev("1e1cdc48aa990d7e154a161d5b5f1cad737742e97d2712ab188027bb42e6e47b");

    let tx1 = Proto::TxIn {
        txid: txid.as_slice().into(),
        value: 8_000,
        sequence: u32::MAX,
        ..Default::default()
    };

    let tx2 = Proto::TxIn {
        txid: txid.as_slice().into(),
        value: 4_000,
        sequence: u32::MAX,
        ..Default::default()
    };

    let tx3 = Proto::TxIn {
        txid: txid.as_slice().into(),
        value: 2_000,
        sequence: u32::MAX,
        ..Default::default()
    };

    let out1 = Proto::TxOut {
        value: 3_000,
        script_pubkey: Default::default(),
    };

    let change_script = change_output();
    let signing = Proto::SigningInput {
        version: 2,
        lock_time: Default::default(),
        inputs: vec![tx1.clone(), tx2.clone(), tx3.clone()],
        outputs: vec![out1.clone()],
        // Select in ASCENDING order:
        input_selector: Proto::InputSelector::SelectAscending,
        weight_base: WEIGHT_BASE,
        change_script_pubkey: change_script.as_bytes().into(),
        // ENABLE change output.
        disable_change_output: false,
    };

    let output = Compiler::<StandardBitcoinContext>::preimage_hashes(signing);
    assert_eq!(output.error, Proto::Error::OK);

    // Two inputs where selected (in ASCENDING order).
    assert_eq!(output.inputs.len(), 2);
    assert_eq!(output.inputs[0], tx3);
    assert_eq!(output.inputs[1], tx2);

    // Two outputs; target and change.
    assert_eq!(output.outputs.len(), 2);
    assert_eq!(output.outputs[0], out1);
}
