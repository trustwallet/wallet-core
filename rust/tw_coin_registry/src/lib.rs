// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

pub mod blockchain_type;
pub mod coin_context;
pub mod coin_type;
pub mod error;
pub mod registry;

use crate::blockchain_type::BlockchainType;
use crate::coin_context::CoinRegistryContext;
use crate::coin_type::CoinType;
use crate::error::{RegistryError, RegistryResult};
use crate::registry::get_coin_item;
use tw_coin_entry::coin_entry_ext::CoinEntryExt;

pub fn blockchain_dispatcher(blockchain: BlockchainType) -> RegistryResult<impl CoinEntryExt> {
    match blockchain {
        BlockchainType::Ethereum => Ok(tw_ethereum::entry::EthereumEntry),
        BlockchainType::Unsupported => Err(RegistryError::Unsupported),
    }
}

pub fn coin_dispatcher(coin: CoinType) -> RegistryResult<(CoinRegistryContext, impl CoinEntryExt)> {
    let item = get_coin_item(coin)?;
    let coin_entry = blockchain_dispatcher(item.blockchain)?;
    let coin_context = CoinRegistryContext::with_coin_item(item);
    Ok((coin_context, coin_entry))
}
