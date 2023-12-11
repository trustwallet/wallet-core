mod common;
use common::{pubkey_hash_from_hex, txid_rev};

use bitcoin::ScriptBuf;
use tw_encoding::hex;
use tw_proto::Utxo::Proto;
use tw_utxo::compiler::{Compiler, StandardBitcoinContext};

#[test]
fn sighash_input_p2pkh_output_p2pkh() {
    let pubkey_hash = pubkey_hash_from_hex("e4c1ea86373d554b8f4efff2cfb0001ea19124d2");
    let input_script_pubkey = ScriptBuf::new_p2pkh(&pubkey_hash);

    let pubkey_hash = pubkey_hash_from_hex("5eaaa4f458f9158f86afcba08dd7448d27045e3d");
    let output_script_pubkey = ScriptBuf::new_p2pkh(&pubkey_hash);

    let txid = txid_rev("1e1cdc48aa990d7e154a161d5b5f1cad737742e97d2712ab188027bb42e6e47b");

    let signing = Proto::SigningInput {
        version: 2,
        lock_time: Default::default(),
        inputs: vec![Proto::TxIn {
            txid: txid.into(),
            vout: 0,
            // Amount is not part of sighash for `Legacy`.
            value: u64::MAX,
            sequence: u32::MAX,
            script_pubkey: input_script_pubkey.as_bytes().into(),
            sighash_type: Proto::SighashType::All,
            signing_method: Proto::SigningMethod::Legacy,
            weight_estimate: 1,
            leaf_hash: Default::default(),
        }],
        outputs: vec![Proto::TxOut {
            value: 50 * 100_000_000 - 1_000_000,
            script_pubkey: output_script_pubkey.as_bytes().into(),
        }],
        input_selector: Proto::InputSelector::UseAll,
        weight_base: 1,
        change_script_pubkey: Default::default(),
        disable_change_output: true,
    };

    let output = Compiler::<StandardBitcoinContext>::preimage_hashes(signing);
    assert_eq!(output.error, Proto::Error::OK);

    let hashes = output.sighashes;
    assert_eq!(hashes.len(), 1);
    assert_eq!(
        hex::encode(hashes[0].sighash.as_ref(), false),
        "6a0e072da66b141fdb448323d54765cafcaf084a06d2fa13c8aed0c694e50d18"
    );
}
