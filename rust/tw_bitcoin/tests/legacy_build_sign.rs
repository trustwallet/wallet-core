#![allow(deprecated)]

mod common;

use common::{hex, MINER_FEE, ONE_BTC};
use secp256k1::ffi::CPtr;
use std::ffi::CString;
use tw_proto::Bitcoin::Proto as LegacyProto;
use tw_proto::Common::Proto as CommonProto;
use wallet_core_rs::ffi::bitcoin::legacy as legacy_ffi;

const ONE_BTC_I64: i64 = ONE_BTC as i64;
const MINER_FEE_I64: i64 = MINER_FEE as i64;

#[test]
fn ffi_proto_sign_input_p2pkh_output_p2pkh() {
    let alice_private_key = hex("56429688a1a6b00b90ccd22a0de0a376b6569d8684022ae92229a28478bfb657");
    let bob_pubkey = hex("037ed9a436e11ec4947ac4b7823787e24ba73180f1edd2857bff19c9f4d62b65bf");

    let txid = hex("1e1cdc48aa990d7e154a161d5b5f1cad737742e97d2712ab188027bb42e6e47b")
        .into_iter()
        .rev()
        .collect();

    // Output.
    let output = unsafe {
        legacy_ffi::tw_bitcoin_legacy_build_p2pkh_script(
            ONE_BTC_I64 * 50 - MINER_FEE_I64,
            bob_pubkey.as_c_ptr(),
            bob_pubkey.len(),
        )
        .into_vec()
    };
    let output: LegacyProto::TransactionOutput = tw_proto::deserialize(&output).unwrap();

    // Prepare SigningInput.
    let signing = LegacyProto::SigningInput {
        private_key: vec![alice_private_key.into()],
        utxo: vec![LegacyProto::UnspentTransaction {
            out_point: Some(LegacyProto::OutPoint {
                hash: txid,
                index: 0,
                sequence: u32::MAX,
                ..Default::default()
            }),
            // For inputs, script is not needed (derived from variant).
            script: Default::default(),
            amount: ONE_BTC_I64 * 50,
            variant: LegacyProto::TransactionVariant::P2PKH,
            spendingScript: Default::default(),
        }],
        plan: Some(LegacyProto::TransactionPlan {
            utxos: vec![LegacyProto::UnspentTransaction {
                out_point: Default::default(),
                script: output.script,
                amount: output.value,
                variant: LegacyProto::TransactionVariant::P2PKH,
                spendingScript: Default::default(),
            }],
            ..Default::default()
        }),
        ..Default::default()
    };
    let serialized = tw_proto::serialize(&signing).unwrap();

    // Sign and build the transaction.
    let signed = unsafe {
        legacy_ffi::tw_bitcoin_legacy_taproot_build_and_sign_transaction(
            serialized.as_c_ptr(),
            serialized.len(),
        )
        .into_vec()
    };
    let signed: LegacyProto::SigningOutput = tw_proto::deserialize(&signed).unwrap();

    // Check result.
    assert_eq!(signed.error, CommonProto::SigningError::OK);
    let encoded_hex = tw_encoding::hex::encode(signed.encoded, false);
    assert_eq!(encoded_hex, "02000000017be4e642bb278018ab12277de9427773ad1c5f5b1d164a157e0d99aa48dc1c1e000000006a473044022078eda020d4b86fcb3af78ef919912e6d79b81164dbbb0b0b96da6ac58a2de4b102201a5fd8d48734d5a02371c4b5ee551a69dca3842edbf577d863cf8ae9fdbbd4590121036666dd712e05a487916384bfcd5973eb53e8038eccbbf97f7eed775b87389536ffffffff01c0aff629010000001976a9145eaaa4f458f9158f86afcba08dd7448d27045e3d88ac00000000");
}

