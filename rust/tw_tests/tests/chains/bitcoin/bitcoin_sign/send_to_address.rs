use crate::chains::common::bitcoin::{
    btc_info, dust_threshold, input, output, sign, TransactionOneof, DUST, SIGHASH_ALL,
};
use std::str::FromStr;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::DecodeHex;
use tw_keypair::ecdsa;
use tw_misc::traits::{ToBytesVec, ToBytesZeroizing};
use tw_proto::BitcoinV2::Proto;
use tw_utxo::address::legacy::LegacyAddress;
use tw_utxo::address::segwit::SegwitAddress;
use tw_utxo::script::standard_script::conditions;

#[test]
fn test_bitcoin_send_to_p2sh_address() {
    let p2sh_prefix = btc_info().unwrap().p2sh_prefix as u8;

    let alice_private_key = "57a64865bce5d4855e99b1cce13327c46171434f2d72eeaf9da53ee075e7f90a";
    let alice_private_key = ecdsa::secp256k1::PrivateKey::try_from(alice_private_key).unwrap();
    let alice_public_key = alice_private_key.public();
    let bob_address = "19prEapJCTF3zAS2ofreXyQhcnDscuXxbd";

    // Create the P2SH address.
    // We use a simple P2PKH as the redeem script.
    let bob_address = LegacyAddress::from_str(bob_address).unwrap();
    let p2pkh_redeem_script = conditions::new_p2pkh(&bob_address.payload());
    let p2sh_address =
        LegacyAddress::p2sh_with_prefix_byte(&p2pkh_redeem_script, p2sh_prefix).unwrap();
    assert_eq!(
        p2sh_address.to_string(),
        "3BSsey83R7AvczeRrYZihwDTkQ8Khjtewr"
    );

    let utxo_hash_0 = "181c84965c9ea86a5fac32fdbd5f73a21a7a9e749fb6ab97e273af2329f6b911";
    let utxo_0 = Proto::Input {
        out_point: input::out_point(utxo_hash_0, 0),
        value: 10_000,
        sighash_type: SIGHASH_ALL,
        claiming_script: input::p2pkh(alice_public_key.to_vec().into()),
        ..Default::default()
    };

    // The output variant is derived from the specified address.
    let out_0 = Proto::Output {
        value: 1_000,
        to_recipient: output::to_address(&p2sh_address.to_string()),
    };

    let builder = Proto::TransactionBuilder {
        version: Proto::TransactionVersion::V2,
        inputs: vec![utxo_0],
        outputs: vec![out_0],
        input_selector: Proto::InputSelector::UseAll,
        dust_policy: dust_threshold(DUST),
        fee_per_vb: 1,
        ..Default::default()
    };

    let signing = Proto::SigningInput {
        private_keys: vec![alice_private_key.to_zeroizing_vec().to_vec().into()],
        transaction: TransactionOneof::builder(builder),
        ..Default::default()
    };

    sign::BitcoinSignHelper::new(&signing)
        .coin(CoinType::Bitcoin)
        .sign(sign::Expected {
            encoded: "020000000111b9f62923af73e297abb69f749e7a1aa2735fbdfd32ac5f6aa89e5c96841c18000000006a4730440220079b598713c12210c13f059c477f4ab33f85e80749c0925e0ac655bc0a3b9ca802200e9b83fb57ab3f2c907daeb386f02ed1e459aa058168c9e3b4e9a074ec8db78d0121028d7dce6d72fb8f7af9566616c6436349c67ad379f2404dd66fe7085fe0fba28fffffffff01e80300000000000017a9146b04883a86994629fcc84e558968e2d70a3472fd8700000000",
            txid: "4a476d6bc5e6e7fe13ef4388a22ff75cf9ecebb656483c27f5d884a1f173ac66",
            inputs: vec![10_000],
            outputs: vec![1_000],
            vsize: 189,
            weight: 756,
            // sum(inputs) - sum(outputs)
            fee: 9000,
        });
}

