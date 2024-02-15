use tw_encoding::hex;
use tw_keypair::ecdsa::secp256k1::PrivateKey;
use tw_keypair::traits::SigningKeyTrait;
// TODO: Consider using ecdsa directly.
use tw_keypair::tw::{PublicKey, PublicKeyType};
use tw_utxo::sighash::SighashBase;
use tw_utxo::sighash::SighashType;
use tw_utxo::sighash_computer::SighashComputer;
use tw_utxo::transaction::standard_transaction::builder::txid_from_str_and_rev;
use tw_utxo::transaction::standard_transaction::builder::OutputBuilder;
use tw_utxo::transaction::standard_transaction::builder::SpendingScriptBuilder;
use tw_utxo::transaction::standard_transaction::builder::TransactionBuilder;
use tw_utxo::transaction::standard_transaction::builder::UtxoBuilder;
use tw_utxo::transaction::transaction_fee::TransactionFee;

const SATS_PER_VBYTE: i64 = 20;

#[test]
fn build_tx_input_legacy_output_legacy() {
    let alice_private_key =
        hex::decode("56429688a1a6b00b90ccd22a0de0a376b6569d8684022ae92229a28478bfb657").unwrap();
    let alice_pubkey =
        hex::decode("036666dd712e05a487916384bfcd5973eb53e8038eccbbf97f7eed775b87389536").unwrap();
    let bob_pubkey =
        hex::decode("037ed9a436e11ec4947ac4b7823787e24ba73180f1edd2857bff19c9f4d62b65bf").unwrap();

    let alice_private_key = PrivateKey::try_from(alice_private_key.as_slice()).unwrap();
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

    let (tx, args) = TransactionBuilder::new()
        .push_input(utxo1, arg1)
        .push_output(output1)
        .build();

    // Compute the primage.
    let computer = SighashComputer::new(tx, args);
    let preimage = computer.preimage_tx().unwrap();

    // Sign the sighash.
    let sighash = preimage.into_h256_list().unwrap()[0];
    let sig = alice_private_key.sign(sighash).unwrap();

    // Build the claim
    let claim = SpendingScriptBuilder::new()
        .sighash_ty(SighashType::new(SighashBase::All))
        .p2pkh(sig, alice_pubkey)
        .unwrap();

    let tx = computer.compile(vec![claim]).unwrap();

    assert_eq!(tx.size(), 191);
    assert_eq!(tx.vsize(), 191); // No witness data
    assert_eq!(tx.weight(), 191 * 4);
    assert_eq!(tx.weight(), 764);
    assert_eq!(tx.fee(SATS_PER_VBYTE), 191 * SATS_PER_VBYTE);
    assert_eq!(tx.fee(SATS_PER_VBYTE), 3820);

    let encoded = hex::encode(tx.encode_out(), false);
    assert_eq!(encoded, "02000000017be4e642bb278018ab12277de9427773ad1c5f5b1d164a157e0d99aa48dc1c1e000000006a473044022078eda020d4b86fcb3af78ef919912e6d79b81164dbbb0b0b96da6ac58a2de4b102201a5fd8d48734d5a02371c4b5ee551a69dca3842edbf577d863cf8ae9fdbbd4590121036666dd712e05a487916384bfcd5973eb53e8038eccbbf97f7eed775b87389536ffffffff01c0aff629010000001976a9145eaaa4f458f9158f86afcba08dd7448d27045e3d88ac00000000");
}

