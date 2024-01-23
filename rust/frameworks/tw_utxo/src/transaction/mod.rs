// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

// pub trait SigningParams {
//     fn hash_type(&self) ->
// }

use crate::sighash::Sighash;

pub mod standard_transaction;

// TODO remove or move.
pub struct HashingArgs {
    pub sighash: Sighash,
}

pub struct SignedTxIn {}
