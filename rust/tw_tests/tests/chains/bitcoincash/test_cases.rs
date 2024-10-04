// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::bitcoincash::BCH_SIGHASH_FORK;
use crate::chains::common::bitcoin::{dust_threshold, input, output, DUST, SIGHASH_ALL};
use tw_proto::BitcoinV2::Proto;

/// Successfully broadcasted:
/// https://blockchair.com/bitcoin-cash/transaction/96ee20002b34e468f9d3c5ee54f6a8ddaa61c118889c4f35395c2cd93ba5bbb4
pub(super) mod transfer_96ee20 {
    use super::*;

    pub const PRIVATE_KEY: &str =
        "7fdafb9db5bc501f2096e7d13d331dc7a75d9594af3d251313ba8b6200f4e384";
    pub const ENCODED_TX: &str = concat!(
        "01000000",
        "01",
            "e28c2b955293159898e34c6840d99bf4d390e2ee1c6f606939f18ee1e2000d05", "02000000", "6b483045022100b70d158b43cbcded60e6977e93f9a84966bc0cec6f2dfd1463d1223a90563f0d02207548d081069de570a494d0967ba388ff02641d91cadb060587ead95a98d4e3534121038eab72ec78e639d02758e7860cdec018b49498c307791f785aa3019622f4ea5b", "ffffffff",
        "02",
            "5802000000000000", "1976a914769bdff96a02f9135a1d19b749db6a78fe07dc9088ac",
            "e510000000000000", "1976a9149e089b6889e032d46e3b915a3392edfd616fb1c488ac",
        "00000000"
    );
    pub const SIGHASH: &str = "1136d4975aee4ff6ccf0b8a9c640532f563b48d9856fdc9682c37a071702937c";
    pub const SIGNATURE: &str = "b70d158b43cbcded60e6977e93f9a84966bc0cec6f2dfd1463d1223a90563f0d7548d081069de570a494d0967ba388ff02641d91cadb060587ead95a98d4e35301";
    pub const TX_ID: &str = "96ee20002b34e468f9d3c5ee54f6a8ddaa61c118889c4f35395c2cd93ba5bbb4";

    pub fn transaction_builder() -> Proto::TransactionBuilder<'static> {
        // Create transaction with P2PKH as input and output.
        let utxo_hash_1 = "050d00e2e18ef13969606f1ceee290d3f49bd940684ce39898159352952b8ce2";
        let utxo_1 = Proto::Input {
            out_point: input::out_point(utxo_hash_1, 2),
            value: 5151,
            sighash_type: SIGHASH_ALL | BCH_SIGHASH_FORK,
            // Cash address without prefix.
            claiming_script: input::receiver_address("qzhlrcrcne07x94h99thved2pgzdtv8ccujjy73xya"),
            ..Default::default()
        };

        let out_1 = Proto::Output {
            value: 600,
            // Legacy address.
            to_recipient: output::to_address("1Bp9U1ogV3A14FMvKbRJms7ctyso4Z4Tcx"),
        };
        let explicit_change_out = Proto::Output {
            value: 4325,
            // Cash address with an explicit prefix.
            to_recipient: output::to_address(
                "bitcoincash:qz0q3xmg38sr94rw8wg45vujah7kzma3cskxymnw06",
            ),
        };

        Proto::TransactionBuilder {
            version: Proto::TransactionVersion::V1,
            inputs: vec![utxo_1],
            outputs: vec![out_1, explicit_change_out],
            change_output: None,
            // No matter which selector to use.
            input_selector: Proto::InputSelector::SelectInOrder,
            dust_policy: dust_threshold(DUST),
            ..Default::default()
        }
    }
}
