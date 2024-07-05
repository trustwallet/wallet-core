use crate::chains::common::bitcoin::{
    btc_info, dust_threshold, input, output, sign, DUST, MINER_FEE, ONE_BTC, SIGHASH_ALL,
};
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::DecodeHex;
use tw_proto::BitcoinV3::Proto;

#[test]
fn coin_entry_sign_input_p2pkh_output_p2tr_key_path() {
    let alice_private_key = "12ce558df23528f1aa86f1f51ac7e13a197a06bda27610fa89e13b04c40ee999"
        .decode_hex()
        .unwrap();
    let alice_pubkey = "0351e003fdc48e7f31c9bc94996c91f6c3273b7ef4208a1686021bedf7673bb058"
        .decode_hex()
        .unwrap();
    let bob_private_key = "26c2566adcc030a1799213bfd546e615f6ab06f72085ec6806ff1761da48d227"
        .decode_hex()
        .unwrap();
    let bob_pubkey = "02c0938cf377023dfde55e9c96b3cff4ca8894fb6b5d2009006bd43c0bff69cac9"
        .decode_hex()
        .unwrap();

    let txid = "c50563913e5a838f937c94232f5a8fc74e58b629fae41dfdffcc9a70f833b53a";
    let tx1 = Proto::Input {
        out_point: input::out_point(txid, 0),
        value: ONE_BTC * 50,
        sighash_type: SIGHASH_ALL,
        claiming_script: input::p2pkh(alice_pubkey.clone()),
        ..Default::default()
    };

    let out1 = Proto::Output {
        value: ONE_BTC * 50 - MINER_FEE,
        to_recipient: output::p2tr_key_path(bob_pubkey.clone()),
    };

    let signing = Proto::SigningInput {
        version: Proto::TransactionVersion::V2,
        private_keys: vec![alice_private_key.into()],
        inputs: vec![tx1],
        outputs: vec![out1],
        input_selector: Proto::InputSelector::UseAll,
        chain_info: btc_info(),
        dust_policy: dust_threshold(DUST),
        ..Default::default()
    };

    let txid = "9a582032f6a50cedaff77d3d5604b33adf8bc31bdaef8de977c2187e395860ac";
    sign::BitcoinSignHelper::new(&signing)
        .coin(CoinType::Bitcoin)
        .sign(sign::Expected {
            encoded: "02000000013ab533f8709accfffd1de4fa29b6584ec78f5a2f23947c938f835a3e916305c5000000006b48304502210086ab2c2192e2738529d6cd9604d8ee75c5b09b0c2f4066a5c5fa3f87a26c0af602202afc7096aaa992235c43e712146057b5ed6a776d82b9129620bc5a21991c0a5301210351e003fdc48e7f31c9bc94996c91f6c3273b7ef4208a1686021bedf7673bb058ffffffff01c0aff62901000000225120e01cfdd05da8fa1d71f987373f3790d45dea9861acb0525c86656fe50f4397a600000000",
            txid,
            inputs: vec![ONE_BTC * 50],
            outputs: vec![ONE_BTC * 50 - MINER_FEE],
            vsize: 201,
            weight: 804,
            fee: MINER_FEE,
        });

    let tx1 = Proto::Input {
        // Now spend just created `9a582032f6a50cedaff77d3d5604b33adf8bc31bdaef8de977c2187e395860ac` output.
        out_point: input::out_point(txid, 0),
        value: ONE_BTC * 50 - MINER_FEE,
        sighash_type: SIGHASH_ALL,
        claiming_script: input::p2tr_key_path(bob_pubkey.clone()),
        ..Default::default()
    };

    let out1 = Proto::Output {
        value: ONE_BTC * 50 - MINER_FEE - MINER_FEE,
        to_recipient: output::p2tr_key_path(alice_pubkey),
    };

    let signing = Proto::SigningInput {
        version: Proto::TransactionVersion::V2,
        private_keys: vec![bob_private_key.into()],
        inputs: vec![tx1],
        outputs: vec![out1],
        input_selector: Proto::InputSelector::UseAll,
        chain_info: btc_info(),
        // We enable deterministic Schnorr signatures here
        dangerous_use_fixed_schnorr_rng: true,
        dust_policy: dust_threshold(DUST),
        ..Default::default()
    };

    sign::BitcoinSignHelper::new(&signing)
        .coin(CoinType::Bitcoin)
        .sign(sign::Expected {
            encoded: "02000000000101ac6058397e18c277e98defda1bc38bdf3ab304563d7df7afed0ca5f63220589a0000000000ffffffff01806de72901000000225120a5c027857e359d19f625e52a106b8ac6ca2d6a8728f6cf2107cd7958ee0787c20140ec2d3910d41506b60aaa20520bb72f15e2d2cbd97e3a8e26ee7bad5f4c56b0f2fb0ceaddac33cb2813a33ba017ba6b1d011bab74a0426f12a2bcf47b4ed5bc8600000000",
            txid: "1487204b521cd24d4e30e9998d2b0584ac8efd41d42bac041dc899abcf83ecdf",
            inputs: vec![ONE_BTC * 50 - MINER_FEE],
            outputs: vec![ONE_BTC * 50 - MINER_FEE - MINER_FEE],
            vsize: 111,
            weight: 444,
            fee: MINER_FEE,
        });
}
