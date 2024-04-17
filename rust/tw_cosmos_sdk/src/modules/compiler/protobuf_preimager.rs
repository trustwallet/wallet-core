// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::context::CosmosContext;
use crate::modules::serializer::protobuf_serializer::{ProtobufSerializer, SignDirectArgs};
use crate::transaction::UnsignedTransaction;
use std::marker::PhantomData;
use tw_coin_entry::error::prelude::*;
use tw_hash::hasher::Hasher;
use tw_memory::Data;
use tw_proto::serialize;

pub struct ProtobufTxPreimage {
    pub encoded_tx: Data,
    pub tx_hash: Data,
}

pub struct ProtobufPreimager<Context: CosmosContext> {
    _phantom: PhantomData<Context>,
}

impl<Context: CosmosContext> ProtobufPreimager<Context> {
    pub fn preimage_hash(
        unsigned: &UnsignedTransaction<Context>,
        hasher: Hasher,
    ) -> SigningResult<ProtobufTxPreimage> {
        let tx_to_sign = ProtobufSerializer::build_sign_doc(unsigned)?;
        let encoded_tx = serialize(&tx_to_sign)?;
        let tx_hash = hasher.hash(&encoded_tx);

        Ok(ProtobufTxPreimage {
            encoded_tx,
            tx_hash,
        })
    }

    pub fn preimage_hash_direct(
        args: &SignDirectArgs,
        hasher: Hasher,
    ) -> SigningResult<ProtobufTxPreimage> {
        let tx_to_sign = ProtobufSerializer::<Context>::build_direct_sign_doc(args);
        let encoded_tx = serialize(&tx_to_sign)?;
        let tx_hash = hasher.hash(&encoded_tx);

        Ok(ProtobufTxPreimage {
            encoded_tx,
            tx_hash,
        })
    }
}
