mod common;
use common::{pubkey_hash_from_hex, txid_rev};

use bitcoin::ScriptBuf;
use tw_proto::Utxo::Proto;
use tw_utxo::compiler::{Compiler, StandardBitcoinContext};

#[test]
fn no_change_output() {
    let pubkey_hash = pubkey_hash_from_hex("e4c1ea86373d554b8f4efff2cfb0001ea19124d2");
    let input_script_pubkey = ScriptBuf::new_p2pkh(&pubkey_hash);

    let pubkey_hash = pubkey_hash_from_hex("5eaaa4f458f9158f86afcba08dd7448d27045e3d");
    let output_script_pubkey = ScriptBuf::new_p2pkh(&pubkey_hash);

    let pubkey_hash = pubkey_hash_from_hex("aabbccddeeff00112233445566778899aabbccdd");
    let change_output = ScriptBuf::new_p2pkh(&pubkey_hash);

    let txid = txid_rev("1e1cdc48aa990d7e154a161d5b5f1cad737742e97d2712ab188027bb42e6e47b");

    const INPUT_VALUE: u64 = 10_000;
    const OUTPUT_VALUE: u64 = 10_000 - FEE_VALUE;

    const WEIGHT_BASE: u64 = 2;
    const WEIGHT_VALUE: u64 = 341;
    const FEE_VALUE: u64 = WEIGHT_VALUE * WEIGHT_BASE; // 341 * 2

    let signing = Proto::SigningInput {
        version: 2,
        lock_time: Default::default(),
        inputs: vec![Proto::TxIn {
            txid: txid.into(),
            vout: 0,
            amount: INPUT_VALUE,
            sequence: u32::MAX,
            script_pubkey: input_script_pubkey.as_bytes().into(),
            sighash_type: Proto::SighashType::All,
            signing_method: Proto::SigningMethod::Legacy,
            weight_projection: 1,
            leaf_hash: Default::default(),
        }],
        outputs: vec![Proto::TxOut {
            value: OUTPUT_VALUE,
            script_pubkey: output_script_pubkey.as_bytes().into(),
        }],
        input_selector: Proto::InputSelector::UseAll,
        weight_base: WEIGHT_BASE,
        change_script_pubkey: change_output.as_bytes().into(),
        // We explicitly enable the creation of the change output.
        disable_change_output: false,
    };

    let output = Compiler::<StandardBitcoinContext>::preimage_hashes(signing);

    assert_eq!(output.sighashes.len(), 1);
    assert_eq!(output.inputs.len(), 1);
    // There's only one output, since the input amount is fully spent and
    // there's therefore no change.
    assert_eq!(output.outputs.len(), 1);
    assert_eq!(output.fee_projection, FEE_VALUE);
    assert_eq!(output.weight_projection, WEIGHT_VALUE);
}

#[test]
fn one_input_one_output() {
    let pubkey_hash = pubkey_hash_from_hex("e4c1ea86373d554b8f4efff2cfb0001ea19124d2");
    let input_script_pubkey = ScriptBuf::new_p2pkh(&pubkey_hash);

    let pubkey_hash = pubkey_hash_from_hex("5eaaa4f458f9158f86afcba08dd7448d27045e3d");
    let output_script_pubkey = ScriptBuf::new_p2pkh(&pubkey_hash);

    let pubkey_hash = pubkey_hash_from_hex("aabbccddeeff00112233445566778899aabbccdd");
    let change_output = ScriptBuf::new_p2pkh(&pubkey_hash);

    let txid = txid_rev("1e1cdc48aa990d7e154a161d5b5f1cad737742e97d2712ab188027bb42e6e47b");

    const INPUT_VALUE: u64 = 10_000;
    const OUTPUT_VALUE: u64 = 6_000;

    const WEIGHT_BASE: u64 = 2;
    const WEIGHT_VALUE: u64 = 341;
    const FEE_VALUE: u64 = WEIGHT_VALUE * WEIGHT_BASE; // 341 * 2

    let signing = Proto::SigningInput {
        version: 2,
        lock_time: Default::default(),
        inputs: vec![Proto::TxIn {
            txid: txid.into(),
            vout: 0,
            amount: INPUT_VALUE,
            sequence: u32::MAX,
            script_pubkey: input_script_pubkey.as_bytes().into(),
            sighash_type: Proto::SighashType::All,
            signing_method: Proto::SigningMethod::Legacy,
            weight_projection: 1,
            leaf_hash: Default::default(),
        }],
        outputs: vec![Proto::TxOut {
            value: OUTPUT_VALUE,
            script_pubkey: output_script_pubkey.as_bytes().into(),
        }],
        input_selector: Proto::InputSelector::UseAll,
        weight_base: WEIGHT_BASE,
        change_script_pubkey: change_output.as_bytes().into(),
        // We explicitly enable the creation of the change output.
        disable_change_output: false,
    };

    let output = Compiler::<StandardBitcoinContext>::preimage_hashes(signing);

    assert_eq!(output.sighashes.len(), 1);
    assert_eq!(output.inputs.len(), 1);
    // There are two outputs: the one defined above and the created change output.
    assert_eq!(output.outputs.len(), 2);
    assert_eq!(output.fee_projection, FEE_VALUE);
    assert_eq!(output.weight_projection, WEIGHT_VALUE);

    assert_eq!(
        output.outputs[1].value,
        INPUT_VALUE - OUTPUT_VALUE - FEE_VALUE
    );
    assert_eq!(output.outputs[1].script_pubkey, change_output.as_bytes());
}

