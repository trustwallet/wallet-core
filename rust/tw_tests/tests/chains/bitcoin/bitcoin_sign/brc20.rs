use crate::chains::common::bitcoin::{
    btc_info, dust_threshold, input, output, plan, sign, TransactionOneof, DUST, SIGHASH_ALL,
};
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::DecodeHex;
use tw_keypair::{ecdsa, schnorr};
use tw_misc::traits::ToBytesVec;
use tw_proto::BitcoinV2::Proto;

const ALICE_PRIVATE_KEY: &str = "e253373989199da27c48680e3a3fc0f648d50f9a727ef17a7fe6a4dc3b159129";

#[test]
fn test_bitcoin_sign_brc20_commit() {
    let alice_private_key = ecdsa::secp256k1::PrivateKey::try_from(ALICE_PRIVATE_KEY).unwrap();
    let alice_pubkey = alice_private_key.public().compressed();

    let txid = "8ec895b4d30adb01e38471ca1019bfc8c3e5fbd1f28d9e7b5653260d89989008";
    let tx1 = Proto::Input {
        out_point: input::out_point(txid, 1),
        value: 26_400,
        sighash_type: SIGHASH_ALL,
        claiming_script: input::p2wpkh(alice_pubkey.to_vec()),
        ..Default::default()
    };

    let out1 = Proto::Output {
        value: 7_000,
        to_recipient: output::brc20_inscribe(alice_pubkey.to_vec(), "oadf", "20"),
    };

    // Change/return transaction.
    let out2 = Proto::Output {
        value: 16_400,
        to_recipient: output::p2wpkh(alice_pubkey.to_vec()),
    };

    let builder = Proto::TransactionBuilder {
        version: Proto::TransactionVersion::V2,
        inputs: vec![tx1],
        outputs: vec![out1, out2],
        input_selector: Proto::InputSelector::UseAll,
        dust_policy: dust_threshold(DUST),
        ..Default::default()
    };

    let signing = Proto::SigningInput {
        private_keys: vec![ALICE_PRIVATE_KEY.decode_hex().unwrap().into()],
        chain_info: btc_info(),
        transaction: TransactionOneof::builder(builder),
        ..Default::default()
    };

    // https://www.blockchain.com/explorer/transactions/btc/797d17d47ae66e598341f9dfdea020b04d4017dcf9cc33f0e51f7a6082171fb1
    let txid = "797d17d47ae66e598341f9dfdea020b04d4017dcf9cc33f0e51f7a6082171fb1";
    sign::BitcoinSignHelper::new(&signing)
        .coin(CoinType::Bitcoin)
        .sign(sign::Expected {
            encoded: "02000000000101089098890d2653567b9e8df2d1fbe5c3c8bf1910ca7184e301db0ad3b495c88e0100000000ffffffff02581b000000000000225120e8b706a97732e705e22ae7710703e7f589ed13c636324461afa443016134cc051040000000000000160014e311b8d6ddff856ce8e9a4e03bc6d4fe5050a83d02483045022100a44aa28446a9a886b378a4a65e32ad9a3108870bd725dc6105160bed4f317097022069e9de36422e4ce2e42b39884aa5f626f8f94194d1013007d5a1ea9220a06dce0121030f209b6ada5edb42c77fd2bc64ad650ae38314c8f451f3e36d80bc8e26f132cb00000000",
            txid,
            inputs: vec![26_400],
            outputs: vec![7_000, 16_400],
            vsize: 153,
            weight: 610,
            fee: 3000,
        });
}

#[test]
fn test_bitcoin_sign_brc20_reveal() {
    let alice_private_key = schnorr::PrivateKey::try_from(ALICE_PRIVATE_KEY).unwrap();
    let alice_pubkey = alice_private_key.public().compressed();

    // Now spend just created `797d17d47ae66e598341f9dfdea020b04d4017dcf9cc33f0e51f7a6082171fb1` commit output.
    let txid = "797d17d47ae66e598341f9dfdea020b04d4017dcf9cc33f0e51f7a6082171fb1";
    let tx1 = Proto::Input {
        out_point: input::out_point(txid, 0),
        value: 7_000,
        sighash_type: SIGHASH_ALL,
        claiming_script: input::brc20_inscribe(alice_pubkey.to_vec(), "oadf", "20"),
        ..Default::default()
    };

    let out1 = Proto::Output {
        value: DUST,
        to_recipient: output::p2wpkh(alice_pubkey.to_vec()),
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
        private_keys: vec![ALICE_PRIVATE_KEY.decode_hex().unwrap().into()],
        chain_info: btc_info(),
        // We enable deterministic Schnorr signatures here
        dangerous_use_fixed_schnorr_rng: true,
        transaction: TransactionOneof::builder(builder),
        ..Default::default()
    };

    plan::BitcoinPlanHelper::new(&signing)
        .coin(CoinType::Bitcoin)
        .plan(plan::Expected {
            inputs: vec![7_000],
            outputs: vec![DUST],
            vsize_estimate: 131,
            fee_estimate: 7_000 - DUST,
            change: 0,
        });

    // https://www.blockchain.com/explorer/transactions/btc/7046dc2689a27e143ea2ad1039710885147e9485ab6453fa7e87464aa7dd3eca
    sign::BitcoinSignHelper::new(&signing)
        .coin(CoinType::Bitcoin)
        .sign(sign::Expected {
            encoded: "02000000000101b11f1782607a1fe5f033ccf9dc17404db020a0dedff94183596ee67ad4177d790000000000ffffffff012202000000000000160014e311b8d6ddff856ce8e9a4e03bc6d4fe5050a83d03406a35548b8fa4620028e021a944c1d3dc6e947243a7bfc901bf63fefae0d2460efa149a6440cab51966aa4f09faef2d1e5efcba23ab4ca6e669da598022dbcfe35b0063036f7264010118746578742f706c61696e3b636861727365743d7574662d3800377b2270223a226272632d3230222c226f70223a227472616e73666572222c227469636b223a226f616466222c22616d74223a223230227d6821c00f209b6ada5edb42c77fd2bc64ad650ae38314c8f451f3e36d80bc8e26f132cb00000000",
            txid: "7046dc2689a27e143ea2ad1039710885147e9485ab6453fa7e87464aa7dd3eca",
            inputs: vec![7_000],
            outputs: vec![DUST],
            // `vsize` is different from the estimated value due to the signatures der serialization.
            vsize: 131,
            weight: 522,
            fee: 7_000 - DUST,
        });
}