#[test]
fn ffi_proto_sign_input_p2pkh_output_p2wpkh() {
    let alice_private_key = hex("57a64865bce5d4855e99b1cce13327c46171434f2d72eeaf9da53ee075e7f90a");
    let bob_pubkey = hex("025a0af1510f0f24d40dd00d7c0e51605ca504bbc177c3e19b065f373a1efdd22f");

    let txid = hex("181c84965c9ea86a5fac32fdbd5f73a21a7a9e749fb6ab97e273af2329f6b911")
        .into_iter()
        .rev()
        .collect();

    // Output.
    let output = unsafe {
        legacy_ffi::tw_bitcoin_legacy_build_p2wpkh_script(
            ONE_BTC_I64 * 50 - MINER_FEE_I64,
            bob_pubkey.as_c_ptr(),
            bob_pubkey.len(),
        )
        .into_vec()
    };
    let output: LegacyProto::TransactionOutput = tw_proto::deserialize(&output).unwrap();

    // Prepare SigningInput.
    let signing = LegacyProto::SigningInput {
        private_key: vec![alice_private_key.into()],
        utxo: vec![LegacyProto::UnspentTransaction {
            out_point: Some(LegacyProto::OutPoint {
                hash: txid,
                index: 0,
                sequence: u32::MAX,
                ..Default::default()
            }),
            // For inputs, script is not needed (derived from variant).
            script: Default::default(),
            amount: ONE_BTC_I64 * 50,
            variant: LegacyProto::TransactionVariant::P2PKH,
            spendingScript: Default::default(),
        }],
        plan: Some(LegacyProto::TransactionPlan {
            utxos: vec![LegacyProto::UnspentTransaction {
                out_point: Default::default(),
                script: output.script,
                amount: output.value,
                variant: LegacyProto::TransactionVariant::P2WPKH,
                spendingScript: Default::default(),
            }],
            ..Default::default()
        }),
        ..Default::default()
    };
    let serialized = tw_proto::serialize(&signing).unwrap();

    // Sign and build the transaction.
    let signed = unsafe {
        legacy_ffi::tw_bitcoin_legacy_taproot_build_and_sign_transaction(
            serialized.as_c_ptr(),
            serialized.len(),
        )
        .into_vec()
    };
    let signed: LegacyProto::SigningOutput = tw_proto::deserialize(&signed).unwrap();

    // Check result.
    assert_eq!(signed.error, CommonProto::SigningError::OK);
    let encoded_hex = tw_encoding::hex::encode(signed.encoded, false);
    assert_eq!(encoded_hex, "020000000111b9f62923af73e297abb69f749e7a1aa2735fbdfd32ac5f6aa89e5c96841c18000000006b483045022100df9ed0b662b759e68b89a42e7144cddf787782a7129d4df05642dd825930e6e6022051a08f577f11cc7390684bbad2951a6374072253ffcf2468d14035ed0d8cd6490121028d7dce6d72fb8f7af9566616c6436349c67ad379f2404dd66fe7085fe0fba28fffffffff01c0aff629010000001600140d0e1cec6c2babe8badde5e9b3dea667da90036d00000000");
}

