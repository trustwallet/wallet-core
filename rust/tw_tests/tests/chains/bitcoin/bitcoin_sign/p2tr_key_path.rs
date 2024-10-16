use crate::chains::common::bitcoin::{
    btc_info, dust_threshold, input, output, sign, TransactionOneof, BITCOIN_HRP, DUST, MINER_FEE,
    ONE_BTC, SIGHASH_ALL,
};
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::DecodeHex;
use tw_keypair::{ecdsa, schnorr};
use tw_misc::traits::ToBytesVec;
use tw_proto::BitcoinV2::Proto;
use tw_utxo::address::taproot::TaprootAddress;
use tw_utxo::script::standard_script::conditions;

const ALICE_PRIVATE_KEY: &str = "12ce558df23528f1aa86f1f51ac7e13a197a06bda27610fa89e13b04c40ee999";
const BOB_PRIVATE_KEY: &str = "26c2566adcc030a1799213bfd546e615f6ab06f72085ec6806ff1761da48d227";

fn p2tr_key_path_address(internal_pubkey: &schnorr::PublicKey) -> String {
    let pubkey = internal_pubkey.compressed();
    let merkle_root = None;
    TaprootAddress::p2tr_with_public_key(BITCOIN_HRP.to_string(), &pubkey, merkle_root)
        .unwrap()
        .to_string()
}

enum P2TRRecipientType {
    KeyPath,
    KeyPathAssumeTweaked,
    P2TRAddress,
}

