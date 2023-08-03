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
fn sighash_input_p2pkh_output_p2tr_key_spend() {
    let pubkey_hash = pubkey_hash_from_hex("");
    let input_script_pubkey = ScriptBuf::new_p2pkh(&pubkey_hash);

    let pubkey_hash = pubkey_hash_from_hex("");
    let output_script_pubkey = ScriptBuf::new_p2pkh(&pubkey_hash);

    let txid = txid_rev("");

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
        ""
    );
}

#[test]
fn sign_p2pkh_two_in_one_out() {}
