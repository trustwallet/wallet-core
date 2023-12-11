// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::context::CosmosContext;
use crate::hasher::CosmosHasher;
use crate::modules::serializer::protobuf_serializer::{ProtobufSerializer, SignDirectArgs};
use crate::transaction::UnsignedTransaction;
use std::marker::PhantomData;
use tw_coin_entry::error::SigningResult;
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
    ) -> SigningResult<ProtobufTxPreimage> {
        let tx_to_sign = ProtobufSerializer::build_sign_doc(unsigned)?;
        let encoded_tx = serialize(&tx_to_sign)?;
        let tx_hash = Context::TxHasher::hash_sign_doc(&encoded_tx);

        Ok(ProtobufTxPreimage {
            encoded_tx,
            tx_hash,
        })
    }

    pub fn preimage_hash_direct(args: &SignDirectArgs) -> SigningResult<ProtobufTxPreimage> {
        let tx_to_sign = ProtobufSerializer::<Context>::build_direct_sign_doc(args);
        let encoded_tx = serialize(&tx_to_sign)?;
        let tx_hash = Context::TxHasher::hash_sign_doc(&encoded_tx);

        Ok(ProtobufTxPreimage {
            encoded_tx,
            tx_hash,
        })
    }
}