#[test]
fn ffi_proto_sign_input_p2pkh_output_p2tr_key_path() {
    let alice_private_key = hex("12ce558df23528f1aa86f1f51ac7e13a197a06bda27610fa89e13b04c40ee999");
    let alice_pubkey = hex("0351e003fdc48e7f31c9bc94996c91f6c3273b7ef4208a1686021bedf7673bb058");
    let bob_private_key = hex("26c2566adcc030a1799213bfd546e615f6ab06f72085ec6806ff1761da48d227");
    let bob_pubkey = hex("02c0938cf377023dfde55e9c96b3cff4ca8894fb6b5d2009006bd43c0bff69cac9");

    let txid = hex("c50563913e5a838f937c94232f5a8fc74e58b629fae41dfdffcc9a70f833b53a")
        .into_iter()
        .rev()
        .collect();

    // Output.
    let output = unsafe {
        legacy_ffi::tw_bitcoin_legacy_build_p2tr_key_path_script(
            ONE_BTC_I64 * 50 - MINER_FEE_I64,
            bob_pubkey.as_c_ptr(),
            bob_pubkey.len(),
        )
        .into_vec()
    };
    let output: LegacyProto::TransactionOutput = tw_proto::deserialize(&output).unwrap();

    // Prepare SigningInput.
    let signing = LegacyProto::SigningInput {
        private_key: vec![alice_private_key.into()],
        utxo: vec![LegacyProto::UnspentTransaction {
            out_point: Some(LegacyProto::OutPoint {
                hash: txid,
                index: 0,
                sequence: u32::MAX,
                ..Default::default()
            }),
            // For inputs, script is not needed (derived from variant).
            script: Default::default(),
            amount: ONE_BTC_I64 * 50,
            variant: LegacyProto::TransactionVariant::P2PKH,
            spendingScript: Default::default(),
        }],
        plan: Some(LegacyProto::TransactionPlan {
            utxos: vec![LegacyProto::UnspentTransaction {
                out_point: Default::default(),
                script: output.script,
                amount: output.value,
                variant: LegacyProto::TransactionVariant::P2TRKEYPATH,
                spendingScript: Default::default(),
            }],
            ..Default::default()
        }),
        ..Default::default()
    };
    let serialized = tw_proto::serialize(&signing).unwrap();

    // Sign and build the transaction.
    let signed = unsafe {
        legacy_ffi::tw_bitcoin_legacy_taproot_build_and_sign_transaction(
            serialized.as_c_ptr(),
            serialized.len(),
        )
        .into_vec()
    };
    let signed: LegacyProto::SigningOutput = tw_proto::deserialize(&signed).unwrap();

    // Check result.
    assert_eq!(signed.error, CommonProto::SigningError::OK);
    let encoded_hex = tw_encoding::hex::encode(signed.encoded, false);
    assert_eq!(encoded_hex, "02000000013ab533f8709accfffd1de4fa29b6584ec78f5a2f23947c938f835a3e916305c5000000006b48304502210086ab2c2192e2738529d6cd9604d8ee75c5b09b0c2f4066a5c5fa3f87a26c0af602202afc7096aaa992235c43e712146057b5ed6a776d82b9129620bc5a21991c0a5301210351e003fdc48e7f31c9bc94996c91f6c3273b7ef4208a1686021bedf7673bb058ffffffff01c0aff62901000000225120e01cfdd05da8fa1d71f987373f3790d45dea9861acb0525c86656fe50f4397a600000000");

    // Next transaction; try to spend the P2TR key-path.

    let txid: Vec<u8> = hex("9a582032f6a50cedaff77d3d5604b33adf8bc31bdaef8de977c2187e395860ac")
        .into_iter()
        .rev()
        .collect();

    // Output.
    let output = unsafe {
        legacy_ffi::tw_bitcoin_legacy_build_p2tr_key_path_script(
            ONE_BTC_I64 * 50 - MINER_FEE_I64 - MINER_FEE_I64,
            alice_pubkey.as_c_ptr(),
            alice_pubkey.len(),
        )
        .into_vec()
    };
    let output: LegacyProto::TransactionOutput = tw_proto::deserialize(&output).unwrap();

    // Prepare SigningInput.
    let signing = LegacyProto::SigningInput {
        private_key: vec![bob_private_key.into()],
        utxo: vec![LegacyProto::UnspentTransaction {
            out_point: Some(LegacyProto::OutPoint {
                hash: txid.into(),
                index: 0,
                sequence: u32::MAX,
                ..Default::default()
            }),
            // For inputs, script is not needed (derived from variant).
            script: Default::default(),
            amount: ONE_BTC_I64 * 50 - MINER_FEE_I64,
            variant: LegacyProto::TransactionVariant::P2TRKEYPATH,
            spendingScript: Default::default(),
        }],
        plan: Some(LegacyProto::TransactionPlan {
            utxos: vec![LegacyProto::UnspentTransaction {
                out_point: Default::default(),
                script: output.script,
                amount: output.value,
                variant: LegacyProto::TransactionVariant::P2TRKEYPATH,
                spendingScript: Default::default(),
            }],
            ..Default::default()
        }),
        ..Default::default()
    };
    let serialized = tw_proto::serialize(&signing).unwrap();

    // Sign and build the transaction.
    let signed = unsafe {
        legacy_ffi::tw_bitcoin_legacy_taproot_build_and_sign_transaction(
            serialized.as_c_ptr(),
            serialized.len(),
        )
        .into_vec()
    };
    let signed: LegacyProto::SigningOutput = tw_proto::deserialize(&signed).unwrap();

    // Check result.
    const REVEAL_RAW: &str = "02000000000101ac6058397e18c277e98defda1bc38bdf3ab304563d7df7afed0ca5f63220589a0000000000ffffffff01806de72901000000225120a5c027857e359d19f625e52a106b8ac6ca2d6a8728f6cf2107cd7958ee0787c20140ec2d3910d41506b60aaa20520bb72f15e2d2cbd97e3a8e26ee7bad5f4c56b0f2fb0ceaddac33cb2813a33ba017ba6b1d011bab74a0426f12a2bcf47b4ed5bc8600000000";

    assert_eq!(signed.error, CommonProto::SigningError::OK);
    let encoded_hex = tw_encoding::hex::encode(signed.encoded, false);
    assert_eq!(encoded_hex[..188], REVEAL_RAW[..188]);
    // Schnorr signature does not match (non-deterministic).
    assert_ne!(encoded_hex[188..316], REVEAL_RAW[188..316]);
    assert_eq!(encoded_hex[316..], REVEAL_RAW[316..]);
}

