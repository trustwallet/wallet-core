// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::Address;
use tw_number::U256;

pub struct Fee {
    amounts: Vec<Coin>,
    gas_limit: U256,
    payer: Address,
}

pub struct Coin {
    pub denom: String,
    pub amount: U256,
}
