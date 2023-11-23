// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::codegen::cpp::coin_entry::BlockchainImpl;
use crate::registry::CoinItem;
use crate::Result;

pub fn new_blockchain(coin: CoinItem) -> Result<()> {
    // Generate C++ files.
    BlockchainImpl::new(coin).create()?;

    Ok(())
}
