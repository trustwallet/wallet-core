mod common;
use common::pubkey_hash_from_hex;

use bitcoin::{PubkeyHash, ScriptBuf};
use secp256k1::hashes::Hash;
use tw_encoding::hex;
use tw_proto::Utxo::Proto;
use tw_utxo::builder::{SigningInputBuilder, TxInBuilder, TxOutBuilder};
use tw_utxo::compiler::{Compiler, StandardBitcoinContext};

use crate::common::{txid_rev, untweaked_pubkey};

#[test]
fn sighash_input_p2pkh_output_p2tr_key_spend() {
    let pubkey_hash = pubkey_hash_from_hex("a0cd6d6e2f9804351ba4b722b708bc2fd3229a5a");
    let input_script_pubkey = ScriptBuf::new_p2pkh(&pubkey_hash);

    let untweaked_pubkey =
        untweaked_pubkey("02c0938cf377023dfde55e9c96b3cff4ca8894fb6b5d2009006bd43c0bff69cac9");
    let output_script_pubkey =
        // Merkle root of `None` is interpreted as P2TR key-spend.
        ScriptBuf::new_v1_p2tr(&secp256k1::Secp256k1::new(), untweaked_pubkey, None);

    let txid = txid_rev("c50563913e5a838f937c94232f5a8fc74e58b629fae41dfdffcc9a70f833b53a");

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

    let output = Compiler::<StandardBitcoinContext>::preimage_hashes(signing);

    let hashes = output.sighashes;
    assert_eq!(hashes.len(), 1);
    assert_eq!(
        hex::encode(hashes[0].sighash.as_ref(), false),
        "c914fd08efdcc7f8007c75c39ab47e1ee736a6ce1e6363250fe88cda8fca04d1"
    );
}

#[test]
fn sign_p2pkh_two_in_one_out() {}
