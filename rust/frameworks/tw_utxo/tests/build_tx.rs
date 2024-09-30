use tw_encoding::hex;

use tw_keypair::ecdsa::secp256k1::PrivateKey;
use tw_keypair::traits::SigningKeyTrait;
use tw_keypair::{ecdsa, schnorr};
use tw_misc::traits::ToBytesVec;
use tw_utxo::modules::fee_estimator::FeeEstimator;
use tw_utxo::modules::sighash_computer::SighashComputer;
use tw_utxo::modules::tx_compiler::TxCompiler;
use tw_utxo::sighash::SighashType;

use tw_utxo::transaction::standard_transaction::builder::txid_from_str_and_rev;
use tw_utxo::transaction::standard_transaction::builder::OutputBuilder;
use tw_utxo::transaction::standard_transaction::builder::TransactionBuilder;
use tw_utxo::transaction::standard_transaction::builder::UtxoBuilder;
use tw_utxo::transaction::transaction_interface::TransactionInterface;

const SATS_PER_VBYTE: i64 = 20;

#[track_caller]
fn verify_fee<Transaction: TransactionInterface>(
    tx: &Transaction,
    fee_per_vbyte: i64,
    expected: i64,
) {
    let actual = FeeEstimator::estimate_fee(tx, fee_per_vbyte).unwrap();
    assert_eq!(actual, expected);
}

