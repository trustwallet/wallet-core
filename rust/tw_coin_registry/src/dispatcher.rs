// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::blockchain_type::BlockchainType;
use crate::coin_context::CoinRegistryContext;
use crate::coin_type::CoinType;
use crate::error::{RegistryError, RegistryResult};
use crate::registry::get_coin_item;
use tw_aptos::entry::AptosEntry;
use tw_binance::entry::BinanceEntry;
use tw_bitcoin::entry::BitcoinEntry;
use tw_bitcoincash::entry::BitcoinCashEntry;
use tw_coin_entry::coin_entry_ext::CoinEntryExt;
use tw_cosmos::entry::CosmosEntry;
use tw_decred::entry::DecredEntry;
use tw_ethereum::entry::EthereumEntry;
use tw_evm::evm_entry::EvmEntryExt;
use tw_greenfield::entry::GreenfieldEntry;
use tw_groestlcoin::entry::GroestlcoinEntry;
use tw_internet_computer::entry::InternetComputerEntry;
use tw_komodo::entry::KomodoEntry;
use tw_native_evmos::entry::NativeEvmosEntry;
use tw_native_injective::entry::NativeInjectiveEntry;
use tw_pactus::entry::PactusEntry;
use tw_polkadot::entry::PolkadotEntry;
use tw_polymesh::entry::PolymeshEntry;
use tw_ripple::entry::RippleEntry;
use tw_ronin::entry::RoninEntry;
use tw_solana::entry::SolanaEntry;
use tw_substrate::entry::SubstrateEntry;
use tw_sui::entry::SuiEntry;
use tw_thorchain::entry::ThorchainEntry;
use tw_ton::entry::TheOpenNetworkEntry;
use tw_zcash::entry::ZcashEntry;

pub type CoinEntryExtStaticRef = &'static dyn CoinEntryExt;
pub type EvmEntryExtStaticRef = &'static dyn EvmEntryExt;

// start_of_blockchain_entries - USED TO GENERATE CODE
const APTOS: AptosEntry = AptosEntry;
const BINANCE: BinanceEntry = BinanceEntry;
const BITCOIN: BitcoinEntry = BitcoinEntry;
const BITCOIN_CASH: BitcoinCashEntry = BitcoinCashEntry;
const COSMOS: CosmosEntry = CosmosEntry;
const DECRED: DecredEntry = DecredEntry;
const ETHEREUM: EthereumEntry = EthereumEntry;
const GREENFIELD: GreenfieldEntry = GreenfieldEntry;
const GROESTLCOIN: GroestlcoinEntry = GroestlcoinEntry;
const INTERNET_COMPUTER: InternetComputerEntry = InternetComputerEntry;
const KOMODO: KomodoEntry = KomodoEntry;
const NATIVE_EVMOS: NativeEvmosEntry = NativeEvmosEntry;
const NATIVE_INJECTIVE: NativeInjectiveEntry = NativeInjectiveEntry;
const PACTUS: PactusEntry = PactusEntry;
const POLKADOT: SubstrateEntry<PolkadotEntry> = SubstrateEntry(PolkadotEntry);
const POLYMESH: SubstrateEntry<PolymeshEntry> = SubstrateEntry(PolymeshEntry);
const RIPPLE: RippleEntry = RippleEntry;
const RONIN: RoninEntry = RoninEntry;
const SOLANA: SolanaEntry = SolanaEntry;
const SUI: SuiEntry = SuiEntry;
const THE_OPEN_NETWORK: TheOpenNetworkEntry = TheOpenNetworkEntry;
const THORCHAIN: ThorchainEntry = ThorchainEntry;
const ZCASH: ZcashEntry = ZcashEntry;
// end_of_blockchain_entries - USED TO GENERATE CODE

pub fn blockchain_dispatcher(blockchain: BlockchainType) -> RegistryResult<CoinEntryExtStaticRef> {
    match blockchain {
        // start_of_blockchain_dispatcher - USED TO GENERATE CODE
        BlockchainType::Aptos => Ok(&APTOS),
        BlockchainType::Binance => Ok(&BINANCE),
        BlockchainType::Bitcoin => Ok(&BITCOIN),
        BlockchainType::BitcoinCash => Ok(&BITCOIN_CASH),
        BlockchainType::Cosmos => Ok(&COSMOS),
        BlockchainType::Decred => Ok(&DECRED),
        BlockchainType::Ethereum => Ok(&ETHEREUM),
        BlockchainType::Greenfield => Ok(&GREENFIELD),
        BlockchainType::Groestlcoin => Ok(&GROESTLCOIN),
        BlockchainType::InternetComputer => Ok(&INTERNET_COMPUTER),
        BlockchainType::Komodo => Ok(&KOMODO),
        BlockchainType::Kusama => Ok(&POLKADOT),
        BlockchainType::NativeEvmos => Ok(&NATIVE_EVMOS),
        BlockchainType::NativeInjective => Ok(&NATIVE_INJECTIVE),
        BlockchainType::Pactus => Ok(&PACTUS),
        BlockchainType::Polkadot => Ok(&POLKADOT),
        BlockchainType::Polymesh => Ok(&POLYMESH),
        BlockchainType::Ripple => Ok(&RIPPLE),
        BlockchainType::Ronin => Ok(&RONIN),
        BlockchainType::Solana => Ok(&SOLANA),
        BlockchainType::Sui => Ok(&SUI),
        BlockchainType::TheOpenNetwork => Ok(&THE_OPEN_NETWORK),
        BlockchainType::Thorchain => Ok(&THORCHAIN),
        BlockchainType::Zcash => Ok(&ZCASH),
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
