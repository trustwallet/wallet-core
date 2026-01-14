// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::common::bitcoin::{
    dust_threshold, input, output, plan, psbt_sign, sign, transaction_psbt_b64, TransactionOneof,
    DUST, SIGHASH_ALL,
};
use crate::chains::zcash::{zcash_extra_data, zec_info, NU6_BRANCH_ID, SAPLING_BRANCH_ID};
use tw_any_coin::test_utils::sign_utils::AnySignerHelper;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::DecodeHex;
use tw_proto::BitcoinV2::Proto;
use tw_proto::Common::Proto::SigningError;
use tw_proto::Zcash::Proto as ZcashProto;

/// Successfully broadcasted:
/// https://explorer.zcha.in/transactions/ec9033381c1cc53ada837ef9981c03ead1c7c41700ff3a954389cfaddc949256
#[test]
fn test_zcash_sign_sapling_era() {
    const PRIVATE_KEY: &str = "a9684f5bebd0e1208aae2e02bc9e9163bd1965ad23d8538644e1df8b99b99559";
    const SENDER_ADDRESS: &str = "t1gWVE2uyrET2CxSmCaBiKzmWxQdHhnvMSz";
    const TO_ADDRESS: &str = "t1QahNjDdibyE4EdYkawUSKBBcVTSqv64CS";

    // Create transaction with P2PKH as input and output.
    let txid = "3a19dd44032dfed61bfca5ba5751aab8a107b30609cbd5d70dc5ef09885b6853";
    let tx1 = Proto::Input {
        out_point: input::out_point(txid, 0),
        value: 494_000,
        sighash_type: SIGHASH_ALL,
        claiming_script: input::receiver_address(SENDER_ADDRESS),
        ..Default::default()
    };

    let out1 = Proto::Output {
        value: 488_000,
        to_recipient: output::to_address(TO_ADDRESS),
    };

    let extra_data = ZcashProto::TransactionBuilderExtraData {
        branch_id: SAPLING_BRANCH_ID.into(),
        zip_0317: false,
        expiry_height: 0,
    };

    let builder = Proto::TransactionBuilder {
        version: Proto::TransactionVersion::UseDefault,
        inputs: vec![tx1],
        outputs: vec![out1],
        input_selector: Proto::InputSelector::UseAll,
        dust_policy: dust_threshold(DUST),
        chain_specific: zcash_extra_data(extra_data),
        ..Default::default()
    };

    let signing = Proto::SigningInput {
        private_keys: vec![PRIVATE_KEY.decode_hex().unwrap().into()],
        chain_info: zec_info(),
        transaction: TransactionOneof::builder(builder),
        ..Default::default()
    };

    plan::BitcoinPlanHelper::new(&signing)
        .coin(CoinType::Zcash)
        .plan(plan::Expected {
            inputs: vec![494_000],
            outputs: vec![488_000],
            // Estimated size is 1 byte greater than the final transaction.
            // That's because the final DER.1 serialized signature becomes more compact.
            vsize_estimate: 212,
            fee_estimate: 6_000,
            // Change output has been omitted.
            change: 0,
        });

    sign::BitcoinSignHelper::new(&signing)
        .coin(CoinType::Zcash)
        .sign(sign::Expected {
            encoded: "0400008085202f890153685b8809efc50dd7d5cb0906b307a1b8aa5157baa5fc1bd6fe2d0344dd193a000000006b483045022100ca0be9f37a4975432a52bb65b25e483f6f93d577955290bb7fb0060a93bfc92002203e0627dff004d3c72a957dc9f8e4e0e696e69d125e4d8e275d119001924d3b48012103b243171fae5516d1dc15f9178cfcc5fdc67b0a883055c117b01ba8af29b953f6ffffffff0140720700000000001976a91449964a736f3713d64283fd0018626ba50091c7e988ac00000000000000000000000000000000000000",
            txid: "ec9033381c1cc53ada837ef9981c03ead1c7c41700ff3a954389cfaddc949256",
            inputs: vec![494_000],
            outputs: vec![488_000],
            // `vsize` is different from the estimated value due to the signatures der serialization.
            vsize: 211,
            weight: 211 * 4,
            fee: 6_000,
        });
}