#[test]
fn ffi_proto_sign_input_p2wpkh_output_brc20() {
    let alice_private_key = hex("e253373989199da27c48680e3a3fc0f648d50f9a727ef17a7fe6a4dc3b159129");
    let alice_pubkey = hex("030f209b6ada5edb42c77fd2bc64ad650ae38314c8f451f3e36d80bc8e26f132cb");

    let txid = hex("8ec895b4d30adb01e38471ca1019bfc8c3e5fbd1f28d9e7b5653260d89989008")
        .into_iter()
        .rev()
        .collect();

    // Output.
    let c_ticker = CString::new("oadf").unwrap();
    let c_amount = CString::new("20").unwrap();
    let brc20_output = unsafe {
        legacy_ffi::tw_bitcoin_legacy_build_brc20_transfer_inscription(
            c_ticker.as_ptr(),
            c_amount.as_ptr(),
            7_000,
            alice_pubkey.as_c_ptr(),
            alice_pubkey.len(),
        )
        .into_vec()
    };
    let brc20_output: LegacyProto::TransactionOutput =
        tw_proto::deserialize(&brc20_output).unwrap();

    // Change output.
    let change_output = unsafe {
        legacy_ffi::tw_bitcoin_legacy_build_p2wpkh_script(
            16_400,
            alice_pubkey.as_c_ptr(),
            alice_pubkey.len(),
        )
        .into_vec()
    };
    let change_output: LegacyProto::TransactionOutput =
        tw_proto::deserialize(&change_output).unwrap();

    // Prepare SigningInput.
    let signing = LegacyProto::SigningInput {
        private_key: vec![alice_private_key.clone().into()],
        utxo: vec![LegacyProto::UnspentTransaction {
            out_point: Some(LegacyProto::OutPoint {
                hash: txid,
                index: 1,
                sequence: u32::MAX,
                ..Default::default()
            }),
            // For inputs, script is not needed (derived from variant).
            script: Default::default(),
            amount: 26_400,
            variant: LegacyProto::TransactionVariant::P2WPKH,
            spendingScript: Default::default(),
        }],
        plan: Some(LegacyProto::TransactionPlan {
            utxos: vec![
                LegacyProto::UnspentTransaction {
                    out_point: Default::default(),
                    script: brc20_output.script.clone(),
                    amount: brc20_output.value,
                    variant: LegacyProto::TransactionVariant::BRC20TRANSFER,
                    spendingScript: Default::default(),
                },
                // Change output.
                LegacyProto::UnspentTransaction {
                    out_point: Default::default(),
                    script: change_output.script,
                    amount: change_output.value,
                    variant: LegacyProto::TransactionVariant::P2WPKH,
                    spendingScript: Default::default(),
                },
            ],
            ..Default::default()
        }),
        ..Default::default()
    };
    let serialized = tw_proto::serialize(&signing).unwrap();

    // Sign and build the transaction.
    let signed = unsafe {
        legacy_ffi::tw_bitcoin_legacy_taproot_build_and_sign_transaction(
            serialized.as_c_ptr(),
            serialized.len(),
        )
        .into_vec()
    };
    let signed: LegacyProto::SigningOutput = tw_proto::deserialize(&signed).unwrap();

    // Check result.
    assert_eq!(signed.error, CommonProto::SigningError::OK);
    let encoded_hex = tw_encoding::hex::encode(signed.encoded, false);
    assert_eq!(encoded_hex, "02000000000101089098890d2653567b9e8df2d1fbe5c3c8bf1910ca7184e301db0ad3b495c88e0100000000ffffffff02581b000000000000225120e8b706a97732e705e22ae7710703e7f589ed13c636324461afa443016134cc051040000000000000160014e311b8d6ddff856ce8e9a4e03bc6d4fe5050a83d02483045022100a44aa28446a9a886b378a4a65e32ad9a3108870bd725dc6105160bed4f317097022069e9de36422e4ce2e42b39884aa5f626f8f94194d1013007d5a1ea9220a06dce0121030f209b6ada5edb42c77fd2bc64ad650ae38314c8f451f3e36d80bc8e26f132cb00000000");

    // Next transaction; try to spend the BRC20 transfer inscription.

    let txid: Vec<u8> = hex("797d17d47ae66e598341f9dfdea020b04d4017dcf9cc33f0e51f7a6082171fb1")
        .into_iter()
        .rev()
        .collect();

    // Tagged output.
    let output = unsafe {
        legacy_ffi::tw_bitcoin_legacy_build_p2wpkh_script(
            546,
            alice_pubkey.as_c_ptr(),
            alice_pubkey.len(),
        )
        .into_vec()
    };
    let output: LegacyProto::TransactionOutput = tw_proto::deserialize(&output).unwrap();

    // Prepare SigningInput.
    let signing = LegacyProto::SigningInput {
        private_key: vec![alice_private_key.into()],
        utxo: vec![LegacyProto::UnspentTransaction {
            out_point: Some(LegacyProto::OutPoint {
                hash: txid.into(),
                index: 0,
                sequence: u32::MAX,
                ..Default::default()
            }),
            script: Default::default(),
            amount: brc20_output.value,
            variant: LegacyProto::TransactionVariant::BRC20TRANSFER,
            // IMPORTANT: spending script is specified.
            spendingScript: brc20_output.spendingScript,
        }],
        plan: Some(LegacyProto::TransactionPlan {
            utxos: vec![LegacyProto::UnspentTransaction {
                out_point: Default::default(),
                script: output.script,
                amount: 546,
                variant: LegacyProto::TransactionVariant::P2WPKH,
                spendingScript: Default::default(),
            }],
            ..Default::default()
        }),
        ..Default::default()
    };
    let serialized = tw_proto::serialize(&signing).unwrap();

    // Sign and build the transaction.
    let signed = unsafe {
        legacy_ffi::tw_bitcoin_legacy_taproot_build_and_sign_transaction(
            serialized.as_c_ptr(),
            serialized.len(),
        )
        .into_vec()
    };
    let signed: LegacyProto::SigningOutput = tw_proto::deserialize(&signed).unwrap();

    // Check result.
    const REVEAL_RAW: &str = "02000000000101b11f1782607a1fe5f033ccf9dc17404db020a0dedff94183596ee67ad4177d790000000000ffffffff012202000000000000160014e311b8d6ddff856ce8e9a4e03bc6d4fe5050a83d03406a35548b8fa4620028e021a944c1d3dc6e947243a7bfc901bf63fefae0d2460efa149a6440cab51966aa4f09faef2d1e5efcba23ab4ca6e669da598022dbcfe35b0063036f7264010118746578742f706c61696e3b636861727365743d7574662d3800377b2270223a226272632d3230222c226f70223a227472616e73666572222c227469636b223a226f616466222c22616d74223a223230227d6821c00f209b6ada5edb42c77fd2bc64ad650ae38314c8f451f3e36d80bc8e26f132cb00000000";

    assert_eq!(signed.error, CommonProto::SigningError::OK);
    let encoded_hex = tw_encoding::hex::encode(signed.encoded, false);
    assert_eq!(encoded_hex[..164], REVEAL_RAW[..164]);
    // Schnorr signature does not match (non-deterministic).
    assert_ne!(encoded_hex[164..292], REVEAL_RAW[164..292]);
    assert_eq!(encoded_hex[292..], REVEAL_RAW[292..]);
}
