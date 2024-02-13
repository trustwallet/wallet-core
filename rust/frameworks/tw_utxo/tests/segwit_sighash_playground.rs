use bitcoin::ScriptBuf;
use tw_encoding::hex;
use tw_keypair::tw::{Curve, PrivateKey};
use tw_memory::Data;
use tw_misc::traits::ToBytesVec;
use tw_utxo::{
    encode::{stream::Stream, Encodable},
    script::{Script, Witness},
    signer::{ClaimingData, TransactionSigner, TxSigningArgs, UtxoToSign},
    signing_mode::SigningMethod,
    transaction::{
        standard_transaction::{Transaction, TransactionInput, TransactionOutput},
        transaction_parts::OutPoint,
    },
};

#[test]
#[ignore]
// NOTE: This is just a playground for now... It's not a real test.
fn segwit_sighash_playground() {
    let alice_pubkey =
        hex::decode("028d7dce6d72fb8f7af9566616c6436349c67ad379f2404dd66fe7085fe0fba28f").unwrap();
    let bob_private_key =
        hex::decode("05dead4689ec7d55de654771120866be83bf1b8e25c9a1b77fc58a336e1cd1a3").unwrap();
    let bob_pubkey =
        hex::decode("025a0af1510f0f24d40dd00d7c0e51605ca504bbc177c3e19b065f373a1efdd22f").unwrap();

    let mut tx = Transaction {
        version: 2,
        inputs: Vec::new(),
        outputs: Vec::new(),
        locktime: 0,
    };

    let script_pubkey = {
        let pubkey = bitcoin::PublicKey::from_slice(&bob_pubkey).unwrap();
        let script_pubkey = ScriptBuf::new_v0_p2wpkh(&pubkey.wpubkey_hash().unwrap())
            // TODO: It's IMPORTANT that we replicate this part:
            .p2wpkh_script_code()
            .unwrap();

        Script::from(script_pubkey.to_vec())
    };

    let utx_arg = UtxoToSign {
        script_pubkey,
        signing_method: SigningMethod::Segwit,
        amount: 50 * 100_000_000 - 1_000_000,
    };

    let utxo_args = TxSigningArgs {
        utxos_to_sign: vec![utx_arg],
        ..Default::default()
    };

    // Prepare TX input
    // TODO: The API should not mandate this, all context should be in UtxoToSign.

    let txid: Vec<u8> =
        hex::decode("858e450a1da44397bde05ca2f8a78510d74c623cc2f69736a8b3fbfadc161f6e")
            .unwrap()
            .into_iter()
            .rev()
            .collect();

    let txid: [u8; 32] = txid.try_into().unwrap();

    let utxo = TransactionInput {
        previous_output: OutPoint {
            hash: txid.into(),
            index: 0,
        },
        sequence: u32::MAX,
        script_sig: Script::new(),
        witness: Witness::new(),
    };

    tx.inputs.push(utxo);

    // Prepare TX output

    let script_pubkey = {
        let pubkey = bitcoin::PublicKey::from_slice(&alice_pubkey).unwrap();

        let script_buf = ScriptBuf::new_v0_p2wpkh(&pubkey.wpubkey_hash().unwrap());
        Script::from(script_buf.to_vec())
    };

    let output = TransactionOutput {
        value: 50 * 100_000_000 - 1_000_000 - 1_000_000,
        script_pubkey,
    };

    tx.outputs.push(output);

    // Sign preimages

    let mut signer = TransactionSigner::new(tx, utxo_args);

    let preimage = &signer.preimage_tx().unwrap().sighashes[0];
    dbg!(preimage);

    // >> Prepare signature.
    let private_key = PrivateKey::new(bob_private_key).unwrap();
    let sig = private_key
        .sign(&preimage.sighash, Curve::Secp256k1)
        .unwrap();

    // >> Prepare witness.
    let witness = {
        let pubkey = bitcoin::PublicKey::from_slice(&bob_pubkey).unwrap();

        let sig = bitcoin::ecdsa::Signature {
            // Note, we're skipping the recovery byte here.
            sig: bitcoin::secp256k1::ecdsa::Signature::from_compact(&sig[..64]).unwrap(),
            hash_ty: bitcoin::sighash::EcdsaSighashType::All,
        };

        let mut w = bitcoin::Witness::new();
        w.push(sig.serialize());
        w.push(pubkey.to_bytes());

        Witness::from(w.to_vec())
    };

    //let utxo = signer.transaction_to_sign.inputs.get_mut(0).unwrap();
    //utxo.script_sig = script_sig;

    let claims = vec![ClaimingData {
        script_sig: Script::new(),
        witness,
    }];

    signer.compile(claims).unwrap();

    let tx = signer.into_transaction();

    let mut stream = Stream::new();
    tx.encode(&mut stream);

    let encoded = hex::encode(stream.out(), false);
    dbg!(encoded);
}