#[test]
fn build_tx_input_legacy_output_legacy() {
    let alice_private_key =
        hex::decode("56429688a1a6b00b90ccd22a0de0a376b6569d8684022ae92229a28478bfb657").unwrap();
    let alice_pubkey =
        hex::decode("036666dd712e05a487916384bfcd5973eb53e8038eccbbf97f7eed775b87389536").unwrap();
    let bob_pubkey =
        hex::decode("037ed9a436e11ec4947ac4b7823787e24ba73180f1edd2857bff19c9f4d62b65bf").unwrap();

    let alice_private_key = PrivateKey::try_from(alice_private_key.as_slice()).unwrap();
    let alice_ecdsa_pubkey =
        ecdsa::secp256k1::PublicKey::try_from(alice_pubkey.as_slice()).unwrap();
    let bob_ecdsa_pubkey = ecdsa::secp256k1::PublicKey::try_from(bob_pubkey.as_slice()).unwrap();

    let txid =
        txid_from_str_and_rev("1e1cdc48aa990d7e154a161d5b5f1cad737742e97d2712ab188027bb42e6e47b")
            .unwrap();

    let (utxo1, arg1) = UtxoBuilder::new()
        .prev_txid(txid)
        .prev_index(0)
        .amount(50 * 100_000_000)
        .sighash_type(SighashType::default())
        .p2pkh(&alice_ecdsa_pubkey)
        .unwrap();

    let output1 = OutputBuilder::new(50 * 100_000_000 - 1_000_000).p2pkh(&bob_ecdsa_pubkey);

    let mut builder = TransactionBuilder::new();
    builder.push_input(utxo1, arg1).push_output(output1);
    let unsigned_tx = builder.build().unwrap();

    // Compute the primage.
    let preimage = SighashComputer::preimage_tx(&unsigned_tx).unwrap();

    // Sign the sighash.
    let sighash = preimage.sighashes.into_iter().next().unwrap().sighash;
    let sig = alice_private_key.sign(sighash).unwrap();

    let tx = TxCompiler::compile(unsigned_tx, &[sig.to_vec()]).unwrap();

    assert_eq!(tx.size(), 191);
    assert_eq!(tx.vsize(), 191); // No witness data
    assert_eq!(tx.weight(), 191 * 4);
    assert_eq!(tx.weight(), 764);
    verify_fee(&tx, SATS_PER_VBYTE, 191 * SATS_PER_VBYTE);

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
    let alice_ecdsa_pubkey =
        ecdsa::secp256k1::PublicKey::try_from(alice_pubkey.as_slice()).unwrap();
    let bob_ecdsa_pubkey = ecdsa::secp256k1::PublicKey::try_from(bob_pubkey.as_slice()).unwrap();

    let txid =
        txid_from_str_and_rev("181c84965c9ea86a5fac32fdbd5f73a21a7a9e749fb6ab97e273af2329f6b911")
            .unwrap();

    let (utxo1, arg1) = UtxoBuilder::new()
        .prev_txid(txid)
        .prev_index(0)
        .amount(50 * 100_000_000)
        .sighash_type(SighashType::default())
        .p2pkh(&alice_ecdsa_pubkey)
        .unwrap();

    let output1 = OutputBuilder::new(50 * 100_000_000 - 1_000_000).p2wpkh(&bob_ecdsa_pubkey);

    let mut builder = TransactionBuilder::new();
    builder.push_input(utxo1, arg1).push_output(output1);
    let unsigned_tx = builder.build().unwrap();

    // Compute the primage.
    let preimage = SighashComputer::preimage_tx(&unsigned_tx).unwrap();

    // Sign the sighash.
    let sighash = preimage.sighashes.into_iter().next().unwrap().sighash;
    let sig = alice_private_key.sign(sighash).unwrap();

    let tx = TxCompiler::compile(unsigned_tx, &[sig.to_vec()]).unwrap();

    assert_eq!(tx.size(), 189);
    assert_eq!(tx.vsize(), 189); // No witness data
    assert_eq!(tx.weight(), 189 * 4);
    assert_eq!(tx.weight(), 756);
    verify_fee(&tx, SATS_PER_VBYTE, 189 * SATS_PER_VBYTE);
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

    let alice_ecdsa_pubkey =
        ecdsa::secp256k1::PublicKey::try_from(alice_pubkey.as_slice()).unwrap();
    let bob_private_key = PrivateKey::try_from(bob_private_key.as_slice()).unwrap();
    let bob_ecdsa_pubkey = ecdsa::secp256k1::PublicKey::try_from(bob_pubkey.as_slice()).unwrap();

    let txid =
        txid_from_str_and_rev("858e450a1da44397bde05ca2f8a78510d74c623cc2f69736a8b3fbfadc161f6e")
            .unwrap();

    let (utxo1, arg1) = UtxoBuilder::new()
        .prev_txid(txid)
        .prev_index(0)
        .amount(50 * 100_000_000 - 1_000_000)
        .sighash_type(SighashType::default())
        .p2wpkh(&bob_ecdsa_pubkey)
        .unwrap();

    let output1 =
        OutputBuilder::new(50 * 100_000_000 - 1_000_000 - 1_000_000).p2wpkh(&alice_ecdsa_pubkey);

    let mut builder = TransactionBuilder::new();
    builder.push_input(utxo1, arg1).push_output(output1);
    let unsigned_tx = builder.build().unwrap();

    // Compute the primage.
    let preimage = SighashComputer::preimage_tx(&unsigned_tx).unwrap();

    // Sign the sighash.
    let sighash = preimage.sighashes.into_iter().next().unwrap().sighash;
    let sig = bob_private_key.sign(sighash).unwrap();

    let tx = TxCompiler::compile(unsigned_tx, &[sig.to_vec()]).unwrap();

    assert_eq!(tx.size(), 192);
    assert_eq!(tx.vsize(), 110); // Witness data discounted
    assert_eq!(tx.weight(), 438);
    verify_fee(&tx, SATS_PER_VBYTE, 110 * SATS_PER_VBYTE);

    let encoded = hex::encode(tx.encode_out(), false);
    assert_eq!(encoded, "020000000001016e1f16dcfafbb3a83697f6c23c624cd71085a7f8a25ce0bd9743a41d0a458e850000000000ffffffff01806de7290100000016001460cda7b50f14c152d7401c28ae773c698db9237302483045022100a9b517de5a5e036d7133df499b5b751db6f9a01576a6c5dc38229ec08b6c45cd02200e42c9f8c707c9bf0ceab4f739ec8d683dc1f1f29e195a8da9bc183584d624a60121025a0af1510f0f24d40dd00d7c0e51605ca504bbc177c3e19b065f373a1efdd22f00000000")
}

