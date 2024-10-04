// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::script::Script;
use std::str::FromStr;
use tw_coin_entry::error::prelude::{AddressError, SigningResult};

pub struct AddressPrefixes {
    pub p2pkh_prefix: u8,
    pub p2sh_prefix: u8,
}

pub trait UtxoContext {
    type Address: FromStr<Err = AddressError>;

    fn addr_to_script_pubkey(
        addr: &Self::Address,
        prefixes: AddressPrefixes,
    ) -> SigningResult<Script>;
}
