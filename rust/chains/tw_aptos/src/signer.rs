// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::Address;
use crate::transaction_builder;
use std::str::FromStr;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::signing_output_error;
use tw_keypair::ed25519;
use tw_proto::Aptos::Proto;

pub struct Signer;

impl Signer {
    #[inline]
    pub fn sign_proto(input: Proto::SigningInput<'_>) -> Proto::SigningOutput<'static> {
        Self::sign_proto_impl(input)
            .unwrap_or_else(|e| signing_output_error!(Proto::SigningOutput, e))
    }

    fn sign_proto_impl(
        input: Proto::SigningInput<'_>,
    ) -> SigningResult<Proto::SigningOutput<'static>> {
        let key_pair = ed25519::sha512::KeyPair::try_from(input.private_key.as_ref())?;
        let builder = transaction_builder::TransactionFactory::new_from_protobuf(input.clone())?;
        let sender = Address::from_str(&input.sender)
            .into_tw()
            .context("Invalid sender address")?;
        let signed_tx = builder
            .sender(sender.inner())
            .sequence_number(input.sequence_number as u64)
            .build()?
            .sign(key_pair)?;
        Ok(Proto::SigningOutput {
            raw_txn: signed_tx.raw_txn_bytes().clone().into(),
            encoded: signed_tx.encoded().clone().into(),
            authenticator: Some((*signed_tx.authenticator()).clone().into()),
            json: signed_tx.to_json().to_string().into(),
            ..Proto::SigningOutput::default()
        })
    }
}
