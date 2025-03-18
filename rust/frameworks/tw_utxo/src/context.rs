// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::DEFAULT_PUBLIC_KEY_HASHER;
use crate::fee::fee_estimator::FeeEstimator;
use crate::script::Script;
use crate::transaction::standard_transaction::DEFAULT_TX_HASHER;
use crate::transaction::transaction_interface::TransactionInterface;
use crate::transaction::TransactionPreimage;
use std::str::FromStr;
use tw_coin_entry::error::prelude::*;
use tw_hash::hasher::Hasher;

pub type ContextTransactionInput<Context> =
    <<Context as UtxoContext>::Transaction as TransactionInterface>::Input;
pub type ContextTransactionOutput<Context> =
    <<Context as UtxoContext>::Transaction as TransactionInterface>::Output;

pub struct AddressPrefixes {
    pub p2pkh_prefix: u8,
    pub p2sh_prefix: u8,
}

pub trait UtxoContext {
    type Address: FromStr<Err = AddressError>;
    type Transaction: TransactionPreimage + TransactionInterface;
    type FeeEstimator: FeeEstimator<Self::Transaction>;

    const TX_HASHER: Hasher = DEFAULT_TX_HASHER;
    const PUBLIC_KEY_HASHER: Hasher = DEFAULT_PUBLIC_KEY_HASHER;

    fn addr_to_script_pubkey(
        addr: &Self::Address,
        prefixes: AddressPrefixes,
    ) -> SigningResult<Script>;
}
