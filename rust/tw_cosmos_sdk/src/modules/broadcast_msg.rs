// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use quick_protobuf::MessageWrite;
use serde::Serialize;
use serde_json::Value as Json;
use tw_coin_entry::error::prelude::*;
use tw_encoding::base64::Base64Encoded;
use tw_proto::serialize;

pub enum BroadcastMode {
    Block,
    Async,
    Sync,
}

#[derive(Serialize)]
#[serde(untagged)]
pub enum BroadcastMsg {
    /// Binary representation of the transaction.
    Raw {
        mode: String,
        tx_bytes: Base64Encoded,
    },
    /// JSON encoded transaction.
    Json { mode: String, tx: Json },
}

impl BroadcastMsg {
    pub fn raw<Tx: MessageWrite>(mode: BroadcastMode, tx: &Tx) -> BroadcastMsg {
        let mode = match mode {
            BroadcastMode::Block => "BROADCAST_MODE_BLOCK",
            BroadcastMode::Async => "BROADCAST_MODE_ASYNC",
            BroadcastMode::Sync => "BROADCAST_MODE_SYNC",
        }
        .to_string();
        let tx_bytes = Base64Encoded(serialize(tx).expect("Error on serializing transaction"));
        BroadcastMsg::Raw { mode, tx_bytes }
    }

    pub fn json<Tx: Serialize>(mode: BroadcastMode, tx: Tx) -> SigningResult<BroadcastMsg> {
        let mode = match mode {
            BroadcastMode::Block => "block",
            BroadcastMode::Async => "async",
            BroadcastMode::Sync => "sync",
        }
        .to_string();
        let tx = serde_json::to_value(tx)
            .tw_err(|_| SigningErrorType::Error_internal)
            .context("Error serializing transaction to sign as JSON")?;
        Ok(BroadcastMsg::Json { mode, tx })
    }

    pub fn to_json_string(&self) -> String {
        // It's safe to unwrap here because `BroadcastMsg` consists of checked fields only.
        serde_json::to_string(self).expect("Unexpected error on serializing a BroadcastMsg")
    }
}
