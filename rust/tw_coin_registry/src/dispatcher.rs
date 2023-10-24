// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::blockchain_type::BlockchainType;
use crate::coin_context::CoinRegistryContext;
use crate::coin_type::CoinType;
use crate::error::{RegistryError, RegistryResult};
use crate::registry::get_coin_item;
use tw_bitcoin::entry::BitcoinEntry;
use tw_coin_entry::coin_entry_ext::CoinEntryExt;
use tw_ethereum::entry::EthereumEntry;
use tw_evm::evm_entry::EvmEntryExt;
use tw_internet_computer::entry::InternetComputerEntry;
use tw_ronin::entry::RoninEntry;

pub type CoinEntryExtStaticRef = &'static dyn CoinEntryExt;
pub type EvmEntryExtStaticRef = &'static dyn EvmEntryExt;

const BITCOIN: BitcoinEntry = BitcoinEntry;
const ETHEREUM: EthereumEntry = EthereumEntry;
const INTERNET_COMPUTER: InternetComputerEntry = InternetComputerEntry;
const RONIN: RoninEntry = RoninEntry;

pub fn blockchain_dispatcher(blockchain: BlockchainType) -> RegistryResult<CoinEntryExtStaticRef> {
    match blockchain {
        BlockchainType::Bitcoin => Ok(&BITCOIN),
        BlockchainType::Ethereum => Ok(&ETHEREUM),
        BlockchainType::InternetComputer => Ok(&INTERNET_COMPUTER),
        BlockchainType::Ronin => Ok(&RONIN),
        BlockchainType::Unsupported => Err(RegistryError::Unsupported),
    }
}

pub fn coin_dispatcher(
    coin: CoinType,
) -> RegistryResult<(CoinRegistryContext, CoinEntryExtStaticRef)> {
    let item = get_coin_item(coin)?;
    let coin_entry = blockchain_dispatcher(item.blockchain)?;
    let coin_context = CoinRegistryContext::with_coin_item(item);
    Ok((coin_context, coin_entry))
}

pub fn evm_dispatcher(coin: CoinType) -> RegistryResult<EvmEntryExtStaticRef> {
    let item = get_coin_item(coin)?;
    match item.blockchain {
        BlockchainType::Bitcoin => Err(RegistryError::Unsupported),
        BlockchainType::Ethereum => Ok(&ETHEREUM),
        BlockchainType::InternetComputer => Err(RegistryError::Unsupported),
        BlockchainType::Ronin => Ok(&RONIN),
        BlockchainType::Unsupported => Err(RegistryError::Unsupported),
    }
}
