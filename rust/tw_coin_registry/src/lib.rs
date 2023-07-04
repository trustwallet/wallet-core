// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_coin_entry::CoinEntry;

mod blockchain_type;
mod coin_context;
mod coin_type;
mod error;
mod registry;

pub use blockchain_type::BlockchainType;
pub use coin_context::CoinRegistryContext;
pub use coin_type::CoinType;
pub use error::{RegistryError, RegistryResult};
pub use registry::{get_coin_item, CoinItem};

pub fn blockchain_dispatcher(blockchain: BlockchainType) -> RegistryResult<impl CoinEntry> {
    match blockchain {
        BlockchainType::Ethereum => Ok(tw_ethereum::entry::EthereumEntry),
        BlockchainType::Unsupported => Err(RegistryError::Unsupported),
    }
}

pub fn coin_dispatcher(coin: CoinType) -> RegistryResult<impl CoinEntry> {
    let item = get_coin_item(coin)?;
    blockchain_dispatcher(item.blockchain)
}
