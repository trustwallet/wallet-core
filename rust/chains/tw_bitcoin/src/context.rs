// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::error::prelude::SigningResult;
use tw_utxo::address::standard_bitcoin::StandardBitcoinAddress;
use tw_utxo::context::{AddressPrefixes, UtxoContext};
use tw_utxo::script::Script;

#[derive(Default)]
pub struct StandardBitcoinContext;

impl UtxoContext for StandardBitcoinContext {
    type Address = StandardBitcoinAddress;

    fn addr_to_script_pubkey(
        addr: &Self::Address,
        prefixes: AddressPrefixes,
    ) -> SigningResult<Script> {
        match addr {
            StandardBitcoinAddress::Legacy(legacy) => {
                legacy.to_script_pubkey(prefixes.p2pkh_prefix, prefixes.p2sh_prefix)
            },
            StandardBitcoinAddress::Segwit(segwit) => segwit.to_script_pubkey(),
            StandardBitcoinAddress::Taproot(taproot) => taproot.to_script_pubkey(),
        }
    }
}
