// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::address::CosmosAddress;
use crate::hasher::CosmosHasher;
use crate::modules::broadcast_msg::{BroadcastMode, BroadcastMsg};
use crate::modules::serializer::json_serializer::JsonSerializer;
use crate::modules::serializer::protobuf_serializer::ProtobufSerializer;
use crate::modules::tx_builder::TxBuilder;
use crate::private_key::CosmosPrivateKey;
use crate::public_key::CosmosPublicKey;
use crate::transaction::{SignedTransaction, UnsignedTransaction};
use std::borrow::Cow;
use std::marker::PhantomData;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::{SigningError, SigningErrorType, SigningResult};
use tw_coin_entry::signing_output_error;
use tw_proto::serialize;
use tw_proto::Cosmos::Proto;

pub mod protobuf_signer;

pub struct TransactionSigner<Address, PrivateKey, PublicKey, Hasher> {
    _phantom: PhantomData<(Address, PrivateKey, PublicKey, Hasher)>,
}

impl<Address, PrivateKey, PublicKey, Hasher>
    TransactionSigner<Address, PrivateKey, PublicKey, Hasher>
where
    Address: CosmosAddress,
    PrivateKey: CosmosPrivateKey,
    PublicKey: CosmosPublicKey,
    Hasher: CosmosHasher,
{
    #[inline]
    pub fn sign(
        coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
    ) -> Proto::SigningOutput<'static> {
        Self::sign_proto_impl(coin, input)
            .unwrap_or_else(|e| signing_output_error!(Proto::SigningOutput, e))
    }

    fn sign_impl(
        coin: &dyn CoinContext,
        mut input: Proto::SigningInput<'_>,
    ) -> SigningResult<Proto::SigningOutput<'static>> {
        let private_key = PrivateKey::try_from(&input.private_key)?;
        let public_key = PublicKey::from_private_key(coin, private_key.as_ref())?;

        let broadcast_mode = match input.mode {
            Proto::BroadcastMode::BLOCK => BroadcastMode::Block,
            Proto::BroadcastMode::SYNC => BroadcastMode::Sync,
            Proto::BroadcastMode::ASYNC => BroadcastMode::Async,
        };

        // Set the public key. It will be used to construct a signer info.
        input.public_key = Cow::from(public_key.to_bytes());
        let unsigned_tx = TxBuilder::<PublicKey>::unsigned_tx_from_proto(coin, &input)?;

        match input.signing_mode {
            Proto::SigningMode::JSON => todo!(),
            Proto::SigningMode::Protobuf => {
                let signed_tx = Self::sign_protobuf_tx(&private_key, unsigned_tx)?;
                let signed_tx_raw = ProtobufSerializer::build_signed_tx(&signed_tx);
                let broadcast_tx =
                    BroadcastMsg::raw(broadcast_mode, &signed_tx_raw).serialize_json();

                let signature_json =
                    JsonSerializer::serialize_signature(&public_key, signed_tx.signature);
                Ok(Proto::SigningOutput {
                    signature: Cow::from(signed_tx.signature.clone()),
                    signature_json: Cow::from(signature_json),
                    serialized: Cow::from(broadcast_tx),
                    ..Proto::SigningOutput::default()
                })
            },
        }
    }

    pub fn sign_protobuf_tx(
        private_key: &PrivateKey,
        unsigned: UnsignedTransaction<Address, PublicKey>,
    ) -> SigningResult<SignedTransaction<Address, PublicKey>> {
        let tx_to_sign = ProtobufSerializer::build_sign_doc(&unsigned);
        let encoded_tx = serialize(&tx_to_sign)?;

        let hash_to_sign = Hasher::hash_sign_doc(&encoded_tx);
        let signature_data = private_key.sign_tx_hash(&hash_to_sign)?;

        Ok(unsigned.into_signed(signature_data))
    }
}
