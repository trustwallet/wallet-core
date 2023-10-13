mod common;
use common::{pubkey_hash_from_hex, txid_rev, witness_pubkey_hash};

use bitcoin::ScriptBuf;
use tw_encoding::hex;
use tw_proto::Utxo::Proto;
use tw_utxo::compiler::{Compiler, StandardBitcoinContext};

#[test]
fn sighash_input_p2pkh_output_p2wpkh() {
    let pubkey_hash = pubkey_hash_from_hex("60cda7b50f14c152d7401c28ae773c698db92373");
    let input_script_pubkey = ScriptBuf::new_p2pkh(&pubkey_hash);

    let wpubkey_hash = witness_pubkey_hash("0d0e1cec6c2babe8badde5e9b3dea667da90036d");
    let output_script_pubkey = ScriptBuf::new_v0_p2wpkh(&wpubkey_hash);

    let txid = txid_rev("181c84965c9ea86a5fac32fdbd5f73a21a7a9e749fb6ab97e273af2329f6b911");

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
        "c4963ecd6c08be4c9dd66416349084a5b54318b3802370451d580210bc883463"
    );
}

#[test]
fn sighash_input_p2wpkh_output_p2wpkh() {
    let wpubkey_hash = witness_pubkey_hash("0d0e1cec6c2babe8badde5e9b3dea667da90036d");
    let input_script_pubkey = ScriptBuf::new_v0_p2wpkh(&wpubkey_hash)
        .p2wpkh_script_code()
        .unwrap();

    let wpubkey_hash = witness_pubkey_hash("60cda7b50f14c152d7401c28ae773c698db92373");
    let output_script_pubkey = ScriptBuf::new_v0_p2wpkh(&wpubkey_hash);

    let txid = txid_rev("858e450a1da44397bde05ca2f8a78510d74c623cc2f69736a8b3fbfadc161f6e");

    let signing = Proto::SigningInput {
        version: 2,
        lock_time: Default::default(),
        inputs: vec![Proto::TxIn {
            txid: txid.into(),
            vout: 0,
            value: 50 * 100_000_000 - 1_000_000,
            sequence: u32::MAX,
            script_pubkey: input_script_pubkey.as_bytes().into(),
            sighash_type: Proto::SighashType::All,
            signing_method: Proto::SigningMethod::Segwit,
            weight_estimate: 1,
            leaf_hash: Default::default(),
        }],
        outputs: vec![Proto::TxOut {
            value: 50 * 100_000_000 - 1_000_000 * 2,
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
        "6900ebbef74c938ec2310df10cd520b5e7c82c0fe1bb68c62c8fae7bf54e2092"
    );
}
