// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use super::*;
use std::collections::HashMap;
use tw_any_coin::test_utils::sign_utils::AnySignerHelper;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::ToHex;
use tw_memory::Data;
use tw_misc::traits::ToBytesVec;
use tw_proto::Common::Proto::SigningError;

type UtxoMap = HashMap<OutPoint, i64>;

pub struct Expected {
    pub encoded: &'static str,
    pub txid: &'static str,
    pub inputs: Vec<i64>,
    pub outputs: Vec<i64>,
    pub vsize: u64,
    pub weight: u64,
    pub fee: i64,
}

pub struct BitcoinSignHelper<'a> {
    input: &'a Proto::SigningInput<'a>,
    coin_type: Option<CoinType>,
}

impl<'a> BitcoinSignHelper<'a> {
    pub fn new(input: &'a Proto::SigningInput<'a>) -> Self {
        BitcoinSignHelper {
            input,
            coin_type: None,
        }
    }

    pub fn coin(mut self, coin_type: CoinType) -> Self {
        self.coin_type = Some(coin_type);
        self
    }

    #[track_caller]
    pub fn sign(self, expected: Expected) {
        let coin_type = self
            .coin_type
            .expect("'BitcoinSignHelper::coin_type' is not set");
        let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
        let output = signer.sign(coin_type, self.input.clone());

        self.verify_output(output, expected);
    }

    pub fn verify_output(self, output: Proto::SigningOutput, expected: Expected) {
        assert_eq!(output.error, SigningError::OK, "{}", output.error_message);

        let tx = output.transaction.as_ref().unwrap();

        // Collect all the UTXO amounts by using `OutPoint`.
        let output_inputs = self.transaction_input_amounts(tx);
        // Collect all the Output amounts.
        let output_outputs: Vec<_> = tx.outputs.iter().map(|output| output.value).collect();

        assert_eq!(
            output.encoded.to_hex(),
            expected.encoded,
            "Wrong encoded signed transaction"
        );
        assert_eq!(output_inputs, expected.inputs, "Wrong UTXOs");
        assert_eq!(output_outputs, expected.outputs, "Wrong Outputs");
        assert_eq!(output.txid.to_hex(), expected.txid, "Wrong txid");
        assert_eq!(output.vsize, expected.vsize, "Wrong vsize");
        assert_eq!(output.weight, expected.weight, "Wrong weight");
        assert_eq!(output.fee, expected.fee, "Wrong fee");
    }

    /// Gets the map of `{ OutPoint -> Amount }`.
    fn utxo_map(&self) -> UtxoMap {
        let builder_input = self.transaction_builder();

        let mut utxo_map = HashMap::with_capacity(builder_input.inputs.len());
        for utxo in builder_input.inputs.iter() {
            utxo_map.insert(OutPoint::from_proto(&utxo.out_point), utxo.value);
        }
        utxo_map
    }

    /// Collect all the UTXO amounts by using `OutPoint`.
    fn transaction_input_amounts(&self, tx: &Proto::Transaction) -> Vec<i64> {
        let utxo_map = self.utxo_map();

        let mut output_inputs = Vec::with_capacity(tx.inputs.len());
        for utxo in tx.inputs.iter() {
            let searching_out_point = OutPoint::from_proto(&utxo.out_point);
            let utxo_amount = *utxo_map.get(&searching_out_point).unwrap_or_else(|| {
                panic!("Signed transaction does not contain {searching_out_point:?} UTXO")
            });
            output_inputs.push(utxo_amount);
        }

        output_inputs
    }

    fn transaction_builder(&self) -> &Proto::TransactionBuilder<'_> {
        match self.input.transaction {
            TransactionOneof::builder(ref builder) => builder,
            TransactionOneof::psbt(_) => panic!(
                "`BitcoinSignHelper` doesn't support PSBT. Consider using `BitcoinPsbtSignHelper`"
            ),
            TransactionOneof::None => unreachable!(),
        }
    }
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
