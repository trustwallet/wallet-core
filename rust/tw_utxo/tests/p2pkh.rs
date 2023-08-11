mod common;
use common::pubkey_hash_from_hex;

use bitcoin::{PubkeyHash, ScriptBuf};
use secp256k1::hashes::Hash;
use tw_encoding::hex;
use tw_proto::Utxo::Proto;
use tw_utxo::builder::{SigningInputBuilder, TxInBuilder, TxOutBuilder};
use tw_utxo::compiler::{Compiler, StandardBitcoinContext};

use crate::common::txid_rev;

#[test]
fn sighash_emtpy() {
    let signing = Proto::SigningInput {
        version: 2,
        inputs: vec![],
        outputs: vec![],
        lock_time: Proto::mod_SigningInput::OneOflock_time::None,
    };

    let output = Compiler::<StandardBitcoinContext>::preimage_hashes(&signing);

    let hashes = output.sighashes;
    assert!(hashes.is_empty());
}

#[test]
fn sighash_input_p2pkh_output_p2pkh() {
    let pubkey_hash = pubkey_hash_from_hex("e4c1ea86373d554b8f4efff2cfb0001ea19124d2");
    let input_script_pubkey = ScriptBuf::new_p2pkh(&pubkey_hash);

    let pubkey_hash = pubkey_hash_from_hex("5eaaa4f458f9158f86afcba08dd7448d27045e3d");
    let output_script_pubkey = ScriptBuf::new_p2pkh(&pubkey_hash);

    let txid = txid_rev("1e1cdc48aa990d7e154a161d5b5f1cad737742e97d2712ab188027bb42e6e47b");

    let signing = SigningInputBuilder::new()
        .version(2)
        .input(|| {
            TxInBuilder::new()
                .txid(&txid)
                .vout(0)
                .value(0)
                .spending_condition(input_script_pubkey.as_bytes())
                .build()
        })
        .unwrap()
        .output(|| {
            TxOutBuilder::new()
                .value(50 * 100_000_000 - 1_000_000)
                .spending_condition(output_script_pubkey.as_bytes())
                .build()
        })
        .unwrap()
        .build()
        .unwrap();

    let output = Compiler::<StandardBitcoinContext>::preimage_hashes(&signing);

    let hashes = output.sighashes;
    assert_eq!(hashes.len(), 1);
    assert_eq!(
        hex::encode(hashes[0].as_ref(), false),
        "6a0e072da66b141fdb448323d54765cafcaf084a06d2fa13c8aed0c694e50d18"
    );
}

#[test]
fn sign_p2pkh_two_in_one_out() {}
