// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::common::bitcoin::babylon::BabylonProto;
use crate::chains::common::bitcoin::{
    babylon, btc_info, dust_threshold, input, output, sign, TransactionOneof, DUST, SIGHASH_ALL,
};
use std::borrow::Cow;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::DecodeHex;
use tw_proto::BitcoinV2::Proto;

/// tb1pt5dc8uhznywzmqpzdf2d3yj4w6980yzav0gdtagarprkzslep28qdrnfr5
const PRIVATE_KEY_1: &str = "43fe106cf05afdd8c39476651ad3729fc71bfe2ff4dfbc3e8654b851677a19b2";
const PUBLIC_KEY_1: &str = "039789cdd12bc90bbd73445718f8a709956eb3cce362716a3425610abb75ea1132";
/// Babylon Foundation 0.
const FINALITY_PROVIDER_BABYLON_FOUND_0: &str =
    "03d5a0bb72d71993e435d6c5a70e2aa4db500a62cfaae33c56050deefee64ec0";
// Forbole (https://forbole.com).
const FINALITY_PROVIDER_FORBOLE: &str =
    "860639b311132fe18972d92d2ee8885312706f1f4075a157be30180ac9c757e9";
const COVENANT_COMMITTEES: [&str; 3] = [
    "0249766ccd9e3cd94343e2040474a77fb37cdfd30530d05f9f1e96ae1e2102c86e",
    "0276d1ae01f8fb6bf30108731c884cddcf57ef6eef2d9d9559e130894e0e40c62c",
    "0217921cf156ccb4e73d428f996ed11b245313e37e27c978ac4d2cc21eca4672e4",
];
const MAGIC_TAG: &str = "62626434";

fn covenant_committees() -> Vec<Cow<'static, [u8]>> {
    COVENANT_COMMITTEES
        .iter()
        .map(|pk| pk.decode_hex().unwrap().into())
        .collect::<Vec<Cow<'static, [u8]>>>()
}