/// Successfully broadcasted:
/// https://blockchair.com/zcash/transaction/e5f4d0c28f96c80caace4052aed4645a78badedc00cd3b071322d63c264b2885
#[test]
fn test_zcash_send_to_tex_address() {
    const PRIVATE_KEY: &str = "cfa0c168cce0d041119b216f60aed343b93fde0a8736ea0bcd6254cfef0a3c79";
    const SENDER_ADDRESS: &str = "t1a6Do9CH4umduHKtoufDDMQMypd7VYLqhH";
    const TO_ADDRESS: &str = "tex1auz6gx89x2wcku6gswdvaz2nf9x3seex6px6v0";

    // Create transaction with P2PKH as input and output.
    let txid = "d0da6ed2f89de9936ca3429110bc60a02f6e797665b8714d646fc25b45210ef2";
    let tx1 = Proto::Input {
        out_point: input::out_point(txid, 0),
        // 0.02
        value: 2_000_000,
        sighash_type: SIGHASH_ALL,
        claiming_script: input::receiver_address(SENDER_ADDRESS),
        ..Default::default()
    };

    let out1 = Proto::Output {
        // 0.002
        value: 200_000,
        to_recipient: output::to_address(TO_ADDRESS),
    };
    let change_out = Proto::Output {
        to_recipient: output::to_address(SENDER_ADDRESS),
        ..Proto::Output::default()
    };

    let extra_data = ZcashProto::TransactionBuilderExtraData {
        branch_id: NU6_BRANCH_ID.into(),
        zip_0317: true,
        expiry_height: 0,
    };

    let builder = Proto::TransactionBuilder {
        version: Proto::TransactionVersion::UseDefault,
        inputs: vec![tx1],
        outputs: vec![out1],
        change_output: Some(change_out),
        input_selector: Proto::InputSelector::UseAll,
        dust_policy: dust_threshold(DUST),
        chain_specific: zcash_extra_data(extra_data),
        ..Default::default()
    };

    let signing = Proto::SigningInput {
        private_keys: vec![PRIVATE_KEY.decode_hex().unwrap().into()],
        chain_info: zec_info(),
        transaction: TransactionOneof::builder(builder),
        ..Default::default()
    };

    plan::BitcoinPlanHelper::new(&signing)
        .coin(CoinType::Zcash)
        .plan(plan::Expected {
            inputs: vec![2_000_000],
            outputs: vec![200_000, 1_790_000],
            // Estimated size is 1 byte greater than the final transaction.
            // That's because the final DER.1 serialized signature becomes more compact.
            vsize_estimate: 246,
            fee_estimate: 10_000,
            // Change output has been omitted.
            change: 1_790_000,
        });

    sign::BitcoinSignHelper::new(&signing)
        .coin(CoinType::Zcash)
        .sign(sign::Expected {
            encoded: "0400008085202f8901f20e21455bc26f644d71b86576796e2fa060bc109142a36c93e99df8d26edad0000000006b483045022100850a98be0d1a432f900bb3c34347d16ea839d4a59de288c17838f1d2ee6ec390022007e5fd53c8c31d75ca6d79b3d2874e6dd8e685e60758874e4884ace9d26eea4501210340643a2a4ea0777ce0b2529be566a3caea5598fef56c44579dadf96b586bed50ffffffff02400d0300000000001976a914ef05a418e5329d8b7348839ace8953494d18672688ac30501b00000000001976a914b1e4e13f836a6e7a33cbb20817a62da829d543e988ac00000000000000000000000000000000000000",
            txid: "e5f4d0c28f96c80caace4052aed4645a78badedc00cd3b071322d63c264b2885",
            inputs: vec![2_000_000],
            outputs: vec![200_000, 1_790_000],
            // `vsize` is different from the estimated value due to the signatures der serialization.
            vsize: 245,
            weight: 245 * 4,
            fee: 10_000,
        });
}

