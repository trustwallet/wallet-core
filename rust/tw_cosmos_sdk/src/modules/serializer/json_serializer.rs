// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::context::CosmosContext;
use crate::private_key::SignatureData;
use crate::public_key::{CosmosPublicKey, JsonPublicKey};
use crate::transaction::{Coin, Fee, SignedTransaction, UnsignedTransaction};
use serde::Serialize;
use serde_json::Value as Json;
use std::marker::PhantomData;
use tw_coin_entry::error::SigningResult;
use tw_encoding::base64::Base64Encoded;

#[derive(Serialize)]
pub struct SignedTxJson {
    pub fee: FeeJson,
    pub memo: String,
    pub msg: Vec<AnyMsg<Json>>,
    pub signatures: Vec<SignatureJson>,
    #[serde(skip_serializing_if = "Option::is_none")]
    #[serde(default)]
    pub timeout_height: Option<String>,
}

#[derive(Serialize)]
pub struct UnsignedTxJson {
    pub account_number: String,
    pub chain_id: String,
    pub fee: FeeJson,
    pub memo: String,
    pub msgs: Vec<AnyMsg<Json>>,
    pub sequence: String,
    #[serde(skip_serializing_if = "Option::is_none")]
    #[serde(default)]
    pub timeout_height: Option<String>,
}

#[derive(Serialize)]
pub struct FeeJson {
    pub amount: Vec<Coin>,
    pub gas: String,
}

#[derive(Clone, Serialize)]
pub struct AnyMsg<Value> {
    #[serde(rename = "type")]
    pub msg_type: String,
    pub value: Value,
}

#[derive(Clone, Serialize)]
pub struct SignatureJson {
    pub pub_key: AnyMsg<Base64Encoded>,
    pub signature: Base64Encoded,
}

/// `JsonSerializer` serializes transaction to JSON in Cosmos specific way.
pub struct JsonSerializer<Context: CosmosContext> {
    _phantom: PhantomData<Context>,
}

impl<Context> JsonSerializer<Context>
where
    Context: CosmosContext,
    Context::PublicKey: JsonPublicKey,
{
    pub fn build_signed_tx(signed: &SignedTransaction<Context>) -> SigningResult<SignedTxJson> {
        let msg = signed
            .tx_body
            .messages
            .iter()
            .map(|msg| msg.to_json())
            .collect::<SigningResult<_>>()?;
        let signature =
            Self::serialize_signature(&signed.signer.public_key, signed.signature.clone());

        let convert = |value: u64| {
            if value == 0 {
                None
            } else {
                Some(value.to_string())
            }
        };
        Ok(SignedTxJson {
            fee: Self::build_fee(&signed.fee),
            memo: signed.tx_body.memo.clone(),
            msg,
            signatures: vec![signature],
            timeout_height: convert(signed.tx_body.timeout_height),
        })
    }

    pub fn build_unsigned_tx(
        unsigned: &UnsignedTransaction<Context>,
    ) -> SigningResult<UnsignedTxJson> {
        let msgs = unsigned
            .tx_body
            .messages
            .iter()
            .map(|msg| msg.to_json())
            .collect::<SigningResult<_>>()?;

        let convert = |value: u64| {
            if value == 0 {
                None
            } else {
                Some(value.to_string())
            }
        };
        Ok(UnsignedTxJson {
            account_number: unsigned.account_number.to_string(),
            chain_id: unsigned.chain_id.clone(),
            fee: Self::build_fee(&unsigned.fee),
            memo: unsigned.tx_body.memo.clone(),
            msgs,
            sequence: unsigned.signer.sequence.to_string(),
            timeout_height: convert(unsigned.tx_body.timeout_height),
        })
    }

    pub fn serialize_signature(
        public_key: &Context::PublicKey,
        signature: SignatureData,
    ) -> SignatureJson {
        SignatureJson {
            pub_key: Self::serialize_public_key(public_key),
            signature: Base64Encoded(signature),
        }
    }

    pub fn serialize_public_key(public_key: &Context::PublicKey) -> AnyMsg<Base64Encoded> {
        AnyMsg {
            msg_type: public_key.public_key_type(),
            value: Base64Encoded(public_key.to_bytes()),
        }
    }

    pub fn build_fee(fee: &Fee<Context::Address>) -> FeeJson {
        FeeJson {
            gas: fee.gas_limit.to_string(),
            amount: fee.amounts.clone(),
        }
    }
}
