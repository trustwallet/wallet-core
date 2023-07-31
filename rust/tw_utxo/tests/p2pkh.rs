use bitcoin::{PubkeyHash, ScriptBuf};
use secp256k1::hashes::Hash;
use std::borrow::Cow;
use tw_encoding::hex;
use tw_proto::Utxo::Proto;
use tw_utxo::{Signer, StandardBitcoinContext};

#[test]
fn sign_p2pkh_emtpy() {
    let private_key =
        hex::decode("56429688a1a6b00b90ccd22a0de0a376b6569d8684022ae92229a28478bfb657").unwrap();

    let input = Proto::SigningInput {
        private_key: Cow::Borrowed(&private_key),
        version: 2,
        inputs: vec![],
        outputs: vec![],
        lock_time: Proto::mod_SigningInput::OneOflock_time::None,
    };

    let output = Signer::<StandardBitcoinContext>::sign_proto(input).unwrap();

    let sigs = output.signatures;
    //assert_eq!(s)
    println!("{:?}", sigs);
}

#[test]
fn sign_p2pkh_one_in_one_out() {
    let private_key =
        hex::decode("56429688a1a6b00b90ccd22a0de0a376b6569d8684022ae92229a28478bfb657").unwrap();

    let pubkey_hash = PubkeyHash::from_byte_array(
        hex::decode("e4c1ea86373d554b8f4efff2cfb0001ea19124d2")
            .unwrap()
            .try_into()
            .unwrap(),
    );

    let script_pubkey = ScriptBuf::new_p2pkh(&pubkey_hash);

    let txid =
        hex::decode("7be4e642bb278018ab12277de9427773ad1c5f5b1d164a157e0d99aa48dc1c1e").unwrap();
    let input = Proto::TxIn {
        private_key: Cow::Borrowed(&[]),
        txid: txid.into(),
        vout: 0,
        sequence: u32::MAX,
        sighash: Proto::SighashType::All,
        signing_method: Proto::mod_TxIn::OneOfsigning_method::legacy(Proto::mod_TxIn::Legacy {
            script_pubkey: script_pubkey.as_bytes().into(),
        }),
    };

    let pubkey_hash = PubkeyHash::from_byte_array(
        hex::decode("5eaaa4f458f9158f86afcba08dd7448d27045e3d")
            .unwrap()
            .try_into()
            .unwrap(),
    );

    let script_pubkey = ScriptBuf::new_p2pkh(&pubkey_hash);

    let output = Proto::TxOut {
        value: 50 * 100_000_000 - 1_000_000,
        script_pubkey: script_pubkey.as_bytes().into(),
    };

    let signing = Proto::SigningInput {
        private_key: Cow::Borrowed(&private_key),
        version: 2,
        inputs: vec![input],
        outputs: vec![output],
        lock_time: Proto::mod_SigningInput::OneOflock_time::None,
    };

    let output = Signer::<StandardBitcoinContext>::sign_proto(signing).unwrap();

    let sigs = output.signatures;
    assert_eq!(sigs.len(), 1);
	// TODO: Note that there should be a SIGHASH type indicator at the end (73 bytes in total).
    assert_eq!(hex::encode(sigs[0].as_ref(), false), "3044022078eda020d4b86fcb3af78ef919912e6d79b81164dbbb0b0b96da6ac58a2de4b102201a5fd8d48734d5a02371c4b5ee551a69dca3842edbf577d863cf8ae9fdbbd459");
}