#[test]
fn test_bitcoin_sign_brc20_transfer() {
    let alice_private_key = schnorr::PrivateKey::try_from(ALICE_PRIVATE_KEY).unwrap();
    let alice_pubkey = alice_private_key.public().compressed();
    let bob_address = "bc1qazgc2zhu2kmy42py0vs8d7yff67l3zgpwfzlpk";

    // Now spend just created `7046dc2689a27e143ea2ad1039710885147e9485ab6453fa7e87464aa7dd3eca` reveal output.
    let txid1 = "7046dc2689a27e143ea2ad1039710885147e9485ab6453fa7e87464aa7dd3eca";
    let tx1 = Proto::Input {
        out_point: input::out_point(txid1, 0),
        value: DUST,
        sighash_type: SIGHASH_ALL,
        claiming_script: input::p2wpkh(alice_pubkey.to_vec()),
        ..Default::default()
    };

    // UTXO to cover fee.
    let txid2 = "797d17d47ae66e598341f9dfdea020b04d4017dcf9cc33f0e51f7a6082171fb1";
    let tx2 = Proto::Input {
        out_point: input::out_point(txid2, 1),
        value: 16_400,
        sighash_type: SIGHASH_ALL,
        claiming_script: input::p2wpkh(alice_pubkey.to_vec()),
        ..Default::default()
    };

    let out1 = Proto::Output {
        value: DUST,
        to_recipient: output::to_address(bob_address),
    };

    let change_output = Proto::Output {
        value: 13_400,
        to_recipient: output::p2wpkh(alice_pubkey.to_vec()),
    };

    let builder = Proto::TransactionBuilder {
        version: Proto::TransactionVersion::V2,
        inputs: vec![tx1, tx2],
        outputs: vec![out1, change_output],
        input_selector: Proto::InputSelector::UseAll,
        // We enable deterministic Schnorr signatures here
        dust_policy: dust_threshold(DUST),
        ..Default::default()
    };

    let signing = Proto::SigningInput {
        private_keys: vec![ALICE_PRIVATE_KEY.decode_hex().unwrap().into()],
        chain_info: btc_info(),
        // We enable deterministic Schnorr signatures here
        dangerous_use_fixed_schnorr_rng: true,
        transaction: TransactionOneof::builder(builder),
        ..Default::default()
    };

    // Successfully broadcasted: https://www.blockchain.com/explorer/transactions/btc/3e3576eb02667fac284a5ecfcb25768969680cc4c597784602d0a33ba7c654b7
    sign::BitcoinSignHelper::new(&signing)
        .coin(CoinType::Bitcoin)
        .sign(sign::Expected {
            encoded: "02000000000102ca3edda74a46877efa5364ab85947e148508713910ada23e147ea28926dc46700000000000ffffffffb11f1782607a1fe5f033ccf9dc17404db020a0dedff94183596ee67ad4177d790100000000ffffffff022202000000000000160014e891850afc55b64aa8247b2076f8894ebdf889015834000000000000160014e311b8d6ddff856ce8e9a4e03bc6d4fe5050a83d024830450221008798393eb0b7390217591a8c33abe18dd2f7ea7009766e0d833edeaec63f2ec302200cf876ff52e68dbaf108a3f6da250713a9b04949a8f1dcd1fb867b24052236950121030f209b6ada5edb42c77fd2bc64ad650ae38314c8f451f3e36d80bc8e26f132cb0248304502210096bbb9d1f0596d69875646689e46f29485e8ceccacde9d0025db87fd96d3066902206d6de2dd69d965d28df3441b94c76e812384ab9297e69afe3480ee4031e1b2060121030f209b6ada5edb42c77fd2bc64ad650ae38314c8f451f3e36d80bc8e26f132cb00000000",
            txid: "3e3576eb02667fac284a5ecfcb25768969680cc4c597784602d0a33ba7c654b7",
            inputs: vec![DUST, 16_400],
            outputs: vec![DUST, 13_400],
            vsize: 209,
            weight: 834,
            fee: 3000,
        });
}

