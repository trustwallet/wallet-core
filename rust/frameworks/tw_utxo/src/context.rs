// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::fee::fee_estimator::FeeEstimator;
use crate::script::Script;
use crate::transaction::transaction_interface::TransactionInterface;
use crate::transaction::TransactionPreimage;
use std::str::FromStr;
use tw_coin_entry::error::prelude::*;

pub struct AddressPrefixes {
    pub p2pkh_prefix: u8,
    pub p2sh_prefix: u8,
}

pub trait UtxoContext {
    type Address: FromStr<Err = AddressError>;
    type Transaction: TransactionPreimage + TransactionInterface;
    type FeeEstimator: FeeEstimator<Self::Transaction>;

    fn addr_to_script_pubkey(
        addr: &Self::Address,
        prefixes: AddressPrefixes,
    ) -> SigningResult<Script>;
}
