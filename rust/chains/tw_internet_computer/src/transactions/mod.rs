// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

pub mod transfer;

pub mod proto {
    include!(concat!(env!("OUT_DIR"), "/proto/mod.rs"));
}

use tw_keypair::ecdsa::secp256k1::PrivateKey;
use tw_proto::InternetComputer::Proto::mod_Transaction::OneOftransaction_oneof as Tx;

use crate::protocol::{identity, principal::Principal, rosetta};

#[derive(Debug)]
pub enum SignTransactionError {
    InvalidAmount,
    InvalidArguments,
    Identity(identity::SigningError),
    EncodingArgsFailed,
    InvalidToAccountIdentifier,
    InvalidEnvelopePair,
}

pub fn sign_transaction(
    private_key: PrivateKey,
    canister_id: Principal,
    transaction: &Tx,
) -> Result<rosetta::SignedTransaction, SignTransactionError> {
    match transaction {
        Tx::transfer(transfer_args) => transfer::transfer(
            private_key,
            canister_id,
            transfer::TransferArgs {
                memo: transfer_args.memo,
                amount: transfer_args.amount,
                max_fee: None,
                to: transfer_args.to_account_identifier.to_string(),
                current_timestamp_nanos: transfer_args.current_timestamp_nanos,
                permitted_drift: if transfer_args.permitted_drift > 0 {
                    Some(transfer_args.permitted_drift)
                } else {
                    None
                },
            },
        ),
        Tx::None => Err(SignTransactionError::InvalidArguments),
    }
}
