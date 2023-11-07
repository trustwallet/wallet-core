// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::context::CosmosContext;
use crate::hasher::CosmosHasher;
use crate::modules::serializer::json_serializer::JsonSerializer;
use crate::private_key::CosmosPrivateKey;
use crate::public_key::JsonPublicKey;
use crate::transaction::{SignedTransaction, UnsignedTransaction};
use std::marker::PhantomData;
use tw_coin_entry::error::{SigningError, SigningErrorType, SigningResult};

pub struct JsonSigner<Context: CosmosContext> {
    _phantom: PhantomData<Context>,
}

impl<Context: CosmosContext> JsonSigner<Context>
where
    Context::PublicKey: JsonPublicKey,
{
    pub fn sign_tx(
        private_key: &Context::PrivateKey,
        unsigned: UnsignedTransaction<Context>,
    ) -> SigningResult<SignedTransaction<Context>> {
        let tx_to_sign = JsonSerializer::build_unsigned_tx(&unsigned)?;
        let encoded_tx = serde_json::to_string(&tx_to_sign)
            .map_err(|_| SigningError(SigningErrorType::Error_internal))?;

        let hash_to_sign = Context::TxHasher::hash_json_tx(&encoded_tx);
        let signature_data = private_key.sign_tx_hash(&hash_to_sign)?;

        Ok(unsigned.into_signed(signature_data))
    }
}
