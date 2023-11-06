// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use quick_protobuf::MessageWrite;
use serde::Serialize;
use serde_json::Value as Json;
use std::fmt;
use std::fmt::Formatter;
use tw_encoding::base64::Base64Encoded;
use tw_proto::serialize;

pub enum BroadcastMode {
    Block,
    Async,
    Sync,
}

impl fmt::Display for BroadcastMode {
    fn fmt(&self, f: &mut Formatter<'_>) -> fmt::Result {
        match self {
            BroadcastMode::Block => write!(f, "BROADCAST_MODE_BLOCK"),
            BroadcastMode::Async => write!(f, "BROADCAST_MODE_ASYNC"),
            BroadcastMode::Sync => write!(f, "BROADCAST_MODE_SYNC"),
        }
    }
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
        BroadcastMsg::Raw {
            mode: mode.to_string(),
            tx_bytes: Base64Encoded(serialize(tx).expect("Error on serializing transaction")),
        }
    }

    pub fn json(mode: BroadcastMode, tx: Json) -> BroadcastMsg {
        BroadcastMsg::Json {
            mode: mode.to_string(),
            tx,
        }
    }

    pub fn to_json_string(&self) -> String {
        // It's safe to unwrap here because `BroadcastMsg` consists of checked fields only.
        serde_json::to_string(self).expect("Unexpected error on serializing a BroadcastMsg")
    }
}
