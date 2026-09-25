// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::common::bitcoin::{
    dust_threshold, input, output, plan, sign, TransactionOneof, DUST, SIGHASH_ALL,
};
use crate::chains::komodo::komodo_info;
use crate::chains::zcash::{zcash_extra_data, SAPLING_BRANCH_ID};
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::DecodeHex;
use tw_proto::BitcoinV2::Proto;
use tw_proto::Zcash::Proto as ZcashProto;

/// Simulate ZEC transaction like it's broadcasted on Komodo chain.
/// Successfully broadcasted ZEC transaction:
/// https://explorer.zcha.in/transactions/ec9033381c1cc53ada837ef9981c03ead1c7c41700ff3a954389cfaddc949256
#[test]
fn test_komodo_sign_sapling_era() {
    const PRIVATE_KEY: &str = "a9684f5bebd0e1208aae2e02bc9e9163bd1965ad23d8538644e1df8b99b99559";
    const SENDER_ADDRESS: &str = "RXv5JDP8UjTzgKm4jKMiHhzoU1tobVLfyj";
    const TO_ADDRESS: &str = "RFzHSugnM6zCXbwrHL7UQ2QU86ixohq1U7";

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
        chain_info: komodo_info(),
        transaction: TransactionOneof::builder(builder),
        ..Default::default()
    };

    plan::BitcoinPlanHelper::new(&signing)
        .coin(CoinType::Komodo)
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
        .coin(CoinType::Komodo)
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
