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
use tw_aptos::entry::AptosEntry;
use tw_bitcoin::entry::BitcoinEntry;
use tw_coin_entry::coin_entry_ext::CoinEntryExt;
use tw_cosmos::entry::CosmosEntry;
use tw_ethereum::entry::EthereumEntry;
use tw_evm::evm_entry::EvmEntryExt;
use tw_internet_computer::entry::InternetComputerEntry;
use tw_native_evmos::entry::NativeEvmosEntry;
use tw_native_injective::entry::NativeInjectiveEntry;
use tw_ronin::entry::RoninEntry;
use tw_thorchain::entry::ThorchainEntry;

pub type CoinEntryExtStaticRef = &'static dyn CoinEntryExt;
pub type EvmEntryExtStaticRef = &'static dyn EvmEntryExt;

// start_of_blockchain_entries - USED TO GENERATE CODE
const APTOS: AptosEntry = AptosEntry;
const BITCOIN: BitcoinEntry = BitcoinEntry;
const COSMOS: CosmosEntry = CosmosEntry;
const ETHEREUM: EthereumEntry = EthereumEntry;
const INTERNET_COMPUTER: InternetComputerEntry = InternetComputerEntry;
const NATIVE_EVMOS: NativeEvmosEntry = NativeEvmosEntry;
const NATIVE_INJECTIVE: NativeInjectiveEntry = NativeInjectiveEntry;
const RONIN: RoninEntry = RoninEntry;
const THORCHAIN: ThorchainEntry = ThorchainEntry;
// end_of_blockchain_entries - USED TO GENERATE CODE

pub fn blockchain_dispatcher(blockchain: BlockchainType) -> RegistryResult<CoinEntryExtStaticRef> {
    match blockchain {
        // start_of_blockchain_dispatcher - USED TO GENERATE CODE
        BlockchainType::Aptos => Ok(&APTOS),
        BlockchainType::Bitcoin => Ok(&BITCOIN),
        BlockchainType::Cosmos => Ok(&COSMOS),
        BlockchainType::Ethereum => Ok(&ETHEREUM),
        BlockchainType::InternetComputer => Ok(&INTERNET_COMPUTER),
        BlockchainType::NativeEvmos => Ok(&NATIVE_EVMOS),
        BlockchainType::NativeInjective => Ok(&NATIVE_INJECTIVE),
        BlockchainType::Ronin => Ok(&RONIN),
        BlockchainType::Thorchain => Ok(&THORCHAIN),
        // end_of_blockchain_dispatcher - USED TO GENERATE CODE
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
        BlockchainType::Ethereum => Ok(&ETHEREUM),
        BlockchainType::Ronin => Ok(&RONIN),
        _ => Err(RegistryError::Unsupported),
    }
}
