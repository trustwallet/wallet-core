mod common;
use common::pubkey_hash_from_hex;

use bitcoin::{PubkeyHash, ScriptBuf};
use secp256k1::hashes::Hash;
use tw_encoding::hex;
use tw_proto::Utxo::Proto;
use tw_utxo::builder::{SigningInputBuilder, TxInBuilder, TxOutBuilder};
use tw_utxo::compiler::{Compiler, StandardBitcoinContext};

use crate::common::{txid_rev, witness_pubkey_hash};

#[test]
fn sighash_input_p2pkh_output_p2wpkh() {
    let pubkey_hash = pubkey_hash_from_hex("60cda7b50f14c152d7401c28ae773c698db92373");
    let input_script_pubkey = ScriptBuf::new_p2pkh(&pubkey_hash);

    let wpubkey_hash = witness_pubkey_hash("0d0e1cec6c2babe8badde5e9b3dea667da90036d");
    let output_script_pubkey = ScriptBuf::new_v0_p2wpkh(&wpubkey_hash);

    let txid = txid_rev("181c84965c9ea86a5fac32fdbd5f73a21a7a9e749fb6ab97e273af2329f6b911");

    let signing = SigningInputBuilder::new()
        .version(2)
        .input(|| {
            TxInBuilder::new()
                .txid(&txid)
                .vout(0)
                .legacy_method()
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

    let output = Compiler::<StandardBitcoinContext>::preimage_hashes(signing);

    let hashes = output.sighashes;
    assert_eq!(hashes.len(), 1);
    assert_eq!(
        hex::encode(hashes[0].as_ref(), false),
        "c4963ecd6c08be4c9dd66416349084a5b54318b3802370451d580210bc883463"
    );
}

#[test]
fn sign_p2pkh_two_in_one_out() {}
