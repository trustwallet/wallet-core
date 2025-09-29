// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_proto::Tezos::Proto::{FA12Parameters, FA2Parameters};

pub enum PrimType {
    Pair = 7,
}

#[derive(Debug)]
pub enum MichelsonValue {
    String(String),
    Int(String),
    Prim {
        prim: String,
        args: Vec<MichelsonValue>,
        anots: Vec<String>,
    },
    Array(Vec<MichelsonValue>),
}

pub fn fa12_parameter_to_michelson(data: FA12Parameters) -> MichelsonValue {
    let address = MichelsonValue::String(data.from.to_string());
    let to = MichelsonValue::String(data.to.to_string());
    let amount = MichelsonValue::Int(data.value.to_string());

    let prim_transfer_infos = MichelsonValue::Prim {
        prim: "Pair".to_string(),
        args: vec![to, amount],
        anots: vec![],
    };

    MichelsonValue::Prim {
        prim: "Pair".to_string(),
        args: vec![address, prim_transfer_infos],
        anots: vec![],
    }
}

pub fn fa2_parameter_to_michelson(data: FA2Parameters) -> MichelsonValue {
    let tx_obj = data
        .txs_object
        .first()
        .expect("FA2Parameters must have at least one txs_object");
    let from = MichelsonValue::String(tx_obj.from.to_string());
    let tx_transfer_infos = &tx_obj.txs[0];

    let token_id = MichelsonValue::Int(tx_transfer_infos.token_id.to_string());
    let amount = MichelsonValue::Int(tx_transfer_infos.amount.to_string());

    let prim_transfer_infos = MichelsonValue::Prim {
        prim: "Pair".to_string(),
        args: vec![token_id, amount],
        anots: vec![],
    };

    let to = MichelsonValue::String(tx_transfer_infos.to.to_string());
    let txs = MichelsonValue::Array(vec![MichelsonValue::Prim {
        prim: "Pair".to_string(),
        args: vec![to, prim_transfer_infos],
        anots: vec![],
    }]);

    let prim_txs = MichelsonValue::Prim {
        prim: "Pair".to_string(),
        args: vec![from, txs],
        anots: vec![],
    };

    MichelsonValue::Array(vec![prim_txs])
}
