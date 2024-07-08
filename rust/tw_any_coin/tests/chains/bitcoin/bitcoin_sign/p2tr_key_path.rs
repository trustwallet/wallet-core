use crate::chains::common::bitcoin::{
    btc_info, dust_threshold, input, output, sign, BITCOIN_HRP, DUST, MINER_FEE, ONE_BTC,
    SIGHASH_ALL,
};
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::DecodeHex;
use tw_keypair::schnorr;
use tw_misc::traits::ToBytesVec;
use tw_proto::BitcoinV3::Proto;
use tw_utxo::address::taproot::TaprootAddress;

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

    let signing = Proto::SigningInput {
        version: Proto::TransactionVersion::V2,
        private_keys: vec![ALICE_PRIVATE_KEY.decode_hex().unwrap().into()],
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

    let signing = Proto::SigningInput {
        version: Proto::TransactionVersion::V2,
        private_keys: vec![BOB_PRIVATE_KEY.decode_hex().unwrap().into()],
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

#[test]
fn test_bitcoin_sign_input_p2tr_key_path() {
    test_bitcoin_sign_output_p2tr(P2TRClaimingScriptType::KeyPath);
}

#[test]
fn test_bitcoin_sign_input_p2tr_address() {
    test_bitcoin_sign_output_p2tr(P2TRClaimingScriptType::P2TRAddress);
}