/// Fixes `{"error":"-26: non-mandatory-script-verify-flag (Invalid Schnorr signature)"}` error.
#[test]
fn test_bitcoin_sign_brc20_reveal_with_extra_p2tr_input() {
    // bc1puq428nh4eynlqph8gynwdtqg4je0hc03gp2ptgsf4c5ylxz0ll2sd34gk7
    let alice_pk_bytes = "8efa479919269076eb331c304fff187b9d7aa60d1f6cd3d6b12a151a52f22582"
        .decode_hex()
        .unwrap();
    let alice_private_key = schnorr::PrivateKey::try_from(alice_pk_bytes.as_slice()).unwrap();
    let alice_pubkey = alice_private_key.public().compressed();
    let my_address = "bc1puq428nh4eynlqph8gynwdtqg4je0hc03gp2ptgsf4c5ylxz0ll2sd34gk7";

    let commit_txid = "164b459a49c5e3a817028df7f4545585874feff3985e48d6ff6792989a4823a8";
    let commit_utxo = Proto::Input {
        out_point: input::out_point(commit_txid, 0),
        value: 546,
        sighash_type: SIGHASH_ALL,
        claiming_script: input::brc20_inscribe(alice_pubkey.to_vec(), "duna", "0.001"),
        ..Default::default()
    };

    // Extra P2TR UTXO is used to cover transaction fee.
    let p2tr_utxo = "164b459a49c5e3a817028df7f4545585874feff3985e48d6ff6792989a4823a8";
    let extra_p2tr = Proto::Input {
        out_point: input::out_point(p2tr_utxo, 1),
        value: 11_210,
        sighash_type: SIGHASH_ALL,
        claiming_script: input::receiver_address(my_address),
        ..Default::default()
    };

    let out1 = Proto::Output {
        value: 546,
        to_recipient: output::to_address(my_address),
    };
    let change_output = Proto::Output {
        value: 0,
        to_recipient: output::to_address(my_address),
    };

    let builder = Proto::TransactionBuilder {
        version: Proto::TransactionVersion::V2,
        inputs: vec![commit_utxo, extra_p2tr],
        outputs: vec![out1],
        change_output: Some(change_output),
        input_selector: Proto::InputSelector::UseAll,
        dust_policy: dust_threshold(DUST),
        fee_per_vb: 9,
        ..Default::default()
    };

    let signing = Proto::SigningInput {
        private_keys: vec![alice_pk_bytes.into()],
        chain_info: btc_info(),
        // We enable deterministic Schnorr signatures here
        dangerous_use_fixed_schnorr_rng: true,
        transaction: TransactionOneof::builder(builder),
        ..Default::default()
    };

    // https://www.blockchain.com/explorer/transactions/btc/113dfc827e4535dccc6aa7fcff5482b4de0fb2ab70f52c44c12c12bca3be5847
    sign::BitcoinSignHelper::new(&signing)
        .coin(CoinType::Bitcoin)
        .sign(sign::Expected {
            encoded: "02000000000102a823489a989267ffd6485e98f3ef4f87855554f4f78d0217a8e3c5499a454b160000000000ffffffffa823489a989267ffd6485e98f3ef4f87855554f4f78d0217a8e3c5499a454b160100000000ffffffff022202000000000000225120e02aa3cef5c927f006e74126e6ac08acb2fbe1f1405415a209ae284f984fffd52d23000000000000225120e02aa3cef5c927f006e74126e6ac08acb2fbe1f1405415a209ae284f984fffd50340b90b099a8facd5d4e6008990d180f9afeeb07d62452570a6e700ca0f7968577da6113cb201e9ac3584caa04898cdc7113cce4df06604b8f294a3959d216d364f5e0063036f7264010118746578742f706c61696e3b636861727365743d7574662d38003a7b2270223a226272632d3230222c226f70223a227472616e73666572222c227469636b223a2264756e61222c22616d74223a22302e303031227d6821c02146f58256fcc00ef86a0e53fc14e943bbea2c7972b598b58178fdd6fa3ef79201401c5e54a0ead877e52146e42f8d197f4c7be84c7d2479a75f33128f15777584bfec410c3e130b4504fe061991a78365add223a3dfb5ca79a988f9ffcf46039b3100000000",
            txid: "113dfc827e4535dccc6aa7fcff5482b4de0fb2ab70f52c44c12c12bca3be5847",
            inputs: vec![546, 11_210],
            outputs: vec![546, 9_005],
            // `vsize` is different from the estimated value due to the signatures der serialization.
            vsize: 244,
            weight: 975,
            fee: 2_205,
        });
}