#[test]
fn test_bitcoin_babylon_stake() {
    const STAKING_TIME: u32 = 1000;
    const STAKING_AMOUNT: i64 = 30_000;
    const RBF_ENABLED: u32 = 0xFFFFFFFD;
    /// `global_parameters.activation_height`
    const STAKING_ACTIVATION_TIME: u32 = 215563;

    let txid0 = "5e70a0b02500c38f0b4078ee9e6da91dbc46afbf91c6e416822e97e23723bdbb";
    let utxo0 = Proto::Input {
        out_point: input::out_point(txid0, 0),
        value: 26_841,
        sighash_type: SIGHASH_ALL,
        sequence: input::sequence(RBF_ENABLED),
        claiming_script: input::p2tr_key_path(PUBLIC_KEY_1.decode_hex().unwrap()),
        ..Default::default()
    };

    let txid1 = "69e8ff4bbc1006f9e967a2d48785f50d7906c69c6d89b9c5a29c648639079ef2";
    let utxo1 = Proto::Input {
        out_point: input::out_point(txid1, 2),
        value: 39_466,
        sighash_type: SIGHASH_ALL,
        sequence: input::sequence(RBF_ENABLED),
        claiming_script: input::p2tr_key_path(PUBLIC_KEY_1.decode_hex().unwrap()),
        ..Default::default()
    };

    let out0 = Proto::Output {
        value: STAKING_AMOUNT,
        to_recipient: babylon::output::staking(BabylonProto::StakingInfo {
            staker_public_key: PUBLIC_KEY_1.decode_hex().unwrap().into(),
            finality_provider_public_key: FINALITY_PROVIDER_BABYLON_FOUND_0
                .decode_hex()
                .unwrap()
                .into(),
            staking_time: STAKING_TIME,
            covenant_committee_public_keys: covenant_committees(),
            covenant_quorum: 2,
        }),
    };

    let out1 = Proto::Output {
        value: 0,
        to_recipient: babylon::output::op_return(
            MAGIC_TAG,
            PUBLIC_KEY_1,
            FINALITY_PROVIDER_BABYLON_FOUND_0,
            STAKING_TIME,
        ),
    };

    let change_output = Proto::Output {
        value: 0,
        to_recipient: output::p2tr_key_path(PUBLIC_KEY_1.decode_hex().unwrap()),
    };

    let builder = Proto::TransactionBuilder {
        version: Proto::TransactionVersion::V2,
        // The transaction can be included starting from height LOCK_TIME+1.
        // https://learnmeabitcoin.com/technical/transaction/locktime/
        lock_time: STAKING_ACTIVATION_TIME - 1,
        inputs: vec![utxo0, utxo1],
        outputs: vec![out0, out1],
        change_output: Some(change_output),
        input_selector: Proto::InputSelector::SelectDescending,
        fee_per_vb: 3,
        dust_policy: dust_threshold(DUST),
        ..Default::default()
    };

    let signing = Proto::SigningInput {
        private_keys: vec![PRIVATE_KEY_1.decode_hex().unwrap().into()],
        chain_info: btc_info(),
        transaction: TransactionOneof::builder(builder),
        dangerous_use_fixed_schnorr_rng: true,
        ..Default::default()
    };

    // Successfully broadcasted: https://mempool.signet.babylonchain.io/signet/tx/0942545a1e324c713933350bd234e708b4f6092406dc45a97a61ce627a095e1b
    // Although, the signature is different because the real transaction was signed schnorr auxiliary random salt.
    let txid = "0942545a1e324c713933350bd234e708b4f6092406dc45a97a61ce627a095e1b";
    sign::BitcoinSignHelper::new(&signing)
        .coin(CoinType::Bitcoin)
        .sign(sign::Expected {
            encoded: "02000000000101f29e073986649ca2c5b9896d9cc606790df58587d4a267e9f90610bc4bffe8690200000000fdffffff0330750000000000002251206be6aea66b20c80a52d9ad36a7b2a9ebaf0300c973fce164dc2a5b411e894cce0000000000000000496a4762626434009789cdd12bc90bbd73445718f8a709956eb3cce362716a3425610abb75ea113203d5a0bb72d71993e435d6c5a70e2aa4db500a62cfaae33c56050deefee64ec003e833220000000000002251205d1b83f2e2991c2d80226a54d89255768a77905d63d0d5f51d18476143f90a8e014089aa8cd80f6edc7552ff431108873ba3d59c9ff359c4ecbded26748f8b616e9467ead4f3061310831a863b59f7a62741add206bce82a5fb14c779526bd5b91240a4a0300",
            txid,
            inputs: vec![39_466],
            outputs: vec![STAKING_AMOUNT, 0, 8755],
            vsize: 236,
            weight: 944,
            fee: 711,
        });
}