#[test]
fn build_tx_input_legacy_output_taproot() {
    let alice_private_key =
        hex::decode("12ce558df23528f1aa86f1f51ac7e13a197a06bda27610fa89e13b04c40ee999").unwrap();
    let alice_pubkey =
        hex::decode("0351e003fdc48e7f31c9bc94996c91f6c3273b7ef4208a1686021bedf7673bb058").unwrap();
    let bob_pubkey =
        hex::decode("02c0938cf377023dfde55e9c96b3cff4ca8894fb6b5d2009006bd43c0bff69cac9").unwrap();

    let alice_private_key = PrivateKey::try_from(alice_private_key.as_slice()).unwrap();
    let alice_ecdsa_pubkey =
        ecdsa::secp256k1::PublicKey::try_from(alice_pubkey.as_slice()).unwrap();
    let bob_schnorr_pubkey = schnorr::PublicKey::try_from(bob_pubkey.as_slice()).unwrap();

    let txid =
        txid_from_str_and_rev("c50563913e5a838f937c94232f5a8fc74e58b629fae41dfdffcc9a70f833b53a")
            .unwrap();

    let (utxo1, arg1) = UtxoBuilder::new()
        .prev_txid(txid)
        .prev_index(0)
        .amount(50 * 100_000_000)
        .sighash_type(SighashType::default())
        .p2pkh(&alice_ecdsa_pubkey)
        .unwrap();

    let output1 =
        OutputBuilder::new(50 * 100_000_000 - 1_000_000).p2tr_key_path(&bob_schnorr_pubkey);

    let mut builder = TransactionBuilder::new();
    builder.push_input(utxo1, arg1).push_output(output1);
    let unsigned_tx = builder.build().unwrap();

    // Compute the primage.
    let preimage = SighashComputer::preimage_tx(&unsigned_tx).unwrap();

    // Sign the sighash.
    let sighash = preimage.sighashes.into_iter().next().unwrap().sighash;
    let sig = alice_private_key.sign(sighash).unwrap();

    let tx = TxCompiler::compile(unsigned_tx, &[sig.to_vec()]).unwrap();

    //assert_eq!(tx.size(), 191);
    //assert_eq!(tx.vsize(), 110); // Witness data discounted
    //assert_eq!(tx.weight(), 110 * 4);
    //assert_eq!(tx.weight(), 440);
    //assert_eq!(tx.fee(SATS_PER_VBYTE), 110 * SATS_PER_VBYTE);
    //assert_eq!(tx.fee(SATS_PER_VBYTE), 2200);

    let encoded = hex::encode(tx.encode_out(), false);
    assert_eq!(encoded, "02000000013ab533f8709accfffd1de4fa29b6584ec78f5a2f23947c938f835a3e916305c5000000006b48304502210086ab2c2192e2738529d6cd9604d8ee75c5b09b0c2f4066a5c5fa3f87a26c0af602202afc7096aaa992235c43e712146057b5ed6a776d82b9129620bc5a21991c0a5301210351e003fdc48e7f31c9bc94996c91f6c3273b7ef4208a1686021bedf7673bb058ffffffff01c0aff62901000000225120e01cfdd05da8fa1d71f987373f3790d45dea9861acb0525c86656fe50f4397a600000000")
}

