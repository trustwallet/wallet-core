use bitcoin::ScriptBuf;
use tw_encoding::hex;
use tw_hash::{H256, H264, H520};
use tw_keypair::{
    traits::SigningKeyTrait,
    tw::{Curve, PrivateKey, PublicKey, PublicKeyType},
};
use tw_misc::traits::ToBytesVec;
use tw_utxo::{
    encode::{stream::Stream, Encodable},
    script::{standard_script::claims, Script, Witness},
    sighash::{BitcoinEcdsaSignature, SighashBase, SighashType},
    signer::{ClaimingData, TransactionSigner, TxSigningArgs, UtxoToSign},
    signing_mode::SigningMethod,
    transaction::{
        standard_transaction::{
            builder::{txid_from_str_and_rev, OutputBuilder, TransactionBuilder, UtxoBuilder},
            Transaction, TransactionInput, TransactionOutput,
        },
        transaction_parts::OutPoint,
    },
};

#[test]
fn build_legacy_tx() {
    let alice_private_key =
        hex::decode("56429688a1a6b00b90ccd22a0de0a376b6569d8684022ae92229a28478bfb657").unwrap();
    let alice_pubkey =
        hex::decode("036666dd712e05a487916384bfcd5973eb53e8038eccbbf97f7eed775b87389536").unwrap();
    let bob_pubkey =
        hex::decode("037ed9a436e11ec4947ac4b7823787e24ba73180f1edd2857bff19c9f4d62b65bf").unwrap();

    let alice_private_key =
        tw_keypair::ecdsa::secp256k1::PrivateKey::try_from(alice_private_key.as_slice()).unwrap();
    let alice_pubkey = PublicKey::new(alice_pubkey, PublicKeyType::Secp256k1).unwrap();
    let bob_pubkey = PublicKey::new(bob_pubkey, PublicKeyType::Secp256k1).unwrap();

    let txid =
        txid_from_str_and_rev("1e1cdc48aa990d7e154a161d5b5f1cad737742e97d2712ab188027bb42e6e47b")
            .unwrap();

    let (utxo1, arg1) = UtxoBuilder::new()
        .prev_txid(txid)
        .prev_index(0)
        .amount(50 * 100_000_000)
        .p2pkh(alice_pubkey.clone())
        .unwrap();

    let output1 = OutputBuilder::new()
        .amount(50 * 100_000_000 - 1_000_000)
        .p2pkh(bob_pubkey)
        .unwrap();

    // TODO: Kind of ugly, adjust this.
    let (tx, args) = TransactionBuilder::new()
        .push_input(utxo1, arg1)
        .push_output(output1)
        .build();

    let mut signer = TransactionSigner::new(tx, args);
    let preimage = signer.preimage_tx().unwrap();
    let preimage = preimage.sighashes[0].clone();

    //let sig = alice_private_key.sign(&preimage.sighash, Curve::Secp256k1).unwrap();
    let sighash: H256 = preimage.sighash.as_slice().try_into().unwrap();
    let der_sig = alice_private_key.sign(sighash).unwrap().to_der().unwrap();
    let btc_sig = BitcoinEcdsaSignature::new(der_sig, SighashType::new(SighashBase::All)).unwrap();

    let hpubkey: H264 = alice_pubkey.to_bytes().as_slice().try_into().unwrap();
    let script_sig = claims::new_p2pkh(&btc_sig.serialize(), &hpubkey);

    let claim = ClaimingData {
        script_sig,
        witness: Witness::default(),
    };

    signer.compile(vec![claim]).unwrap();
    let tx = signer.into_transaction();

    let encoded = hex::encode(tx.encode_out(), false);
    assert_eq!(encoded, "02000000017be4e642bb278018ab12277de9427773ad1c5f5b1d164a157e0d99aa48dc1c1e000000006a473044022078eda020d4b86fcb3af78ef919912e6d79b81164dbbb0b0b96da6ac58a2de4b102201a5fd8d48734d5a02371c4b5ee551a69dca3842edbf577d863cf8ae9fdbbd4590121036666dd712e05a487916384bfcd5973eb53e8038eccbbf97f7eed775b87389536ffffffff01c0aff629010000001976a9145eaaa4f458f9158f86afcba08dd7448d27045e3d88ac00000000")
}