#[test]
fn test_bitcoin_babylon_unbond() {
    const STAKING_TIME: u32 = 1000;
    const UNBONDING_TIME: u32 = 5;
    const STAKING_AMOUNT: i64 = 30_000;
    /// `global_parameters.unbonding_fee`
    const UNBONDING_FEE: i64 = 3000;
    /// Covenant signature of the Unbonding transaction path.
    const COVENANT_1_SIGNATURE: &str = "f5c08530d9f48ab1b3bc2e3f5fed043a82cbcdf1f10af4b1774ac943392af108f6fd071157474505dea939ede9f7b07157e7274fb53e5b3d6e88f6fb1d23e790";
    const COVENANT_2_SIGNATURE: &str = "57f09c746aa784e4aebe8002d9068755f7d22262af71fdb75a840793fab6871e5ab387081dd6e31dd0e1ddfdfa11e298469321171eba722e79efaf2dbdb89905";

    let txid0 = "0942545a1e324c713933350bd234e708b4f6092406dc45a97a61ce627a095e1b";
    let covenant_signatures = babylon::input::PubkeySignatures::default()
        .add(&COVENANT_COMMITTEES[1], COVENANT_1_SIGNATURE)
        .add(&COVENANT_COMMITTEES[2], COVENANT_2_SIGNATURE);
    let utxo0 = Proto::Input {
        out_point: input::out_point(txid0, 0),
        value: STAKING_AMOUNT,
        sighash_type: SIGHASH_ALL,
        // RBF disabled.
        sequence: None,
        // Same `StakingInfo` as used in Staking Output.
        claiming_script: babylon::input::staking_unbonding_path(
            BabylonProto::StakingInfo {
                staker_public_key: PUBLIC_KEY_1.decode_hex().unwrap().into(),
                finality_provider_public_key: FINALITY_PROVIDER_BABYLON_FOUND_0
                    .decode_hex()
                    .unwrap()
                    .into(),
                staking_time: STAKING_TIME,
                covenant_committee_public_keys: covenant_committees(),
                covenant_quorum: 2,
            },
            covenant_signatures,
        ),
        ..Default::default()
    };

    let out0 = Proto::Output {
        value: STAKING_AMOUNT - UNBONDING_FEE,
        to_recipient: babylon::output::unbonding(BabylonProto::StakingInfo {
            staker_public_key: PUBLIC_KEY_1.decode_hex().unwrap().into(),
            finality_provider_public_key: FINALITY_PROVIDER_BABYLON_FOUND_0
                .decode_hex()
                .unwrap()
                .into(),
            staking_time: UNBONDING_TIME,
            covenant_committee_public_keys: covenant_committees(),
            covenant_quorum: 2,
        }),
    };

    let builder = Proto::TransactionBuilder {
        version: Proto::TransactionVersion::V2,
        // Can be included in any block.
        lock_time: 0,
        inputs: vec![utxo0],
        outputs: vec![out0],
        input_selector: Proto::InputSelector::UseAll,
        dust_policy: dust_threshold(DUST),
        ..Default::default()
    };

    let signing = Proto::SigningInput {
        private_keys: vec![PRIVATE_KEY_1.decode_hex().unwrap().into()],
        chain_info: btc_info(),
        transaction: TransactionOneof::builder(builder),
        dangerous_use_fixed_schnorr_rng: true,
        ..Default::default()
    };

    // Successfully broadcasted: https://mempool.signet.babylonchain.io/signet/tx/d7f7e34c4c3996f45f8a3ec291a27a9d5a3a636699d89c3ab22485295263fe77
    // Although, the signature is different because the real transaction was signed schnorr auxiliary random salt.
    // - Sighash is `bb3a0dcd4a3aa41980c7765f6180d2b228e761a3933c4f659d25cae782218197`
    // - Staker pubkey is `039789cdd12bc90bbd73445718f8a709956eb3cce362716a3425610abb75ea1132`
    // - Broadcasted signature is `e0ea3c8f58a13afa13822db74fdcc527f7ca6fa60dd2e817a27bb7fd5f36551283c0a76a7b0fe4690fb1b8884a59fecaac150519518f7d9654720302cae951ed`
    // - Signature in this test is `f3f6ff2a8b1aaf43cc7f6bb40b3ccf1aca14dd190a70f06528f594d26281e5c1afb792c454b5e9eda5e8f1a2723ebe463c5cf559710a9dbde6f4c329f1a64ec9`
    let txid = "d7f7e34c4c3996f45f8a3ec291a27a9d5a3a636699d89c3ab22485295263fe77";
    sign::BitcoinSignHelper::new(&signing)
        .coin(CoinType::Bitcoin)
        .sign(sign::Expected {
            encoded: "020000000001011b5e097a62ce617aa945dc062409f6b408e734d20b353339714c321e5a5442090000000000ffffffff0178690000000000002251206a19e05bf781849787564097f8986b84270b35f6dead370b0846b0c6ff4b0a5f0640f5c08530d9f48ab1b3bc2e3f5fed043a82cbcdf1f10af4b1774ac943392af108f6fd071157474505dea939ede9f7b07157e7274fb53e5b3d6e88f6fb1d23e790004057f09c746aa784e4aebe8002d9068755f7d22262af71fdb75a840793fab6871e5ab387081dd6e31dd0e1ddfdfa11e298469321171eba722e79efaf2dbdb8990540f3f6ff2a8b1aaf43cc7f6bb40b3ccf1aca14dd190a70f06528f594d26281e5c1afb792c454b5e9eda5e8f1a2723ebe463c5cf559710a9dbde6f4c329f1a64ec98a209789cdd12bc90bbd73445718f8a709956eb3cce362716a3425610abb75ea1132ad2017921cf156ccb4e73d428f996ed11b245313e37e27c978ac4d2cc21eca4672e4ac2049766ccd9e3cd94343e2040474a77fb37cdfd30530d05f9f1e96ae1e2102c86eba2076d1ae01f8fb6bf30108731c884cddcf57ef6eef2d9d9559e130894e0e40c62cba529c61c050929b74c1a04954b78b4b6035e97a5e078a5a0f28ec96d547bfee9ace803ac01a868c60f4ea18d9593a742735d726a3c63569a36fd50fe15d291fd1e3538429e494fdcafb7ee38a6636ab068f83d359e89af3b37ad3ed15b32354917f72254900000000",
            txid,
            inputs: vec![STAKING_AMOUNT],
            outputs: vec![STAKING_AMOUNT - UNBONDING_FEE],
            vsize: 203,
            weight: 812,
            fee: UNBONDING_FEE,
        });
}