#[test]
fn build_tx_input_taproot_output_taproot() {
    let bob_private_key =
        hex::decode("26c2566adcc030a1799213bfd546e615f6ab06f72085ec6806ff1761da48d227").unwrap();
    let alice_pubkey =
        hex::decode("0351e003fdc48e7f31c9bc94996c91f6c3273b7ef4208a1686021bedf7673bb058").unwrap();

    let alice_schnorr_pubkey = schnorr::PublicKey::try_from(alice_pubkey.as_slice()).unwrap();
    let bob_private_key = schnorr::PrivateKey::try_from(bob_private_key.as_slice()).unwrap();
    let bob_pubkey = bob_private_key.public();

    let txid =
        txid_from_str_and_rev("9a582032f6a50cedaff77d3d5604b33adf8bc31bdaef8de977c2187e395860ac")
            .unwrap();

    let (utxo1, arg1) = UtxoBuilder::new()
        .prev_txid(txid)
        .prev_index(0)
        .amount(50 * 100_000_000 - 1_000_000)
        .sighash_type(SighashType::default())
        .p2tr_key_path(&bob_pubkey)
        .unwrap();

    let output1 = OutputBuilder::new(50 * 100_000_000 - 1_000_000 - 1_000_000)
        .p2tr_key_path(&alice_schnorr_pubkey);

    let mut builder = TransactionBuilder::new();
    builder.push_input(utxo1, arg1).push_output(output1);
    let unsigned_tx = builder.build().unwrap();

    // Compute the primage.
    let preimage = SighashComputer::preimage_tx(&unsigned_tx).unwrap();

    // Sign the sighash.
    let sighash = preimage.sighashes.into_iter().next().unwrap().sighash;
    let tweaked = bob_private_key.tweak(None).no_aux_rand();
    let sig = tweaked.sign(sighash).unwrap();

    let tx = TxCompiler::compile(unsigned_tx, &[sig.to_vec()]).unwrap();

    let encoded = hex::encode(tx.encode_out(), false);
    assert_eq!(encoded, "02000000000101ac6058397e18c277e98defda1bc38bdf3ab304563d7df7afed0ca5f63220589a0000000000ffffffff01806de72901000000225120a5c027857e359d19f625e52a106b8ac6ca2d6a8728f6cf2107cd7958ee0787c20140ec2d3910d41506b60aaa20520bb72f15e2d2cbd97e3a8e26ee7bad5f4c56b0f2fb0ceaddac33cb2813a33ba017ba6b1d011bab74a0426f12a2bcf47b4ed5bc8600000000")
}

#[test]
fn build_tx_input_segwit_output_brc20_transfer_commit() {
    let alice_private_key =
        hex::decode("e253373989199da27c48680e3a3fc0f648d50f9a727ef17a7fe6a4dc3b159129").unwrap();
    let alice_pubkey =
        hex::decode("030f209b6ada5edb42c77fd2bc64ad650ae38314c8f451f3e36d80bc8e26f132cb").unwrap();

    let alice_private_key = PrivateKey::try_from(alice_private_key.as_slice()).unwrap();
    let alice_ecdsa_pubkey =
        ecdsa::secp256k1::PublicKey::try_from(alice_pubkey.as_slice()).unwrap();
    let alice_schnorr_pubkey = schnorr::PublicKey::try_from(alice_pubkey.as_slice()).unwrap();

    let txid =
        txid_from_str_and_rev("8ec895b4d30adb01e38471ca1019bfc8c3e5fbd1f28d9e7b5653260d89989008")
            .unwrap();

    let (utxo1, arg1) = UtxoBuilder::new()
        .prev_txid(txid)
        .prev_index(1)
        .amount(26_400)
        .sighash_type(SighashType::default())
        .p2wpkh(&alice_ecdsa_pubkey)
        .unwrap();

    let output1 = OutputBuilder::new(7_000)
        .brc20_transfer(&alice_schnorr_pubkey, "oadf".into(), "20".into())
        .unwrap();

    let output2 = OutputBuilder::new(16_400).p2wpkh(&alice_ecdsa_pubkey);

    let mut builder = TransactionBuilder::new();
    builder
        .push_input(utxo1, arg1.clone())
        .push_output(output1)
        .push_output(output2);
    let unsigned_tx = builder.build().unwrap();

    // Compute the primage.
    let preimage = SighashComputer::preimage_tx(&unsigned_tx).unwrap();

    // Sign the sighash.
    let sighash = preimage.sighashes.into_iter().next().unwrap().sighash;

    // Tweak the private key with the Taproot script leaf hash.
    let sig = alice_private_key.sign(sighash).unwrap();

    let tx = TxCompiler::compile(unsigned_tx, &[sig.to_vec()]).unwrap();

    let encoded = hex::encode(tx.encode_out(), false);
    assert_eq!(encoded, "02000000000101089098890d2653567b9e8df2d1fbe5c3c8bf1910ca7184e301db0ad3b495c88e0100000000ffffffff02581b000000000000225120e8b706a97732e705e22ae7710703e7f589ed13c636324461afa443016134cc051040000000000000160014e311b8d6ddff856ce8e9a4e03bc6d4fe5050a83d02483045022100a44aa28446a9a886b378a4a65e32ad9a3108870bd725dc6105160bed4f317097022069e9de36422e4ce2e42b39884aa5f626f8f94194d1013007d5a1ea9220a06dce0121030f209b6ada5edb42c77fd2bc64ad650ae38314c8f451f3e36d80bc8e26f132cb00000000")
}

