// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::context::CosmosContext;
use crate::hasher::CosmosHasher;
use crate::modules::serializer::protobuf_serializer::ProtobufSerializer;
use crate::private_key::CosmosPrivateKey;
use crate::public_key::ProtobufPublicKey;
use crate::transaction::{SignedTransaction, UnsignedTransaction};
use std::marker::PhantomData;
use tw_coin_entry::error::SigningResult;
use tw_proto::serialize;

pub struct ProtobufSigner<Context: CosmosContext> {
    _phantom: PhantomData<Context>,
}

impl<Context: CosmosContext> ProtobufSigner<Context>
where
    Context::PublicKey: ProtobufPublicKey,
{
    pub fn sign_tx(
        private_key: &Context::PrivateKey,
        unsigned: UnsignedTransaction<Context>,
    ) -> SigningResult<SignedTransaction<Context>> {
        let tx_to_sign = ProtobufSerializer::build_sign_doc(&unsigned)?;
        let encoded_tx = serialize(&tx_to_sign)?;

        let hash_to_sign = Context::TxHasher::hash_sign_doc(&encoded_tx);
        let signature_data = private_key.sign_tx_hash(&hash_to_sign)?;

        Ok(unsigned.into_signed(signature_data))
    }
}
