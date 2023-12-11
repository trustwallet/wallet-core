// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::context::CosmosContext;
use crate::hasher::CosmosHasher;
use crate::modules::serializer::json_serializer::JsonSerializer;
use crate::public_key::JsonPublicKey;
use crate::transaction::UnsignedTransaction;
use std::marker::PhantomData;
use tw_coin_entry::error::{SigningError, SigningErrorType, SigningResult};
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
    pub fn preimage_hash(unsigned: &UnsignedTransaction<Context>) -> SigningResult<JsonTxPreimage> {
        let tx_to_sign = JsonSerializer::build_unsigned_tx(unsigned)?;
        let encoded_tx = serde_json::to_string(&tx_to_sign)
            .map_err(|_| SigningError(SigningErrorType::Error_internal))?;
        let tx_hash = Context::TxHasher::hash_json_tx(&encoded_tx);

        Ok(JsonTxPreimage {
            encoded_tx,
            tx_hash,
        })
    }
}
