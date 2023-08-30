mod common;
use common::{pubkey_hash_from_hex, txid_rev};

use bitcoin::ScriptBuf;
use tw_proto::Utxo::Proto;
use tw_utxo::compiler::{Compiler, StandardBitcoinContext};

#[test]
fn zero_sequence_disabled() {
    let pubkey_hash = pubkey_hash_from_hex("e4c1ea86373d554b8f4efff2cfb0001ea19124d2");
    let input_script_pubkey = ScriptBuf::new_p2pkh(&pubkey_hash);

    let pubkey_hash = pubkey_hash_from_hex("5eaaa4f458f9158f86afcba08dd7448d27045e3d");
    let output_script_pubkey = ScriptBuf::new_p2pkh(&pubkey_hash);

    let txid = txid_rev("1e1cdc48aa990d7e154a161d5b5f1cad737742e97d2712ab188027bb42e6e47b");

    let tx1 = Proto::TxIn {
        txid: txid.into(),
        vout: 0,
        value: 1_000,
        // We use a sequence of zero,
        sequence: 0,
        // ...but it's not explicitly enabled.
        sequence_enable_zero: false,
        p2sh_script_sig: Default::default(),
        script_pubkey: input_script_pubkey.as_bytes().into(),
        sighash_type: Proto::SighashType::All,
        signing_method: Proto::SigningMethod::Legacy,
        weight_estimate: 1,
        leaf_hash: Default::default(),
    };

    let out1 = Proto::TxOut {
        value: 500,
        script_pubkey: output_script_pubkey.as_bytes().into(),
    };

    let signing = Proto::SigningInput {
        version: 2,
        lock_time: Default::default(),
        inputs: vec![tx1],
        outputs: vec![out1],
        input_selector: Proto::InputSelector::UseAll,
        weight_base: 1,
        change_script_pubkey: Default::default(),
        disable_change_output: true,
    };

    let output = Compiler::<StandardBitcoinContext>::preimage_hashes(signing);
    assert_eq!(output.error, Proto::Error::Error_zero_sequence_not_enabled);
    assert_eq!(output.sighashes.len(), 0);
    assert_eq!(output.inputs.len(), 0);
    assert_eq!(output.outputs.len(), 0);
    assert_eq!(output.weight_estimate, 0);
    assert_eq!(output.fee_estimate, 0);
}
