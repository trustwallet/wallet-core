// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_any_coin::ffi::tw_any_address::{
    tw_any_address_create_with_string, tw_any_address_description,
};
use tw_any_coin::test_utils::address_utils::TWAnyAddressHelper;
use tw_coin_registry::coin_type::CoinType;
use tw_coin_registry::registry::get_coin_item;
use tw_memory::test_utils::tw_string_helper::TWStringHelper;

#[test]
fn test_any_address_normalize_eth() {
    for coin in CoinType::iter() {
        let coin_item = get_coin_item(coin).unwrap();

        // Skip unsupported blockchains.
        if !coin_item.blockchain.is_supported() {
            continue;
        }

        let (denormalized, expected_normalized) = match coin {
            CoinType::Ethereum
            | CoinType::AcalaEVM
            | CoinType::Arbitrum
            | CoinType::ArbitrumNova
            | CoinType::Aurora
            | CoinType::AvalancheCChain
            | CoinType::Boba
            | CoinType::Callisto
            | CoinType::Celo
            | CoinType::ConfluxeSpace
            | CoinType::CronosChain
            | CoinType::ECOChain
            | CoinType::EthereumClassic
            | CoinType::Evmos
            | CoinType::Fantom
            | CoinType::GoChain
            | CoinType::KavaEvm
            | CoinType::Klaytn
            | CoinType::KuCoinCommunityChain
            | CoinType::Meter
            | CoinType::Metis
            | CoinType::Moonbeam
            | CoinType::Moonriver
            | CoinType::Optimism
            | CoinType::Zksync
            | CoinType::PolygonzkEVM
            | CoinType::OKXChain
            | CoinType::POANetwork
            | CoinType::Polygon
            | CoinType::SmartBitcoinCash
            | CoinType::SmartChain
            | CoinType::SmartChainLegacy
            | CoinType::Theta
            | CoinType::ThetaFuel
            | CoinType::ThunderCore
            | CoinType::TomoChain
            | CoinType::VeChain
            | CoinType::Wanchain
            | CoinType::xDai
            | CoinType::IoTeXEVM
            | CoinType::Scroll
            | CoinType::OpBNB
            | CoinType::Neon
            | CoinType::Base
            | CoinType::Linea
            | CoinType::Greenfield
            | CoinType::Mantle
            | CoinType::ZenEON => (
                "0xb16db98b365b1f89191996942612b14f1da4bd5f",
                "0xb16Db98B365B1f89191996942612B14F1Da4Bd5f",
            ),
            // end_of_ethereum_coin_address_normalization
            CoinType::Bitcoin
            // TODO all Bitcoin-based blockchains should have different addresses.
            // It should be fixed when Bitcoin is finalized.
            | CoinType::Litecoin
            | CoinType::Dogecoin
            | CoinType::Dash
            | CoinType::Viacoin
            | CoinType::DigiByte
            | CoinType::Monacoin
            | CoinType::Syscoin
            | CoinType::Pivx
            | CoinType::Firo
            | CoinType::BitcoinCash
            | CoinType::BitcoinGold
            | CoinType::Ravencoin
            | CoinType::Qtum
            | CoinType::eCash
            | CoinType::Stratis
            => (
                "19cAJn4Ms8jodBBGtroBNNpCZiHAWGAq7X",
                "19cAJn4Ms8jodBBGtroBNNpCZiHAWGAq7X",
            ),
            CoinType::Aptos => (
                "0xf3d7f364dd7705824a5ebda9c7aab6cb3fc7bb5b58718249f12defec240b36cc",
                "0xf3d7f364dd7705824a5ebda9c7aab6cb3fc7bb5b58718249f12defec240b36cc",
            ),
            CoinType::Cosmos => (
                "cosmosvaloper1sxx9mszve0gaedz5ld7qdkjkfv8z992ax69k08",
                "cosmosvaloper1sxx9mszve0gaedz5ld7qdkjkfv8z992ax69k08",
            ),
            CoinType::Stargaze => (
                "stars1ten42eesehw0ktddcp0fws7d3ycsqez3tcyzth",
                "stars1ten42eesehw0ktddcp0fws7d3ycsqez3tcyzth"
            ),
            CoinType::Juno => (
                "juno1ten42eesehw0ktddcp0fws7d3ycsqez3fksy86",
                "juno1ten42eesehw0ktddcp0fws7d3ycsqez3fksy86"
            ),
            CoinType::Stride => (
                "stride1ten42eesehw0ktddcp0fws7d3ycsqez3u0nr52",
                "stride1ten42eesehw0ktddcp0fws7d3ycsqez3u0nr52"
            ),
            CoinType::Axelar => (
                "axelar1ten42eesehw0ktddcp0fws7d3ycsqez3m29ht8",
                "axelar1ten42eesehw0ktddcp0fws7d3ycsqez3m29ht8"
            ),
            CoinType::Crescent => (
                "cre1ten42eesehw0ktddcp0fws7d3ycsqez3mvq64t",
                "cre1ten42eesehw0ktddcp0fws7d3ycsqez3mvq64t"
            ),
            CoinType::Kujira => (
                "kujira1ten42eesehw0ktddcp0fws7d3ycsqez3wv38dv",
                "kujira1ten42eesehw0ktddcp0fws7d3ycsqez3wv38dv"
            ),
            CoinType::Comdex => (
                "comdex1ten42eesehw0ktddcp0fws7d3ycsqez3ct3ae3",
                "comdex1ten42eesehw0ktddcp0fws7d3ycsqez3ct3ae3"
            ),
            CoinType::Neutron => (
                "neutron1ten42eesehw0ktddcp0fws7d3ycsqez3mm6a6p",
                "neutron1ten42eesehw0ktddcp0fws7d3ycsqez3mm6a6p"
            ),
            CoinType::Sommelier => (
                "somm1ten42eesehw0ktddcp0fws7d3ycsqez3ncun3v",
                "somm1ten42eesehw0ktddcp0fws7d3ycsqez3ncun3v"
            ),
            CoinType::FetchAI => (
                "fetch1ten42eesehw0ktddcp0fws7d3ycsqez3ve6mz3",
                "fetch1ten42eesehw0ktddcp0fws7d3ycsqez3ve6mz3"
            ),
            CoinType::Mars => (
                "mars1ten42eesehw0ktddcp0fws7d3ycsqez3ze2x4a",
                "mars1ten42eesehw0ktddcp0fws7d3ycsqez3ze2x4a"
            ),
            CoinType::Umee => (
                "umee1ten42eesehw0ktddcp0fws7d3ycsqez3djwqy5",
                "umee1ten42eesehw0ktddcp0fws7d3ycsqez3djwqy5"
            ),
            CoinType::Noble => (
                "noble1ten42eesehw0ktddcp0fws7d3ycsqez3h8xhcg",
                "noble1ten42eesehw0ktddcp0fws7d3ycsqez3h8xhcg"
            ),
            CoinType::Sei => (
                "sei1ten42eesehw0ktddcp0fws7d3ycsqez3jgzfx8",
                "sei1ten42eesehw0ktddcp0fws7d3ycsqez3jgzfx8"
            ),
            CoinType::Tia => (
                "celestia1ten42eesehw0ktddcp0fws7d3ycsqez3wwz06t",
                "celestia1ten42eesehw0ktddcp0fws7d3ycsqez3wwz06t"
            ),
            CoinType::Coreum => (
                "core1ten42eesehw0ktddcp0fws7d3ycsqez3v2ty8a",
                "core1ten42eesehw0ktddcp0fws7d3ycsqez3v2ty8a"
            ),
            CoinType::Quasar => (
                "quasar1ten42eesehw0ktddcp0fws7d3ycsqez338fzdr",
                "quasar1ten42eesehw0ktddcp0fws7d3ycsqez338fzdr"
            ),
            CoinType::Persistence => (
                "persistence1ten42eesehw0ktddcp0fws7d3ycsqez33g4vwz",
                "persistence1ten42eesehw0ktddcp0fws7d3ycsqez33g4vwz"
            ),
            CoinType::Akash => (
                "akash1ten42eesehw0ktddcp0fws7d3ycsqez3jl7ceu",
                "akash1ten42eesehw0ktddcp0fws7d3ycsqez3jl7ceu"
            ),
            CoinType::Terra => (
                "terra1ten42eesehw0ktddcp0fws7d3ycsqez3eqflzx",
                "terra1ten42eesehw0ktddcp0fws7d3ycsqez3eqflzx"
            ),
            CoinType::TerraV2 => (
                "terra1ten42eesehw0ktddcp0fws7d3ycsqez3eqflzx",
                "terra1ten42eesehw0ktddcp0fws7d3ycsqez3eqflzx"
            ),
            CoinType::Kava => (
                "kava1ten42eesehw0ktddcp0fws7d3ycsqez3r38zkp",
                "kava1ten42eesehw0ktddcp0fws7d3ycsqez3r38zkp"
            ),
            CoinType::Bluzelle => (
                "bluzelle1ten42eesehw0ktddcp0fws7d3ycsqez32usaxh",
                "bluzelle1ten42eesehw0ktddcp0fws7d3ycsqez32usaxh"
            ),
            CoinType::BandChain => (
                "band1ten42eesehw0ktddcp0fws7d3ycsqez3xtnacw",
                "band1ten42eesehw0ktddcp0fws7d3ycsqez3xtnacw"
            ),
            CoinType::Rootstock => (
                "0xAc1ec44E4f0ca7D172B7803f6836De87Fb72b309",
                "0xAc1ec44E4f0ca7D172B7803f6836De87Fb72b309"
            ),
            CoinType::THORChain => (
                "thor1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r",
                "thor1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r",
            ),
            CoinType::CryptoOrg => (
                "cro1ten42eesehw0ktddcp0fws7d3ycsqez38lmxuh",
                "cro1ten42eesehw0ktddcp0fws7d3ycsqez38lmxuh"
            ),
            CoinType::Ronin => (
                "ronin:Ac1ec44E4f0ca7D172B7803f6836De87Fb72b309",
                "ronin:Ac1ec44E4f0ca7D172B7803f6836De87Fb72b309"
            ),
            CoinType::Secret => (
                "secret1ten42eesehw0ktddcp0fws7d3ycsqez3ap8ka6",
                "secret1ten42eesehw0ktddcp0fws7d3ycsqez3ap8ka6"
            ),
            CoinType::Osmosis => (
                "osmo1ten42eesehw0ktddcp0fws7d3ycsqez3hlq0k5",
                "osmo1ten42eesehw0ktddcp0fws7d3ycsqez3hlq0k5"
            ),
            CoinType::NativeEvmos => (
                "evmos17xpfvakm2amg962yls6f84z3kell8c5ljcjw34",
                "evmos17xpfvakm2amg962yls6f84z3kell8c5ljcjw34",
            ),
            CoinType::Agoric => (
                "agoric1ten42eesehw0ktddcp0fws7d3ycsqez3de3qss",
                "agoric1ten42eesehw0ktddcp0fws7d3ycsqez3de3qss"
            ),
            CoinType::NativeInjective => (
                "inj14py36sx57ud82t9yrks9z6hdsrpn5x6k8tf7m3",
                "inj14py36sx57ud82t9yrks9z6hdsrpn5x6k8tf7m3",
            ),
            CoinType::NativeCanto => (
                "canto14s0vgnj0pjnazu4hsqlksdk7slah9vcfuuhw7m",
                "canto14s0vgnj0pjnazu4hsqlksdk7slah9vcfuuhw7m"
            ),
            CoinType::InternetComputer => (
                "290CC7C359F44C8516FC169C5ED4F0F3AE2E24BF5DE0D4C51F5E7545B5474FAA",
                "290cc7c359f44c8516fc169c5ed4f0f3ae2e24bf5de0d4c51f5e7545b5474faa",
            ),
            // end_of_coin_address_normalization
            _ => panic!("{:?} must be covered", coin),
        };

        let denormalized = TWStringHelper::create(denormalized);

        let any_address = TWAnyAddressHelper::wrap(unsafe {
            tw_any_address_create_with_string(denormalized.ptr(), coin as u32)
        });

        let normalized =
            TWStringHelper::wrap(unsafe { tw_any_address_description(any_address.ptr()) });

        assert_eq!(
            normalized.to_string(),
            Some(expected_normalized.to_string())
        );
    }
}
