use bitcoin::{PubkeyHash, ScriptBuf};
use secp256k1::hashes::Hash;
use tw_encoding::hex;
use tw_proto::Utxo::Proto;
use tw_utxo::builder::{SigningInputBuilder, TxInBuilder, TxOutBuilder};
use tw_utxo::{Signer, StandardBitcoinContext};

#[test]
fn sign_p2pkh_emtpy() {
    let signing = Proto::SigningInput {
        version: 2,
        inputs: vec![],
        outputs: vec![],
        lock_time: Proto::mod_SigningInput::OneOflock_time::None,
    };

    let output = Signer::<StandardBitcoinContext>::sign_proto(signing).unwrap();

    let hashes = output.sighashes;
    assert!(hashes.is_empty());
}

#[test]
fn sign_p2pkh_one_in_one_out() {
    let pubkey_hash = PubkeyHash::from_byte_array(
        hex::decode("e4c1ea86373d554b8f4efff2cfb0001ea19124d2")
            .unwrap()
            .try_into()
            .unwrap(),
    );

    let input_script_pubkey = ScriptBuf::new_p2pkh(&pubkey_hash);

    let txid =
        hex::decode("7be4e642bb278018ab12277de9427773ad1c5f5b1d164a157e0d99aa48dc1c1e").unwrap();

    let pubkey_hash = PubkeyHash::from_byte_array(
        hex::decode("5eaaa4f458f9158f86afcba08dd7448d27045e3d")
            .unwrap()
            .try_into()
            .unwrap(),
    );

    let output_script_pubkey = ScriptBuf::new_p2pkh(&pubkey_hash);

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

    let output = Signer::<StandardBitcoinContext>::sign_proto(signing).unwrap();

    let hashes = output.sighashes;
    assert_eq!(hashes.len(), 1);
    assert_eq!(
        hex::encode(hashes[0].as_ref(), false),
        "6a0e072da66b141fdb448323d54765cafcaf084a06d2fa13c8aed0c694e50d18"
    );
}