#[test]
fn test_zcash_sign_pczt() {
    const PRIVATE_KEY: &str = "c9d84f11d992c1a527293b468ba67f739f8098c333748493da45b9cf53844ec4";
    const PSBT: &str = "UENaVAEAAAAEhcG8yQjVoJzHDAEAz+i/AYUBgwACD/xF9t6uGgnrn6HPk4TTxoTB42mxjfZ2E8dQ7UzfodcAAf////8PAAAAgIl6GXapFFWjZeeDsUiGjDI8hm1k+WCt0MtqiKwAAAEAAAAAAACg+XQ96jWHJsJWFA3/kk+6ZaHhUI5wi57Hf3ZM6xQzRQEB/////w8AAAC1lhgZdqkUVaNl54OxSIaMMjyGbWT5YK3Qy2qIrAAAAQAAAAAAAALD8X4ZdqkUkFFaBL/xtThv5Hnc0F1hHGFf2sSIrAAAASN0MVgyZ21OUVJxd1dpR2luY0syVXRZd1piV2c3aXhZRkVLcgDCwxEZdqkUVaNl54OxSIaMMjyGbWT5YK3Qy2qIrAAAASN0MVJnUkJqam54WFNnMXB0TERya2FNTmlWNHRKVlh1N2RXVgAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAwABAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA==";

    let signing = Proto::SigningInput {
        private_keys: vec![PRIVATE_KEY.decode_hex().unwrap().into()],
        chain_info: zec_info(),
        transaction: transaction_psbt_b64(PSBT),
        ..Default::default()
    };

    // Successfully broadcasted tx:
    // https://blockchair.com/zcash/transaction/5055198cfba7109a23de0406885298939e3ea8ce5c160737b4cdf02360a8d10b
    psbt_sign::BitcoinPsbtSignHelper::new(&signing)
        .coin(CoinType::Zcash)
        .sign_psbt(psbt_sign::Expected {
            psbt: "50435a54010000000485c1bcc908d5a09cc70c0100cfe8bf0185018300020ffc45f6deae1a09eb9fa1cf9384d3c684c1e369b18df67613c750ed4cdfa1d70001ffffffff0f0000016b4830450221008ec23495bc2a7e7ccab7b7c6f7b1f9ee2f77a048bc4d2f4b5299fc1ab08b607e0220175453b2e2f25462abc1b5bff30d0c9e01fba60beab1d5a8460260612507e0d0012103f395d296aca81ab1d54a0eab0349333478c83591e6de81b9979cb63aef97c9ad80897a1976a91455a365e783b148868c323c866d64f960add0cb6a88ac000001000000000000a0f9743dea358726c256140dff924fba65a1e1508e708b9ec77f764ceb1433450101ffffffff0f0000016a473044022022d1eb63d66f868228439e6e18d21df262d8e05fce380ab3b21aa84f064ca614022063e87e73110b1829d4c8eb747d7ca8cc43e28e0a4f7d287aa42ed84048ba3cd4012103f395d296aca81ab1d54a0eab0349333478c83591e6de81b9979cb63aef97c9adb596181976a91455a365e783b148868c323c866d64f960add0cb6a88ac00000100000000000002c3f17e1976a91490515a04bff1b5386fe479dcd05d611c615fdac488ac0000012374315832676d4e51527177576947696e634b32557459775a6257673769785946454b7200c2c3111976a91455a365e783b148868c323c866d64f960add0cb6a88ac000001237431526752426a6a6e785853673170744c44726b614d4e695634744a56587537645756000000000000000000000000000000000000000000000000000000000000000000000000000003000100000000000000000000000000000000000000000000000000000000000000000000",
            encoded: "0400008085202f89020ffc45f6deae1a09eb9fa1cf9384d3c684c1e369b18df67613c750ed4cdfa1d7000000006b4830450221008ec23495bc2a7e7ccab7b7c6f7b1f9ee2f77a048bc4d2f4b5299fc1ab08b607e0220175453b2e2f25462abc1b5bff30d0c9e01fba60beab1d5a8460260612507e0d0012103f395d296aca81ab1d54a0eab0349333478c83591e6de81b9979cb63aef97c9adffffffffa0f9743dea358726c256140dff924fba65a1e1508e708b9ec77f764ceb143345010000006a473044022022d1eb63d66f868228439e6e18d21df262d8e05fce380ab3b21aa84f064ca614022063e87e73110b1829d4c8eb747d7ca8cc43e28e0a4f7d287aa42ed84048ba3cd4012103f395d296aca81ab1d54a0eab0349333478c83591e6de81b9979cb63aef97c9adffffffff02c3b81f00000000001976a91490515a04bff1b5386fe479dcd05d611c615fdac488acc2610400000000001976a91455a365e783b148868c323c866d64f960add0cb6a88ac000000004ff42f000000000000000000000000",
            txid: "5055198cfba7109a23de0406885298939e3ea8ce5c160737b4cdf02360a8d10b",
            vsize: 392,
            weight: 1568,
            fee: 30000,
        });
}

#[test]
fn test_zcash_sign_pczt_unsupported_tx_version() {
    const PRIVATE_KEY: &str = "c9d84f11d992c1a527293b468ba67f739f8098c333748493da45b9cf53844ec4";
    const PSBT: &str = "UENaVAEAAAAFis6ctQLVoJzHDAEA4ua/AYUBgwABFRZQRxlNqLSGFmGot6F/FeWgAGC2IUW5epyMH5ttEEQAAf////8PAAAAoMuYARl2qRRVo2Xng7FIhowyPIZtZPlgrdDLaoisAAABAAAAAAAAAsPxfhl2qRQthdKs2GnpfCJRan162hFZgsQNXoisAAABI3QxTjJKajlkRG9ZUUVtRVdBQlZ4TmF6YndzR3VVcko0UDREALWWGBl2qRRVo2Xng7FIhowyPIZtZPlgrdDLaoisAAABI3QxUmdSQmpqbnhYU2cxcHRMRHJrYU1OaVY0dEpWWHU3ZFdWAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAADAAEAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";

    let signing = Proto::SigningInput {
        private_keys: vec![PRIVATE_KEY.decode_hex().unwrap().into()],
        chain_info: zec_info(),
        transaction: transaction_psbt_b64(PSBT),
        ..Default::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Zcash, signing);
    assert_eq!(output.error, SigningError::Error_not_supported);
}