fn test_bitcoin_sign_input_p2pkh_output_p2tr_key_path(send_to: P2TRRecipientType) {
    let alice_private_key = schnorr::PrivateKey::try_from(ALICE_PRIVATE_KEY).unwrap();
    let alice_pubkey = alice_private_key.public();

    let bob_private_key = schnorr::PrivateKey::try_from(BOB_PRIVATE_KEY).unwrap();
    let bob_pubkey = bob_private_key.public();

    let txid = "c50563913e5a838f937c94232f5a8fc74e58b629fae41dfdffcc9a70f833b53a";
    let tx1 = Proto::Input {
        out_point: input::out_point(txid, 0),
        value: ONE_BTC * 50,
        sighash_type: SIGHASH_ALL,
        claiming_script: input::p2pkh(alice_pubkey.to_vec()),
        ..Default::default()
    };

    let to_recipient = match send_to {
        P2TRRecipientType::KeyPath => output::p2tr_key_path(bob_pubkey.to_vec()),
        P2TRRecipientType::KeyPathAssumeTweaked => {
            let bob_pubkey_tweaked = bob_pubkey.tweak(None).x_only();
            output::p2tr_assume_tweaked(bob_pubkey_tweaked.bytes().to_vec())
        },
        P2TRRecipientType::P2TRAddress => output::to_address(&p2tr_key_path_address(&bob_pubkey)),
    };

    let out1 = Proto::Output {
        value: ONE_BTC * 50 - MINER_FEE,
        to_recipient,
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
        transaction: TransactionOneof::builder(builder),
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
}

#[test]
fn test_bitcoin_sign_output_p2tr_key_path() {
    test_bitcoin_sign_input_p2pkh_output_p2tr_key_path(P2TRRecipientType::KeyPath);
}

#[test]
fn test_bitcoin_sign_output_p2tr_assume_tweaked() {
    test_bitcoin_sign_input_p2pkh_output_p2tr_key_path(P2TRRecipientType::KeyPathAssumeTweaked);
}

#[test]
fn test_bitcoin_sign_output_p2tr_address() {
    test_bitcoin_sign_input_p2pkh_output_p2tr_key_path(P2TRRecipientType::P2TRAddress);
}

enum P2TRClaimingScriptType {
    KeyPath,
    P2TRAddress,
    // Direct scriptPubkey with the witness program.
    P2TRCustomScript,
}

fn test_bitcoin_sign_output_p2tr(utxo_owner: P2TRClaimingScriptType) {
    let alice_private_key = schnorr::PrivateKey::try_from(ALICE_PRIVATE_KEY).unwrap();
    let alice_pubkey = alice_private_key.public();

    let bob_private_key = schnorr::PrivateKey::try_from(BOB_PRIVATE_KEY).unwrap();
    let bob_pubkey = bob_private_key.public();

    let claiming_script = match utxo_owner {
        P2TRClaimingScriptType::KeyPath => input::p2tr_key_path(bob_pubkey.to_vec()),
        P2TRClaimingScriptType::P2TRAddress => {
            input::receiver_address(&p2tr_key_path_address(&bob_pubkey))
        },
        P2TRClaimingScriptType::P2TRCustomScript => {
            let custom_script = conditions::new_p2tr_key_path(&bob_pubkey.compressed());
            input::custom_script(custom_script.to_vec())
        },
    };

    // Now spend `9a582032f6a50cedaff77d3d5604b33adf8bc31bdaef8de977c2187e395860ac` output
    // created at `test_bitcoin_sign_input_p2pkh_output_p2tr_key_path` test.
    let txid = "9a582032f6a50cedaff77d3d5604b33adf8bc31bdaef8de977c2187e395860ac";
    let tx1 = Proto::Input {
        out_point: input::out_point(txid, 0),
        value: ONE_BTC * 50 - MINER_FEE,
        sighash_type: SIGHASH_ALL,
        claiming_script,
        ..Default::default()
    };

    let out1 = Proto::Output {
        value: ONE_BTC * 50 - MINER_FEE - MINER_FEE,
        to_recipient: output::p2tr_key_path(alice_pubkey.to_vec()),
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
        private_keys: vec![BOB_PRIVATE_KEY.decode_hex().unwrap().into()],
        chain_info: btc_info(),
        // We enable deterministic Schnorr signatures here
        dangerous_use_fixed_schnorr_rng: true,
        transaction: TransactionOneof::builder(builder),
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

#[test]
fn test_bitcoin_sign_input_p2tr_key_path() {
    test_bitcoin_sign_output_p2tr(P2TRClaimingScriptType::KeyPath);
}

#[test]
fn test_bitcoin_sign_input_p2tr_address() {
    test_bitcoin_sign_output_p2tr(P2TRClaimingScriptType::P2TRAddress);
}

#[test]
fn test_bitcoin_sign_input_p2tr_custom_script() {
    test_bitcoin_sign_output_p2tr(P2TRClaimingScriptType::P2TRCustomScript);
}

/// Transfer from P2TR to P2WPKH address.
#[test]
fn test_bitcoin_sign_input_p2tr_key_path_with_change_output_a9c63d() {
    const PRIVATE_KEY: &str = "7fa638b0df495b2968ae6dc7011c4db08c86df16c91aa71a77ee6a222954e5bb";
    const SEND_TO: &str = "bc1qtaquch7d90x37qre6f75z5a6l0luzh0c03epyz";

    let my_private_key = schnorr::PrivateKey::try_from(PRIVATE_KEY).unwrap();
    let my_public_key = my_private_key.public();

    let txid = "75ed78f0ae2bad924065d2357ef01184ceee2181c44e03337746512be9371a82";
    let utxo0 = Proto::Input {
        out_point: input::out_point(txid, 1),
        value: 8_802,
        sighash_type: SIGHASH_ALL,
        claiming_script: input::p2tr_key_path(my_public_key.to_vec()),
        ..Default::default()
    };

    let out0 = Proto::Output {
        value: 3_000,
        to_recipient: output::to_address(SEND_TO),
    };
    // Send the change amount back to the same P2TR address.
    // The correct amount will be calculated for us.
    let change_output = Proto::Output {
        to_recipient: output::p2tr_key_path(my_public_key.to_vec()),
        ..Proto::Output::default()
    };

    let builder = Proto::TransactionBuilder {
        version: Proto::TransactionVersion::V2,
        inputs: vec![utxo0],
        outputs: vec![out0],
        input_selector: Proto::InputSelector::SelectDescending,
        fee_per_vb: 8,
        change_output: Some(change_output),
        dust_policy: dust_threshold(DUST),
        ..Default::default()
    };

    let signing = Proto::SigningInput {
        private_keys: vec![PRIVATE_KEY.decode_hex().unwrap().into()],
        chain_info: btc_info(),
        dangerous_use_fixed_schnorr_rng: true,
        transaction: TransactionOneof::builder(builder),
        ..Default::default()
    };

    // Successfully broadcasted: https://mempool.space/tx/a9c63dfe54f6ff462155d966a54226c456b3e43b52a9abe55d7fa87d6564c6e4
    let txid = "a9c63dfe54f6ff462155d966a54226c456b3e43b52a9abe55d7fa87d6564c6e4";
    sign::BitcoinSignHelper::new(&signing)
        .coin(CoinType::Bitcoin)
        .sign(sign::Expected {
            encoded: "02000000000101821a37e92b51467733034ec48121eece8411f07e35d2654092ad2baef078ed750100000000ffffffff02b80b0000000000001600145f41cc5fcd2bcd1f0079d27d4153bafbffc15df83212000000000000225120412a773e0bba5cfb5462d024cd4bf2cce1b8688a9e7a7a3f8507ebba8f00de580140cbe4d13bc9e067b042179e2c217e4e4b1d552119d12839aa4df11c21282f9159e2c4b58a4f22b291c200c0d0c5f277902282bdd78589dff0edbea89d3f00d77400000000",
            txid,
            inputs: vec![8_802],
            outputs: vec![3_000, 4_658],
            vsize: 142,
            weight: 568,
            fee: 1_144,
        });
}

#[test]
fn test_bitcoin_sign_input_p2tr_key_path_with_max_amount_89c5d1() {
    const PRIVATE_KEY: &str = "7fa638b0df495b2968ae6dc7011c4db08c86df16c91aa71a77ee6a222954e5bb";
    const SEND_TO: &str = "bc1qtaquch7d90x37qre6f75z5a6l0luzh0c03epyz";

    let my_private_key = schnorr::PrivateKey::try_from(PRIVATE_KEY).unwrap();
    let my_public_key = my_private_key.public();

    let txid = "a9c63dfe54f6ff462155d966a54226c456b3e43b52a9abe55d7fa87d6564c6e4";
    let utxo0 = Proto::Input {
        out_point: input::out_point(txid, 1),
        value: 4_658,
        sighash_type: SIGHASH_ALL,
        claiming_script: input::p2tr_key_path(my_public_key.to_vec()),
        ..Default::default()
    };

    // Send max amount to an address.
    let max_output = Proto::Output {
        to_recipient: output::to_address(SEND_TO),
        ..Proto::Output::default()
    };

    let builder = Proto::TransactionBuilder {
        version: Proto::TransactionVersion::V2,
        inputs: vec![utxo0],
        input_selector: Proto::InputSelector::SelectDescending,
        fee_per_vb: 6,
        max_amount_output: Some(max_output),
        dust_policy: dust_threshold(DUST),
        ..Default::default()
    };

    let signing = Proto::SigningInput {
        private_keys: vec![PRIVATE_KEY.decode_hex().unwrap().into()],
        chain_info: btc_info(),
        dangerous_use_fixed_schnorr_rng: true,
        transaction: TransactionOneof::builder(builder),
        ..Default::default()
    };

    // Successfully broadcasted: https://mempool.space/tx/89c5d1d6242677a409c20fee95d6c7f7f397b82f707c96cca8e83308c4400f07
    let txid = "89c5d1d6242677a409c20fee95d6c7f7f397b82f707c96cca8e83308c4400f07";
    sign::BitcoinSignHelper::new(&signing)
        .coin(CoinType::Bitcoin)
        .sign(sign::Expected {
            encoded: "02000000000101e4c664657da87f5de5aba9523be4b356c42642a566d9552146fff654fe3dc6a90100000000ffffffff01da0f0000000000001600145f41cc5fcd2bcd1f0079d27d4153bafbffc15df80140f480cb921aeae4ab03e0455da56517b6a9f2f90145e4432e0c1c39b1c06a42aa8265b30bed2d4185de2302faa89cb2a664a8a38b7fe9808a926263b7bf7cffb800000000",
            txid,
            inputs: vec![4_658],
            outputs: vec![4_058],
            vsize: 99,
            weight: 396,
            fee: 600,
        });
}

#[test]
fn test_bitcoin_sign_input_p2tr_and_p2wpkh() {
    const P2TR_PRIVATE_KEY: &str =
        "2481de1ce115aa2f0ae9066046baf37256db97958aa7a5ac26c6d8ed0a48e88c";
    const P2WPKH_PRIVATE_KEY: &str =
        "25834fabebf75c15d5852aed768894e6f49853023df586e3dd5e83b42a9e2e2b";
    const SEND_TO: &str = "bc1q8jqgv4us4pluu8nql9ze66y6rs6kxwk5tq678a";

    let p2tr_private_key = schnorr::PrivateKey::try_from(P2TR_PRIVATE_KEY).unwrap();
    let p2wpkh_private_key = ecdsa::secp256k1::PrivateKey::try_from(P2WPKH_PRIVATE_KEY).unwrap();

    let txid = "0200c9a11371465e5d02fdb86fd13f2fa76561fd9d7d14929f31b8f919217995";
    let p2tr_utxo = Proto::Input {
        out_point: input::out_point(txid, 0),
        value: 5_817,
        sighash_type: SIGHASH_ALL,
        // Spend a P2TR UTXO.
        claiming_script: input::p2tr_key_path(p2tr_private_key.public().to_vec()),
        ..Default::default()
    };

    let txid = "aff21919411e1988755768c2f7d2c34b7451a12f81c333d7609f6f03f408f2f0";
    let p2wpkh_utxo = Proto::Input {
        out_point: input::out_point(txid, 1),
        value: 4_500,
        sighash_type: SIGHASH_ALL,
        // Spend a P2WPKH UTXO.
        claiming_script: input::p2wpkh(p2wpkh_private_key.public().to_vec()),
        ..Default::default()
    };

    // Send max amount to a P2WPKH address.
    let max_output = Proto::Output {
        to_recipient: output::to_address(SEND_TO),
        ..Proto::Output::default()
    };

    let builder = Proto::TransactionBuilder {
        version: Proto::TransactionVersion::V2,
        inputs: vec![p2tr_utxo, p2wpkh_utxo],
        input_selector: Proto::InputSelector::UseAll,
        fee_per_vb: 4,
        max_amount_output: Some(max_output),
        dust_policy: dust_threshold(DUST),
        ..Default::default()
    };

    let signing = Proto::SigningInput {
        private_keys: vec![
            P2TR_PRIVATE_KEY.decode_hex().unwrap().into(),
            P2WPKH_PRIVATE_KEY.decode_hex().unwrap().into(),
        ],
        chain_info: btc_info(),
        dangerous_use_fixed_schnorr_rng: true,
        transaction: TransactionOneof::builder(builder),
        ..Default::default()
    };

    // Successfully broadcasted: https://mempool.space/tx/bfc782da443774b9cf35e6d59b08312be311d791b4e802136fff88adc2312d28
    sign::BitcoinSignHelper::new(&signing)
        .coin(CoinType::Bitcoin)
        .sign(sign::Expected {
            encoded: "0200000000010295792119f9b8319f92147d9dfd6165a72f3fd16fb8fd025d5e467113a1c900020000000000fffffffff0f208f4036f9f60d733c3812fa151744bc3d2f7c268577588191e411919f2af0100000000ffffffff01ad250000000000001600143c80865790a87fce1e60f9459d689a1c35633ad40140d8cd111da08d7863366d4ff23f6b3be3ce7c7496ecabdbb6a275b1591270767e163d091b208190abae3075f24f11b1291656732369e3f9a14efdf131e481701302473044022021df9a043a886a9e7068fe06a19d8aa0a4a5a21c4b4e50abdbc6aeede32ca494022049da5e9082d83e6108027f4d63965cee50a2c9b619e603692748388150b3bf840121037ef29d31b889dfbae30cff4e996f742a49aae10a03fb6f992048c8d366b4b7c900000000",
            txid: "bfc782da443774b9cf35e6d59b08312be311d791b4e802136fff88adc2312d28",
            inputs: vec![5_817, 4_500],
            outputs: vec![9_645],
            vsize: 167,
            weight: 667,
            fee: 672,
        });
}