#[test]
fn test_bitcoin_babylon_withdraw_unbonding_tx() {
    const UNBONDING_TIME: u32 = 5;
    const STAKING_AMOUNT: i64 = 30_000;
    /// `global_parameters.unbonding_fee`
    const UNBONDING_FEE: i64 = 3000;

    let txid0 = "d7f7e34c4c3996f45f8a3ec291a27a9d5a3a636699d89c3ab22485295263fe77";
    let utxo0 = Proto::Input {
        out_point: input::out_point(txid0, 0),
        value: STAKING_AMOUNT - UNBONDING_FEE,
        sighash_type: SIGHASH_ALL,
        sequence: input::sequence(UNBONDING_TIME),
        // Same `StakingInfo` as used in Staking Output.
        claiming_script: babylon::input::unbonding_timelock_path(BabylonProto::StakingInfo {
            staker_public_key: PUBLIC_KEY_1.decode_hex().unwrap().into(),
            finality_provider_public_key: FINALITY_PROVIDER_BABYLON_FOUND_0
                .decode_hex()
                .unwrap()
                .into(),
            staking_time: UNBONDING_TIME,
            covenant_committee_public_keys: covenant_committees(),
            covenant_quorum: 2,
        }),
        ..Default::default()
    };

    // Regular P2TR transfer.
    let explicit_max_out = Proto::Output {
        value: 26_841,
        to_recipient: output::p2tr_key_path(PUBLIC_KEY_1.decode_hex().unwrap()),
    };

    let builder = Proto::TransactionBuilder {
        version: Proto::TransactionVersion::V2,
        // Can be included in any block.
        lock_time: 0,
        inputs: vec![utxo0],
        outputs: vec![explicit_max_out],
        fee_per_vb: 1,
        input_selector: Proto::InputSelector::UseAll,
        dust_policy: dust_threshold(DUST),
        ..Default::default()
    };

    let signing = Proto::SigningInput {
        private_keys: vec![PRIVATE_KEY_1.decode_hex().unwrap().into()],
        chain_info: btc_info(),
        transaction: TransactionOneof::builder(builder),
        dangerous_use_fixed_schnorr_rng: true,
        ..Default::default()
    };

    // Successfully broadcasted: https://mempool.signet.babylonchain.io/signet/tx/48365f8c6afec38c2cd3a63f55685a10593a156869beda5165eb807886d089a5
    // Although, the signature is different because the real transaction was signed schnorr auxiliary random salt.
    // - Sighash is `478673e3ea9442f7f51072f0516ebeb1d4b9e5e812b0d052ee9edfa3dba431a2`
    // - Staker pubkey is `039789cdd12bc90bbd73445718f8a709956eb3cce362716a3425610abb75ea1132`
    // - Broadcasted signature is `ba5697f94a2d9f0583b70b41cf8cfe4d030bda0be6b18df7938eecd498c23363e5bb9100ebc72cb313594f9488e4db0c5c8ab22c8a671cf2b404b6b60e291d73`
    // - Signature in this test is `2856dd1927e5f95fdc45346261f17455130244f5d3db225ceaef292ed233b66104fea4eb343f7f9ee99a8ee6d67b4e5a95d660bf0299b67d37a96e08e6839be8`
    let txid = "48365f8c6afec38c2cd3a63f55685a10593a156869beda5165eb807886d089a5";
    sign::BitcoinSignHelper::new(&signing)
        .coin(CoinType::Bitcoin)
        .sign(sign::Expected {
            encoded: "0200000000010177fe6352298524b23a9cd89966633a5a9d7aa291c23e8a5ff496394c4ce3f7d700000000000500000001d9680000000000002251205d1b83f2e2991c2d80226a54d89255768a77905d63d0d5f51d18476143f90a8e03402856dd1927e5f95fdc45346261f17455130244f5d3db225ceaef292ed233b66104fea4eb343f7f9ee99a8ee6d67b4e5a95d660bf0299b67d37a96e08e6839be824209789cdd12bc90bbd73445718f8a709956eb3cce362716a3425610abb75ea1132ad55b241c050929b74c1a04954b78b4b6035e97a5e078a5a0f28ec96d547bfee9ace803ac0e494fdcafb7ee38a6636ab068f83d359e89af3b37ad3ed15b32354917f72254900000000",
            txid,
            inputs: vec![STAKING_AMOUNT - UNBONDING_FEE],
            outputs: vec![26_841],
            vsize: 137,
            weight: 547,
            fee: 159,
        });
}