#[test]
fn test_bitcoin_send_to_p2pkh_address() {
    let alice_private_key = "57a64865bce5d4855e99b1cce13327c46171434f2d72eeaf9da53ee075e7f90a"
        .decode_hex()
        .unwrap();
    let alice_pubkey = "028d7dce6d72fb8f7af9566616c6436349c67ad379f2404dd66fe7085fe0fba28f"
        .decode_hex()
        .unwrap();
    let bob_address = "12C2h5hXPxyrdvnYUBFaBGFnNNYjpWXhPX";

    let txid = "181c84965c9ea86a5fac32fdbd5f73a21a7a9e749fb6ab97e273af2329f6b911";
    let tx1 = Proto::Input {
        out_point: input::out_point(txid, 0),
        value: 10_000,
        sighash_type: SIGHASH_ALL,
        claiming_script: input::p2pkh(alice_pubkey.clone()),
        ..Default::default()
    };

    // The output variant is derived from the specified address.
    let out1 = Proto::Output {
        value: 1_000,
        to_recipient: output::to_address(bob_address),
    };

    let builder = Proto::TransactionBuilder {
        version: Proto::TransactionVersion::V2,
        inputs: vec![tx1],
        outputs: vec![out1],
        input_selector: Proto::InputSelector::UseAll,
        dust_policy: dust_threshold(DUST),
        ..Default::default()
    };

    let signing = Proto::SigningInput {
        private_keys: vec![alice_private_key.into()],
        transaction: TransactionOneof::builder(builder),
        ..Default::default()
    };

    sign::BitcoinSignHelper::new(&signing)
        .coin(CoinType::Bitcoin)
        .sign(sign::Expected {
            encoded: "020000000111b9f62923af73e297abb69f749e7a1aa2735fbdfd32ac5f6aa89e5c96841c18000000006b483045022100a42afe29055e70d912c7bc3b113a6a58216ad0eafc7d73ae447df9044e347d900220530c5b4721c0c832d06be4921369792da9187df6fcf17bcf6c77a27d5ed85a200121028d7dce6d72fb8f7af9566616c6436349c67ad379f2404dd66fe7085fe0fba28fffffffff01e8030000000000001976a9140d0e1cec6c2babe8badde5e9b3dea667da90036d88ac00000000",
            txid: "ff0901ef4796e9a0a82c4f8a6edebe9112cc54c4eefac77f6d4c4dd267b0da86",
            inputs: vec![10_000],
            outputs: vec![1_000],
            vsize: 192,
            weight: 768,
            fee: 9_000,
        });
}

#[test]
fn test_bitcoin_send_to_p2wsh_address() {
    let alice_private_key = "57a64865bce5d4855e99b1cce13327c46171434f2d72eeaf9da53ee075e7f90a";
    let alice_private_key = ecdsa::secp256k1::PrivateKey::try_from(alice_private_key).unwrap();
    let alice_pubkey = alice_private_key.public();
    let bob_address = "12C2h5hXPxyrdvnYUBFaBGFnNNYjpWXhPX";

    // Create the P2WSH address.
    // We use a simple P2PKH as the redeem script.
    let bob_address = LegacyAddress::from_str(bob_address).unwrap();
    let p2pkh_redeem_script = conditions::new_p2pkh(&bob_address.payload());
    let p2wsh_address =
        SegwitAddress::p2wsh_with_hrp(&p2pkh_redeem_script, "bc".to_string()).unwrap();
    assert_eq!(
        p2wsh_address.to_string(),
        "bc1qmtahx7nh2fqlcrf8fgf49mq6ywxnvjssay26aa8qfvf3xd907l6suvaz4x"
    );

    let utxo_hash_0 = "181c84965c9ea86a5fac32fdbd5f73a21a7a9e749fb6ab97e273af2329f6b911";
    let utxo_0 = Proto::Input {
        out_point: input::out_point(utxo_hash_0, 0),
        value: 10_000,
        sighash_type: SIGHASH_ALL,
        claiming_script: input::p2pkh(alice_pubkey.to_vec()),
        ..Default::default()
    };

    // The output variant is derived from the specified address.
    let out_0 = Proto::Output {
        value: 1_000,
        to_recipient: output::to_address(&p2wsh_address.to_string()),
    };

    let builder = Proto::TransactionBuilder {
        version: Proto::TransactionVersion::V2,
        inputs: vec![utxo_0],
        outputs: vec![out_0],
        input_selector: Proto::InputSelector::UseAll,
        dust_policy: dust_threshold(DUST),
        ..Default::default()
    };

    let signing = Proto::SigningInput {
        private_keys: vec![alice_private_key.to_zeroizing_vec().to_vec().into()],
        transaction: TransactionOneof::builder(builder),
        ..Default::default()
    };

    sign::BitcoinSignHelper::new(&signing)
        .coin(CoinType::Bitcoin)
        .sign(sign::Expected {
            encoded: "020000000111b9f62923af73e297abb69f749e7a1aa2735fbdfd32ac5f6aa89e5c96841c18000000006b483045022100b0ee8b8a5466db1fef9054e29bf2d241f1637dfafe65f87559bc55028153051802201008183146fd71a744cbe66ae01222c4372f2b6d274658d7dfa18ef33ab5ab5b0121028d7dce6d72fb8f7af9566616c6436349c67ad379f2404dd66fe7085fe0fba28fffffffff01e803000000000000220020dafb737a775241fc0d274a1352ec1a238d364a10e915aef4e04b131334aff7f500000000",
            txid: "a0dbacd99420b9fdd08623382efc474174e289cd84765097057a24d2cf67b108",
            inputs: vec![10_000],
            outputs: vec![1_000],
            vsize: 201,
            weight: 804,
            fee: 9_000,
        });
}

