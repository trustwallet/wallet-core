// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::transaction::payload::{BondPayload, Payload, TransferPayload};
use crate::transaction::Transaction;
use crate::types::{Address, Amount, ValidatorPublicKey};
use std::str::FromStr;
use tw_coin_entry::error::prelude::*;
use tw_proto::Pactus;

pub struct TxBuilder;

impl TxBuilder {
    pub fn from_proto(input: &Pactus::Proto::SigningInput) -> SigningResult<Transaction> {
        match &input.transaction {
            None => SigningError::err(SigningErrorType::Error_invalid_params),
            Some(trx) => {
                let payload: Box<dyn Payload> = match &trx.payload {
                    Pactus::Proto::mod_TransactionMessage::OneOfpayload::transfer(pld) => {
                        let sender = Address::from_str(&pld.sender)?;
                        let receiver = Address::from_str(&pld.receiver)?;
                        Box::new(TransferPayload::new(sender, receiver, Amount(pld.amount)))
                    },
                    Pactus::Proto::mod_TransactionMessage::OneOfpayload::bond(pld) => {
                        let sender = Address::from_str(&pld.sender)?;
                        let receiver = Address::from_str(&pld.receiver)?;
                        let public_key = if !pld.public_key.is_empty() {
                            Some(ValidatorPublicKey::from_str(&pld.public_key)?)
                        } else {
                            None
                        };

                        Box::new(BondPayload::new(
                            sender,
                            receiver,
                            Amount(pld.stake),
                            public_key,
                        ))
                    },
                    Pactus::Proto::mod_TransactionMessage::OneOfpayload::None => {
                        return SigningError::err(SigningErrorType::Error_invalid_params)
                    },
                };

                Ok(Transaction::new(
                    trx.lock_time,
                    Amount(trx.fee),
                    trx.memo.to_string(),
                    payload,
                ))
            },
        }
    }
}