#[test]
fn build_tx_input_brc20_transfer_commit_output_brc20_transfer_reveal() {
    let alice_private_key =
        hex::decode("e253373989199da27c48680e3a3fc0f648d50f9a727ef17a7fe6a4dc3b159129").unwrap();
    let alice_pubkey =
        hex::decode("030f209b6ada5edb42c77fd2bc64ad650ae38314c8f451f3e36d80bc8e26f132cb").unwrap();

    let alice_ecdsa_pubkey =
        ecdsa::secp256k1::PublicKey::try_from(alice_pubkey.as_slice()).unwrap();
    let alice_private_key = schnorr::PrivateKey::try_from(alice_private_key.as_slice())
        .unwrap()
        .no_aux_rand();
    let alice_pubkey = alice_private_key.public();

    let txid =
        txid_from_str_and_rev("797d17d47ae66e598341f9dfdea020b04d4017dcf9cc33f0e51f7a6082171fb1")
            .unwrap();

    let (utxo1, arg1) = UtxoBuilder::new()
        .prev_txid(txid)
        .prev_index(0)
        .amount(7_000)
        .sighash_type(SighashType::default())
        .brc20_transfer(&alice_pubkey, "oadf".into(), "20".into())
        .unwrap();

    let output1 = OutputBuilder::new(546).p2wpkh(&alice_ecdsa_pubkey);

    let mut builder = TransactionBuilder::new();
    builder.push_input(utxo1, arg1.clone()).push_output(output1);
    let unsigned_tx = builder.build().unwrap();

    // Compute the primage.
    let preimage = SighashComputer::preimage_tx(&unsigned_tx).unwrap();

    // Sign the sighash.
    let sighash = preimage.sighashes.into_iter().next().unwrap().sighash;

    // Tweak the private key with the Taproot script leaf hash.
    let sig = alice_private_key.sign(sighash).unwrap();

    let tx = TxCompiler::compile(unsigned_tx, &[sig.to_vec()]).unwrap();

    let encoded = hex::encode(tx.encode_out(), false);
    assert_eq!(encoded, "02000000000101b11f1782607a1fe5f033ccf9dc17404db020a0dedff94183596ee67ad4177d790000000000ffffffff012202000000000000160014e311b8d6ddff856ce8e9a4e03bc6d4fe5050a83d03406a35548b8fa4620028e021a944c1d3dc6e947243a7bfc901bf63fefae0d2460efa149a6440cab51966aa4f09faef2d1e5efcba23ab4ca6e669da598022dbcfe35b0063036f7264010118746578742f706c61696e3b636861727365743d7574662d3800377b2270223a226272632d3230222c226f70223a227472616e73666572222c227469636b223a226f616466222c22616d74223a223230227d6821c00f209b6ada5edb42c77fd2bc64ad650ae38314c8f451f3e36d80bc8e26f132cb00000000")
}
