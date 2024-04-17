// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::context::CosmosContext;
use crate::modules::serializer::json_serializer::JsonSerializer;
use crate::public_key::JsonPublicKey;
use crate::transaction::UnsignedTransaction;
use std::marker::PhantomData;
use tw_coin_entry::error::prelude::*;
use tw_hash::hasher::Hasher;
use tw_memory::Data;

pub struct JsonTxPreimage {
    pub encoded_tx: String,
    pub tx_hash: Data,
}

pub struct JsonPreimager<Context: CosmosContext> {
    _phantom: PhantomData<Context>,
}

impl<Context: CosmosContext> JsonPreimager<Context>
where
    Context::PublicKey: JsonPublicKey,
{
    pub fn preimage_hash(
        unsigned: &UnsignedTransaction<Context>,
        hasher: Hasher,
    ) -> SigningResult<JsonTxPreimage> {
        let tx_to_sign = JsonSerializer::build_unsigned_tx(unsigned)?;
        let encoded_tx = serde_json::to_string(&tx_to_sign)
            .tw_err(|_| SigningErrorType::Error_internal)
            .context("Error serializing transaction to sign as JSON")?;
        let tx_hash = hasher.hash(encoded_tx.as_bytes());

        Ok(JsonTxPreimage {
            encoded_tx,
            tx_hash,
        })
    }
}