#[test]
fn build_tx_input_legacy_output_segwit() {
    let alice_private_key =
        hex::decode("57a64865bce5d4855e99b1cce13327c46171434f2d72eeaf9da53ee075e7f90a").unwrap();
    let alice_pubkey =
        hex::decode("028d7dce6d72fb8f7af9566616c6436349c67ad379f2404dd66fe7085fe0fba28f").unwrap();
    let bob_pubkey =
        hex::decode("025a0af1510f0f24d40dd00d7c0e51605ca504bbc177c3e19b065f373a1efdd22f").unwrap();

    let alice_private_key = PrivateKey::try_from(alice_private_key.as_slice()).unwrap();
    let alice_pubkey = PublicKey::new(alice_pubkey, PublicKeyType::Secp256k1).unwrap();
    let bob_pubkey = PublicKey::new(bob_pubkey, PublicKeyType::Secp256k1).unwrap();

    let txid =
        txid_from_str_and_rev("181c84965c9ea86a5fac32fdbd5f73a21a7a9e749fb6ab97e273af2329f6b911")
            .unwrap();

    let (utxo1, arg1) = UtxoBuilder::new()
        .prev_txid(txid)
        .prev_index(0)
        .amount(50 * 100_000_000)
        .p2pkh(alice_pubkey.clone())
        .unwrap();

    let output1 = OutputBuilder::new()
        .amount(50 * 100_000_000 - 1_000_000)
        .p2wpkh(bob_pubkey)
        .unwrap();

    let (tx, args) = TransactionBuilder::new()
        .push_input(utxo1, arg1)
        .push_output(output1)
        .build();

    // Compute the primage.
    let computer = SighashComputer::new(tx, args);
    let preimage = computer.preimage_tx().unwrap();

    // Sign the sighash.
    let sighash = preimage.into_h256_list().unwrap()[0];
    let sig = alice_private_key.sign(sighash).unwrap();

    // Build the claim
    let claim = SpendingScriptBuilder::new()
        .sighash_ty(SighashType::new(SighashBase::All))
        .p2pkh(sig, alice_pubkey)
        .unwrap();

    let tx = computer.compile(vec![claim]).unwrap();

    assert_eq!(tx.size(), 189);
    assert_eq!(tx.vsize(), 189); // No witness data
    assert_eq!(tx.weight(), 189 * 4);
    assert_eq!(tx.weight(), 756);
    assert_eq!(tx.fee(SATS_PER_VBYTE), 189 * SATS_PER_VBYTE);
    assert_eq!(tx.fee(SATS_PER_VBYTE), 3780);

    let encoded = hex::encode(tx.encode_out(), false);
    assert_eq!(encoded, "020000000111b9f62923af73e297abb69f749e7a1aa2735fbdfd32ac5f6aa89e5c96841c18000000006b483045022100df9ed0b662b759e68b89a42e7144cddf787782a7129d4df05642dd825930e6e6022051a08f577f11cc7390684bbad2951a6374072253ffcf2468d14035ed0d8cd6490121028d7dce6d72fb8f7af9566616c6436349c67ad379f2404dd66fe7085fe0fba28fffffffff01c0aff629010000001600140d0e1cec6c2babe8badde5e9b3dea667da90036d00000000")
}

#[test]
fn build_tx_input_segwit_output_segwit() {
    let alice_pubkey =
        hex::decode("028d7dce6d72fb8f7af9566616c6436349c67ad379f2404dd66fe7085fe0fba28f").unwrap();
    let bob_private_key =
        hex::decode("05dead4689ec7d55de654771120866be83bf1b8e25c9a1b77fc58a336e1cd1a3").unwrap();
    let bob_pubkey =
        hex::decode("025a0af1510f0f24d40dd00d7c0e51605ca504bbc177c3e19b065f373a1efdd22f").unwrap();

    let alice_pubkey = PublicKey::new(alice_pubkey, PublicKeyType::Secp256k1).unwrap();
    let bob_private_key = PrivateKey::try_from(bob_private_key.as_slice()).unwrap();
    let bob_pubkey = PublicKey::new(bob_pubkey, PublicKeyType::Secp256k1).unwrap();

    let txid =
        txid_from_str_and_rev("858e450a1da44397bde05ca2f8a78510d74c623cc2f69736a8b3fbfadc161f6e")
            .unwrap();

    let (utxo1, arg1) = UtxoBuilder::new()
        .prev_txid(txid)
        .prev_index(0)
        .amount(50 * 100_000_000 - 1_000_000)
        .p2wpkh(bob_pubkey.clone())
        .unwrap();

    let output1 = OutputBuilder::new()
        .amount(50 * 100_000_000 - 1_000_000 - 1_000_000)
        .p2wpkh(alice_pubkey)
        .unwrap();

    let (tx, args) = TransactionBuilder::new()
        .push_input(utxo1, arg1)
        .push_output(output1)
        .build();

    // Compute the primage.
    let computer = SighashComputer::new(tx, args);
    let preimage = computer.preimage_tx().unwrap();

    // Sign the sighash.
    let sighash = preimage.into_h256_list().unwrap()[0];
    let sig = bob_private_key.sign(sighash).unwrap();

    // Build the claim
    // TODO: Consider using type safetly for calling the right method here?
    let claim = SpendingScriptBuilder::new()
        .sighash_ty(SighashType::new(SighashBase::All))
        .p2wpkh(sig, bob_pubkey)
        .unwrap();

    let tx = computer.compile(vec![claim]).unwrap();

    assert_eq!(tx.size(), 191);
    assert_eq!(tx.vsize(), 110); // Witness data discounted
    assert_eq!(tx.weight(), 110 * 4);
    assert_eq!(tx.weight(), 440);
    assert_eq!(tx.fee(SATS_PER_VBYTE), 110 * SATS_PER_VBYTE);
    assert_eq!(tx.fee(SATS_PER_VBYTE), 2200);

    let encoded = hex::encode(tx.encode_out(), false);
    assert_eq!(encoded, "020000000001016e1f16dcfafbb3a83697f6c23c624cd71085a7f8a25ce0bd9743a41d0a458e850000000000ffffffff01806de7290100000016001460cda7b50f14c152d7401c28ae773c698db9237302483045022100a9b517de5a5e036d7133df499b5b751db6f9a01576a6c5dc38229ec08b6c45cd02200e42c9f8c707c9bf0ceab4f739ec8d683dc1f1f29e195a8da9bc183584d624a60121025a0af1510f0f24d40dd00d7c0e51605ca504bbc177c3e19b065f373a1efdd22f00000000")
}
