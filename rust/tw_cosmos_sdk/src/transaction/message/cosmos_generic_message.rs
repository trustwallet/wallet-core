// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::transaction::message::{CosmosMessage, JsonMessage};
use serde_json::Value as Json;
use tw_coin_entry::error::SigningResult;

/// Any raw JSON message.
/// Supports JSON serialization only.
pub struct JsonRawMessage {
    pub msg_type: String,
    pub value: Json,
}

impl CosmosMessage for JsonRawMessage {
    fn to_json(&self) -> SigningResult<JsonMessage> {
        Ok(JsonMessage {
            msg_type: self.msg_type.clone(),
            value: self.value.clone(),
        })
    }
}