#[test]
fn multiple_inputs_and_outputs() {
    let pubkey_hash = pubkey_hash_from_hex("e4c1ea86373d554b8f4efff2cfb0001ea19124d2");
    let input_script_pubkey = ScriptBuf::new_p2pkh(&pubkey_hash);

    let pubkey_hash = pubkey_hash_from_hex("5eaaa4f458f9158f86afcba08dd7448d27045e3d");
    let output_script_pubkey = ScriptBuf::new_p2pkh(&pubkey_hash);

    let pubkey_hash = pubkey_hash_from_hex("aabbccddeeff00112233445566778899aabbccdd");
    let change_output = ScriptBuf::new_p2pkh(&pubkey_hash);

    let txid = txid_rev("1e1cdc48aa990d7e154a161d5b5f1cad737742e97d2712ab188027bb42e6e47b");

    const INPUT_VALUE_1: u64 = 10_000;
    const INPUT_VALUE_2: u64 = 5_000;
    const OUTPUT_VALUE_1: u64 = 3_000;
    const OUTPUT_VALUE_2: u64 = 8_000;

    const WEIGHT_BASE: u64 = 2;
    const WEIGHT_VALUE: u64 = 642;
    const FEE_VALUE: u64 = WEIGHT_VALUE * WEIGHT_BASE; // 341 * 2

    let signing = Proto::SigningInput {
        version: 2,
        lock_time: Default::default(),
        inputs: vec![
            Proto::TxIn {
                txid: txid.clone().into(),
                vout: 0,
                amount: INPUT_VALUE_1,
                sequence: u32::MAX,
                script_pubkey: input_script_pubkey.as_bytes().into(),
                sighash_type: Proto::SighashType::All,
                signing_method: Proto::SigningMethod::Legacy,
                weight_projection: 1,
                leaf_hash: Default::default(),
            },
            Proto::TxIn {
                txid: txid.into(),
                vout: 0,
                amount: INPUT_VALUE_2,
                sequence: u32::MAX,
                script_pubkey: input_script_pubkey.as_bytes().into(),
                sighash_type: Proto::SighashType::All,
                signing_method: Proto::SigningMethod::Legacy,
                weight_projection: 1,
                leaf_hash: Default::default(),
            },
        ],
        outputs: vec![
            Proto::TxOut {
                value: OUTPUT_VALUE_1,
                script_pubkey: output_script_pubkey.as_bytes().into(),
            },
            Proto::TxOut {
                value: OUTPUT_VALUE_2,
                script_pubkey: output_script_pubkey.as_bytes().into(),
            },
        ],
        input_selector: Proto::InputSelector::UseAll,
        weight_base: WEIGHT_BASE,
        change_script_pubkey: change_output.as_bytes().into(),
        // We explicitly enable the creation of the change output.
        disable_change_output: false,
    };

    let output = Compiler::<StandardBitcoinContext>::preimage_hashes(signing);

    assert_eq!(output.sighashes.len(), 2);
    assert_eq!(output.inputs.len(), 2);
    assert_eq!(output.outputs.len(), 3);
    assert_eq!(output.fee_projection, FEE_VALUE);
    assert_eq!(output.weight_projection, WEIGHT_VALUE);

    assert_eq!(
        output.outputs[2].value,
        (INPUT_VALUE_1 + INPUT_VALUE_2) - (OUTPUT_VALUE_1 + OUTPUT_VALUE_2) - FEE_VALUE
    );
    assert_eq!(output.outputs[2].script_pubkey, change_output.as_bytes());
}
