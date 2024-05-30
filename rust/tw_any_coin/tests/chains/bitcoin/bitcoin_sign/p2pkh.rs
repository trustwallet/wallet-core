use crate::chains::common::bitcoin::{
    btc_info, dust_threshold, input, output, plan, sign, DUST, MINER_FEE, ONE_BTC, SIGHASH_ALL,
};
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::DecodeHex;
use tw_proto::BitcoinV3::Proto;

/// Note this test contains a sample transaction that has never been broadcasted.
#[test]
fn coin_entry_sign_input_p2pkh_output_p2pkh() {
    let alice_private_key = "56429688a1a6b00b90ccd22a0de0a376b6569d8684022ae92229a28478bfb657"
        .decode_hex()
        .unwrap();
    let alice_pubkey = "036666dd712e05a487916384bfcd5973eb53e8038eccbbf97f7eed775b87389536"
        .decode_hex()
        .unwrap();
    let bob_pubkey = "037ed9a436e11ec4947ac4b7823787e24ba73180f1edd2857bff19c9f4d62b65bf"
        .decode_hex()
        .unwrap();

    // Create transaction with P2PKH as input and output.
    let txid = "1e1cdc48aa990d7e154a161d5b5f1cad737742e97d2712ab188027bb42e6e47b";
    let tx1 = Proto::Input {
        out_point: input::out_point(txid, 0),
        value: ONE_BTC * 50,
        sighash_type: SIGHASH_ALL,
        claiming_script: input::p2pkh(alice_pubkey.clone()),
        ..Default::default()
    };

    let out1 = Proto::Output {
        value: ONE_BTC * 50 - MINER_FEE,
        to_recipient: output::p2pkh(bob_pubkey.clone()),
    };

    let signing = Proto::SigningInput {
        private_keys: vec![alice_private_key.into()],
        inputs: vec![tx1],
        outputs: vec![out1],
        input_selector: Proto::InputSelector::UseAll,
        chain_info: btc_info(),
        dust_policy: dust_threshold(DUST),
        ..Default::default()
    };

    plan::BitcoinPlanHelper::new(&signing)
        .coin(CoinType::Bitcoin)
        .plan(plan::Expected {
            inputs: vec![ONE_BTC * 50],
            outputs: vec![ONE_BTC * 50 - MINER_FEE],
            vsize_estimate: 193,
            // Fee includes potential change amount.
            // In this test we do not want to have a change output.
            fee_estimate: MINER_FEE,
            change: 0,
        });

    sign::BitcoinSignHelper::new(&signing)
        .coin(CoinType::Bitcoin)
        .sign(sign::Expected {
            encoded: "02000000017be4e642bb278018ab12277de9427773ad1c5f5b1d164a157e0d99aa48dc1c1e000000006a473044022078eda020d4b86fcb3af78ef919912e6d79b81164dbbb0b0b96da6ac58a2de4b102201a5fd8d48734d5a02371c4b5ee551a69dca3842edbf577d863cf8ae9fdbbd4590121036666dd712e05a487916384bfcd5973eb53e8038eccbbf97f7eed775b87389536ffffffff01c0aff629010000001976a9145eaaa4f458f9158f86afcba08dd7448d27045e3d88ac00000000",
            txid: "c19f410bf1d70864220e93bca20f836aaaf8cdde84a46692616e9f4480d54885",
            inputs: vec![ONE_BTC * 50],
            outputs: vec![ONE_BTC * 50 - MINER_FEE],
            // `vsize` is different from the estimated value due to the signatures der serialization.
            vsize: 191,
            fee: MINER_FEE,
        });
}
