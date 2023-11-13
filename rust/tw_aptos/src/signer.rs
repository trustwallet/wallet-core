// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::address::{Address, AptosAddress};
use crate::transaction_builder;
use std::marker::PhantomData;
use std::str::FromStr;
use tw_coin_entry::error::SigningResult;
use tw_coin_entry::signing_output_error;
use tw_keypair::ed25519;
use tw_proto::Aptos::Proto;

pub trait AptosContext {
    type Address: AptosAddress;
}

#[derive(Default)]
pub struct StandardAptosContext;

impl AptosContext for StandardAptosContext {
    type Address = Address;
}

pub struct Signer<Context: AptosContext> {
    _phantom: PhantomData<Context>,
}

impl<Context: AptosContext> Signer<Context> {
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
        let sender = Address::from_str(&input.sender)?;
        let signed_tx = builder
            .sender(sender.inner())
            .sequence_number(input.sequence_number as u64)
            .build()
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
