use crate::chains::common::bitcoin::{
    btc_info, dust_threshold, input, output, plan, sign, BITCOIN_P2PKH_PREFIX, DUST, MINER_FEE,
    ONE_BTC, SIGHASH_ALL,
};
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::DecodeHex;
use tw_hash::hasher::sha256_ripemd;
use tw_keypair::ecdsa;
use tw_misc::traits::ToBytesVec;
use tw_proto::BitcoinV2::Proto;
use tw_utxo::address::legacy::LegacyAddress;

enum P2PKHClaimingScriptType {
    PublicKey,
    PublicKeyHash,
    P2PKHAddress,
}

/// Note this test contains a sample transaction that has never been broadcasted.
fn test_bitcoin_sign_input_p2pkh(utxo_owner: P2PKHClaimingScriptType) {
    const ALICE_PRIVATE_KEY: &str =
        "56429688a1a6b00b90ccd22a0de0a376b6569d8684022ae92229a28478bfb657";

    let alice_private_key = ecdsa::secp256k1::PrivateKey::try_from(ALICE_PRIVATE_KEY).unwrap();
    let alice_pubkey = alice_private_key.public();
    let bob_pubkey = "037ed9a436e11ec4947ac4b7823787e24ba73180f1edd2857bff19c9f4d62b65bf"
        .decode_hex()
        .unwrap();

    let claiming_script = match utxo_owner {
        P2PKHClaimingScriptType::PublicKey => input::p2pkh(alice_pubkey.to_vec()),
        P2PKHClaimingScriptType::PublicKeyHash => {
            let alice_pubkey_hash = sha256_ripemd(alice_pubkey.compressed().as_slice());
            input::p2pkh_with_hash(alice_pubkey_hash)
        },
        P2PKHClaimingScriptType::P2PKHAddress => {
            let alice_address =
                LegacyAddress::p2pkh_with_public_key(BITCOIN_P2PKH_PREFIX, &alice_pubkey).unwrap();
            input::receiver_address(&alice_address.to_string())
        },
    };

    // Create transaction with P2PKH as input and output.
    let txid = "1e1cdc48aa990d7e154a161d5b5f1cad737742e97d2712ab188027bb42e6e47b";
    let tx1 = Proto::Input {
        out_point: input::out_point(txid, 0),
        value: ONE_BTC * 50,
        sighash_type: SIGHASH_ALL,
        claiming_script,
        ..Default::default()
    };

    let out1 = Proto::Output {
        value: ONE_BTC * 50 - MINER_FEE,
        to_recipient: output::p2pkh(bob_pubkey.clone()),
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
            weight: 764,
            fee: MINER_FEE,
        });
}

/// Note this test contains a sample transaction that has never been broadcasted.
#[test]
fn test_bitcoin_sign_input_p2pkh_public_key() {
    test_bitcoin_sign_input_p2pkh(P2PKHClaimingScriptType::PublicKey);
}

/// Note this test contains a sample transaction that has never been broadcasted.
#[test]
fn test_bitcoin_sign_input_p2pkh_public_key_hash() {
    test_bitcoin_sign_input_p2pkh(P2PKHClaimingScriptType::PublicKeyHash);
}

/// Note this test contains a sample transaction that has never been broadcasted.
#[test]
fn test_bitcoin_sign_input_p2pkh_address() {
    test_bitcoin_sign_input_p2pkh(P2PKHClaimingScriptType::P2PKHAddress);
}

/// TODO consider moving this test to `bitcoin_cash`.
/// Original test: https://github.com/trustwallet/wallet-core/blob/cd5a27481d2181e63362cb57e2b2160506cce163/tests/chains/BitcoinCash/TWBitcoinCashTests.cpp#L122-L164
/// *Note* that if we enable change output, current selection algorithm will calculate
/// different change and fee amounts than in the original test.
#[test]
fn test_bitcoin_cash_sign_input_p2pkh_custom_script() {
    const BCH_SIGHASH_FORK: u32 = 0x40;

    let alice_private_key = "7fdafb9db5bc501f2096e7d13d331dc7a75d9594af3d251313ba8b6200f4e384"
        .decode_hex()
        .unwrap();

    // Create transaction with P2PKH as input and output.
    let utxo_hash_1 = "050d00e2e18ef13969606f1ceee290d3f49bd940684ce39898159352952b8ce2";
    let utxo_script_1 = "76a914aff1e0789e5fe316b729577665aa0a04d5b0f8c788ac"
        .decode_hex()
        .unwrap();
    let utxo_1 = Proto::Input {
        out_point: input::out_point(utxo_hash_1, 2),
        value: 5151,
        sighash_type: SIGHASH_ALL | BCH_SIGHASH_FORK,
        claiming_script: input::custom_script(utxo_script_1),
        ..Default::default()
    };

    let out_1 = Proto::Output {
        value: 600,
        to_recipient: output::to_address("1Bp9U1ogV3A14FMvKbRJms7ctyso4Z4Tcx"),
    };
    let explicit_change_out = Proto::Output {
        value: 4325,
        to_recipient: output::to_address("1FQc5LdgGHMHEN9nwkjmz6tWkxhPpxBvBU"),
    };

    let signing = Proto::SigningInput {
        version: Proto::TransactionVersion::V1,
        private_keys: vec![alice_private_key.into()],
        inputs: vec![utxo_1],
        outputs: vec![out_1, explicit_change_out],
        change_output: None,
        // No matter which selector to use.
        input_selector: Proto::InputSelector::SelectInOrder,
        chain_info: btc_info(),
        dust_policy: dust_threshold(DUST),
        ..Default::default()
    };

    let expected_encoded = concat!(
        "01000000",
        "01",
            "e28c2b955293159898e34c6840d99bf4d390e2ee1c6f606939f18ee1e2000d05", "02000000", "6b483045022100b70d158b43cbcded60e6977e93f9a84966bc0cec6f2dfd1463d1223a90563f0d02207548d081069de570a494d0967ba388ff02641d91cadb060587ead95a98d4e3534121038eab72ec78e639d02758e7860cdec018b49498c307791f785aa3019622f4ea5b", "ffffffff",
        "02",
            "5802000000000000", "1976a914769bdff96a02f9135a1d19b749db6a78fe07dc9088ac",
            "e510000000000000", "1976a9149e089b6889e032d46e3b915a3392edfd616fb1c488ac",
        "00000000"
    );
    // Successfully broadcasted:
    // https://blockchair.com/bitcoin-cash/transaction/96ee20002b34e468f9d3c5ee54f6a8ddaa61c118889c4f35395c2cd93ba5bbb4
    sign::BitcoinSignHelper::new(&signing)
        .coin(CoinType::Bitcoin)
        .sign(sign::Expected {
            encoded: expected_encoded,
            txid: "96ee20002b34e468f9d3c5ee54f6a8ddaa61c118889c4f35395c2cd93ba5bbb4",
            inputs: vec![5151],
            outputs: vec![600, 4325],
            // `vsize` is different from the estimated value due to the signatures der serialization.
            vsize: 226,
            weight: 904,
            // vsize
            fee: 226,
        });
}
