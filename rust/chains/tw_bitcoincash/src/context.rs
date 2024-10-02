// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::Address;
use tw_coin_entry::error::prelude::*;
use tw_utxo::context::{AddressPrefixes, UtxoContext};
use tw_utxo::script::Script;

pub struct BitcoinCashContext;

impl UtxoContext for BitcoinCashContext {
    type Address = Address;

    fn addr_to_script_pubkey(
        addr: &Self::Address,
        prefixes: AddressPrefixes,
    ) -> SigningResult<Script> {
        match addr {
            Address::Cash(cash) => cash
                .to_legacy(prefixes.p2pkh_prefix, prefixes.p2sh_prefix)
                .into_tw()
                .context("Error converting 'CashAddress' to 'LegacyAddress'")?
                .to_script_pubkey(prefixes.p2pkh_prefix, prefixes.p2sh_prefix),
            Address::Legacy(legacy) => {
                legacy.to_script_pubkey(prefixes.p2pkh_prefix, prefixes.p2sh_prefix)
            },
        }
    }
}