#[test]
fn test_bitcoin_babylon_withdraw_staking_tx() {
    const STAKING_TIME: u32 = 150;
    const STAKING_AMOUNT: i64 = 30_000;

    let txid0 = "69e8ff4bbc1006f9e967a2d48785f50d7906c69c6d89b9c5a29c648639079ef2";
    let utxo0 = Proto::Input {
        out_point: input::out_point(txid0, 0),
        value: STAKING_AMOUNT,
        sighash_type: SIGHASH_ALL,
        sequence: input::sequence(STAKING_TIME),
        // Same `StakingInfo` as used in Staking Output.
        claiming_script: babylon::input::staking_timelock_path(BabylonProto::StakingInfo {
            staker_public_key: PUBLIC_KEY_1.decode_hex().unwrap().into(),
            finality_provider_public_key: FINALITY_PROVIDER_FORBOLE.decode_hex().unwrap().into(),
            staking_time: STAKING_TIME,
            covenant_committee_public_keys: covenant_committees(),
            covenant_quorum: 2,
        }),
    };

    // Regular P2TR transfer.
    let max_out = Proto::Output {
        value: 0,
        to_recipient: output::p2tr_key_path(PUBLIC_KEY_1.decode_hex().unwrap()),
    };

    let builder = Proto::TransactionBuilder {
        version: Proto::TransactionVersion::V2,
        // Can be included in any block.
        lock_time: 0,
        inputs: vec![utxo0],
        fee_per_vb: 1,
        max_amount_output: Some(max_out),
        input_selector: Proto::InputSelector::UseAll,
        dust_policy: dust_threshold(DUST),
        ..Default::default()
    };

    let signing = Proto::SigningInput {
        private_keys: vec![PRIVATE_KEY_1.decode_hex().unwrap().into()],
        chain_info: btc_info(),
        transaction: TransactionOneof::builder(builder),
        dangerous_use_fixed_schnorr_rng: true,
        ..Default::default()
    };

    // Successfully broadcasted: https://mempool.signet.babylonchain.io/signet/tx/b85b36bb375a03f8d5f5b5aa0bca804b103b575dac15a96fb49dd2f760fe54cc%
    let txid = "b85b36bb375a03f8d5f5b5aa0bca804b103b575dac15a96fb49dd2f760fe54cc";
    sign::BitcoinSignHelper::new(&signing)
        .coin(CoinType::Bitcoin)
        .sign(sign::Expected {
            encoded: "02000000000101f29e073986649ca2c5b9896d9cc606790df58587d4a267e9f90610bc4bffe869000000000096000000019e740000000000002251205d1b83f2e2991c2d80226a54d89255768a77905d63d0d5f51d18476143f90a8e0340641992c74bd77bac866fe4a3daa0ac85829f2defceba9acfae6e50a850bb1658518aabf9a12f57ddca77d272b6079248d17bfb0f91df374aa989c6483586ca8f26209789cdd12bc90bbd73445718f8a709956eb3cce362716a3425610abb75ea1132ad029600b261c050929b74c1a04954b78b4b6035e97a5e078a5a0f28ec96d547bfee9ace803ac08cec39f47bbd70eeda791f6b48a4bf906e878c87f6e5ca3f650008f39f1d4a51e77ab126d8d0a6c6ca6c2f1c0d31141352fb95112aa2bbd4390a7e4481e3d8dc00000000",
            txid,
            inputs: vec![STAKING_AMOUNT],
            outputs: vec![29_854],
            vsize: 146,
            weight: 581,
            fee: 146,
        });
}
