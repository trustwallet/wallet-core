use bitcoin::ScriptBuf;
use tw_encoding::hex;
use tw_keypair::tw::{Curve, PrivateKey};
use tw_misc::traits::ToBytesVec;
use tw_utxo::{
    encode::{stream::Stream, Encodable},
    script::{Script, Witness},
    sighash_computer::{ClaimingData, SighashComputer, TxSigningArgs, UtxoToSign},
    signing_mode::SigningMethod,
    transaction::{
        standard_transaction::{Transaction, TransactionInput, TransactionOutput},
        transaction_parts::OutPoint,
    },
};

#[test]
#[ignore]
// NOTE: This is just a playground for now... It's not a real test.
fn legacy_sighash_playground() {
    let alice_private_key =
        hex::decode("56429688a1a6b00b90ccd22a0de0a376b6569d8684022ae92229a28478bfb657").unwrap();
    let alice_pubkey =
        hex::decode("036666dd712e05a487916384bfcd5973eb53e8038eccbbf97f7eed775b87389536").unwrap();
    let _bob_private_key =
        hex::decode("b7da1ec42b19085fe09fec54b9d9eacd998ae4e6d2ad472be38d8393391b9ead").unwrap();
    let bob_pubkey =
        hex::decode("037ed9a436e11ec4947ac4b7823787e24ba73180f1edd2857bff19c9f4d62b65bf").unwrap();

    let mut tx = Transaction {
        version: 2,
        inputs: Vec::new(),
        outputs: Vec::new(),
        locktime: 0,
    };

    let script_pubkey = {
        let pubkey = bitcoin::PublicKey::from_slice(&alice_pubkey).unwrap();
        let script_pubkey = ScriptBuf::new_p2pkh(&pubkey.pubkey_hash());

        Script::from(script_pubkey.to_vec())
    };

    let utx_arg = UtxoToSign {
        script_pubkey,
        signing_method: SigningMethod::Legacy,
        amount: 50 * 100_000_000,
    };

    let utxo_args = TxSigningArgs {
        utxos_to_sign: vec![utx_arg],
        ..Default::default()
    };

    // Prepare TX input
    // TODO: The API should not mandate this, all context should be in UtxoToSign.

    let txid: Vec<u8> =
        hex::decode("1e1cdc48aa990d7e154a161d5b5f1cad737742e97d2712ab188027bb42e6e47b")
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
        let pubkey = bitcoin::PublicKey::from_slice(&bob_pubkey).unwrap();

        let script_buf = ScriptBuf::new_p2pkh(&pubkey.pubkey_hash());
        Script::from(script_buf.to_vec())
    };

    let output = TransactionOutput {
        value: 50 * 100_000_000 - 1_000_000,
        script_pubkey,
    };

    tx.outputs.push(output);

    // Sign preimages

    let mut signer = SighashComputer::new(tx, utxo_args);

    let preimage = &signer.preimage_tx().unwrap().sighashes[0];
    dbg!(preimage);

    // >> Prepare signature.
    let private_key = PrivateKey::new(alice_private_key).unwrap();
    let sig = private_key
        .sign(&preimage.sighash, Curve::Secp256k1)
        .unwrap();

    // >> Prepare script_sig.
    let script_sig = {
        let pubkey = bitcoin::PublicKey::from_slice(&alice_pubkey).unwrap();

        let sig = bitcoin::ecdsa::Signature {
            // Note, we're skipping the recovery byte here.
            sig: bitcoin::secp256k1::ecdsa::Signature::from_compact(&sig[..64]).unwrap(),
            hash_ty: bitcoin::sighash::EcdsaSighashType::All,
        };

        let script_buf = ScriptBuf::builder()
            .push_slice(&sig.serialize())
            .push_key(&pubkey)
            .into_script();

        Script::from(script_buf.to_vec())
    };

    //let utxo = signer.transaction_to_sign.inputs.get_mut(0).unwrap();
    //utxo.script_sig = script_sig;

    let claims = vec![ClaimingData {
        script_sig,
        witness: Witness::new(),
    }];

    signer.compile(claims).unwrap();

    let tx = signer.into_transaction();

    let mut stream = Stream::new();
    tx.encode(&mut stream);

    let encoded = hex::encode(stream.out(), false);
    dbg!(encoded);
}