#[test]
fn test_bitcoin_send_to_p2wpkh_address() {
    let alice_private_key = "57a64865bce5d4855e99b1cce13327c46171434f2d72eeaf9da53ee075e7f90a"
        .decode_hex()
        .unwrap();
    let alice_pubkey = "028d7dce6d72fb8f7af9566616c6436349c67ad379f2404dd66fe7085fe0fba28f"
        .decode_hex()
        .unwrap();
    let bob_address = "bc1qp58pemrv9w473wkauh5m8h4xvldfqqmdk7s5ju";

    let txid = "181c84965c9ea86a5fac32fdbd5f73a21a7a9e749fb6ab97e273af2329f6b911";
    let tx1 = Proto::Input {
        out_point: input::out_point(txid, 0),
        value: 10_000,
        sighash_type: SIGHASH_ALL,
        claiming_script: input::p2pkh(alice_pubkey),
        ..Default::default()
    };

    // The output variant is derived from the specified address.
    let out1 = Proto::Output {
        value: 1_000,
        to_recipient: output::to_address(bob_address),
    };

    let builder = Proto::TransactionBuilder {
        version: Proto::TransactionVersion::V2,
        inputs: vec![tx1],
        outputs: vec![out1],
        input_selector: Proto::InputSelector::UseAll,
        dust_policy: dust_threshold(DUST),
        ..Default::default()
    };

    let signing = Proto::SigningInput {
        private_keys: vec![alice_private_key.into()],
        transaction: TransactionOneof::builder(builder),
        ..Default::default()
    };

    sign::BitcoinSignHelper::new(&signing)
        .coin(CoinType::Bitcoin)
        .sign(sign::Expected {
            encoded: "020000000111b9f62923af73e297abb69f749e7a1aa2735fbdfd32ac5f6aa89e5c96841c18000000006b483045022100cdee6087a59c27f670d04d2782d139b202abdb936643bff8c0a46ec726bbc7ec022072241248659a582ef3460bdd232936fcea77b25b0e024f3841f8ba5e89efd31a0121028d7dce6d72fb8f7af9566616c6436349c67ad379f2404dd66fe7085fe0fba28fffffffff01e8030000000000001600140d0e1cec6c2babe8badde5e9b3dea667da90036d00000000",
            txid: "711566e32e03c01106f019bf1911958979441f5aec5f58b7a368549d4cac569c",
            inputs: vec![10_000],
            outputs: vec![1_000],
            vsize: 189,
            weight: 756,
            fee: 9_000,
        });
}

#[test]
fn test_bitcoin_send_to_p2tr_key_path_address() {
    let alice_private_key = "57a64865bce5d4855e99b1cce13327c46171434f2d72eeaf9da53ee075e7f90a"
        .decode_hex()
        .unwrap();
    let alice_pubkey = "028d7dce6d72fb8f7af9566616c6436349c67ad379f2404dd66fe7085fe0fba28f"
        .decode_hex()
        .unwrap();
    let bob_address = "bc1pps3sf8cl8xuy42s9gnyacez9wlwjxpyf32lrxlac7fw4wnuf6e9s0v066l";

    let txid = "181c84965c9ea86a5fac32fdbd5f73a21a7a9e749fb6ab97e273af2329f6b911";
    let tx1 = Proto::Input {
        out_point: input::out_point(txid, 0),
        value: 10_000,
        sighash_type: SIGHASH_ALL,
        claiming_script: input::p2pkh(alice_pubkey),
        ..Default::default()
    };

    // The output variant is derived from the specified address.
    let out1 = Proto::Output {
        value: 1_000,
        to_recipient: output::to_address(bob_address),
    };

    let builder = Proto::TransactionBuilder {
        version: Proto::TransactionVersion::V2,
        inputs: vec![tx1],
        outputs: vec![out1],
        input_selector: Proto::InputSelector::UseAll,
        dust_policy: dust_threshold(DUST),
        ..Default::default()
    };

    let signing = Proto::SigningInput {
        private_keys: vec![alice_private_key.into()],
        transaction: TransactionOneof::builder(builder),
        ..Default::default()
    };

    sign::BitcoinSignHelper::new(&signing)
        .coin(CoinType::Bitcoin)
        .sign(sign::Expected {
            encoded: "020000000111b9f62923af73e297abb69f749e7a1aa2735fbdfd32ac5f6aa89e5c96841c18000000006b483045022100c85b0030e0f18fdb6bf4b6085945732ed6354aa45beb4da3cc8fb76378ab424a02204aef0ccfd9270549c302ca8fca2aed9e6d459b07e7a1af8477f2137e5ca070f30121028d7dce6d72fb8f7af9566616c6436349c67ad379f2404dd66fe7085fe0fba28fffffffff01e8030000000000002251200c23049f1f39b84aaa0544c9dc644577dd2304898abe337fb8f25d574f89d64b00000000",
            txid: "b37fdb67c208aeb9a42b329eeafe52d9ff870341c8bdc2e09b2a8de64ea13d2a",
            inputs: vec![10_000],
            outputs: vec![1_000],
            vsize: 201,
            weight: 804,
            fee: 9_000,
        });
}
