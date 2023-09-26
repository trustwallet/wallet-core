mod common;
use common::{pubkey_hash_from_hex, txid_rev, untweaked_pubkey};

use bitcoin::ScriptBuf;
use tw_encoding::hex;
use tw_proto::Utxo::Proto;
use tw_utxo::compiler::{Compiler, StandardBitcoinContext};

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
        "c914fd08efdcc7f8007c75c39ab47e1ee736a6ce1e6363250fe88cda8fca04d1"
    );
}
