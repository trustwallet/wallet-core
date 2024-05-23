// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use super::*;
use std::collections::HashMap;
use tw_encoding::hex::ToHex;
use tw_memory::Data;
use tw_misc::traits::ToBytesVec;
use tw_proto::Common::Proto::SigningError;

pub struct Expected {
    pub encoded: &'static str,
    pub txid: &'static str,
    pub inputs: Vec<i64>,
    pub outputs: Vec<i64>,
    pub vsize: u64,
    pub fee: i64,
}

#[track_caller]
pub fn verify(input: &Proto::SigningInput, actual: &Proto::SigningOutput, expected: Expected) {
    assert_eq!(actual.error, SigningError::OK, "{}", actual.error_message);

    let tx = actual.transaction.as_ref().unwrap();

    // Get the map of `{ OutPoint -> Amount }`.
    let mut utxo_map = HashMap::with_capacity(input.inputs.len());
    for utxo in input.inputs.iter() {
        utxo_map.insert(OutPoint::from_proto(&utxo.out_point), utxo.value);
    }

    // Collect all the UTXO amounts by using `OutPoint`.
    let mut actual_inputs = Vec::with_capacity(tx.inputs.len());
    for utxo in tx.inputs.iter() {
        let searching_out_point = OutPoint::from_proto(&utxo.out_point);
        let utxo_amount = *utxo_map.get(&searching_out_point).unwrap_or_else(|| {
            panic!("Signed transaction does not contain {searching_out_point:?} UTXO")
        });
        actual_inputs.push(utxo_amount);
    }

    // Collect all the Output amounts.
    let actual_outputs: Vec<_> = tx.outputs.iter().map(|output| output.value).collect();

    assert_eq!(actual_inputs, expected.inputs, "Wrong UTXOs");
    assert_eq!(actual_outputs, expected.outputs, "Wrong Outputs");
    assert_eq!(
        actual.encoded.to_hex(),
        expected.encoded,
        "Wrong encoded signed transaction"
    );
    assert_eq!(actual.txid.to_hex(), expected.txid, "Wrong txid");
    assert_eq!(actual.vsize, expected.vsize, "Wrong vsize");
    assert_eq!(actual.fee, expected.fee, "Wrong fee");
}

#[derive(Debug, Eq, Hash, PartialEq)]
struct OutPoint {
    hash: Data,
    vout: u32,
}

impl OutPoint {
    fn from_proto(proto: &Option<Proto::OutPoint>) -> OutPoint {
        let out_point = proto.as_ref().expect("No OutPoint specified");
        OutPoint {
            hash: out_point.hash.to_vec(),
            vout: out_point.vout,
        }
    }
}
