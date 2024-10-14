// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::ffi::tw_any_address::{
    tw_any_address_create_with_public_key_derivation, tw_any_address_description,
};
use tw_any_coin::test_utils::address_utils::TWAnyAddressHelper;
use tw_coin_registry::coin_type::CoinType;
use tw_coin_registry::registry::get_coin_item;
use tw_coin_registry::tw_derivation::TWDerivation;
use tw_keypair::ffi::privkey::tw_private_key_get_public_key_by_type;
use tw_keypair::test_utils::tw_private_key_helper::TWPrivateKeyHelper;
use tw_keypair::test_utils::tw_public_key_helper::TWPublicKeyHelper;
use tw_memory::test_utils::tw_string_helper::TWStringHelper;

#[test]
fn test_coin_address_derivation() {
    let private_key = TWPrivateKeyHelper::with_hex(
        "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5",
    );

    for coin in CoinType::iter() {
        let coin_item = get_coin_item(coin).unwrap();

        // Skip unsupported blockchains.
        if !coin_item.blockchain.is_supported() {
            continue;
        }

        let public_key = TWPublicKeyHelper::wrap(unsafe {
            tw_private_key_get_public_key_by_type(
                private_key.ptr(),
                coin_item.public_key_type as u32,
            )
        });

        let expected_address = match coin {
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
            | CoinType::Kaia
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
            | CoinType::Viction
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
            | CoinType::ZenEON
            | CoinType::MantaPacific
            | CoinType::ZetaEVM
            | CoinType::Merlin
            | CoinType::Lightlink
            | CoinType::Blast
            | CoinType::BounceBit
            | CoinType::ZkLinkNova
            // end_of_evm_address_derivation_tests_marker_do_not_modify
                => "0xAc1ec44E4f0ca7D172B7803f6836De87Fb72b309",
            CoinType::Bitcoin => "bc1qten42eesehw0ktddcp0fws7d3ycsqez3f7d5yt",
            CoinType::Litecoin => "ltc1qten42eesehw0ktddcp0fws7d3ycsqez3dzhsum",
            CoinType::Dogecoin => "DDkFr311AYe6ABMsdSnjv8yoSr1Tppokp8",
            CoinType::Dash => "XjJ192iFpqxPn7mrkk7QDuVzQ3rrY5yXJG",
            CoinType::Viacoin => "via1qten42eesehw0ktddcp0fws7d3ycsqez3rmf8ld",
            CoinType::DigiByte => "dgb1qten42eesehw0ktddcp0fws7d3ycsqez379fvu9",
            CoinType::Monacoin => "MGWKYCxmN9ucWr9c7qT7ceRY2wAKz2NnGf",
            CoinType::Syscoin => "sys1qten42eesehw0ktddcp0fws7d3ycsqez3pwuwme",
            CoinType::Pivx => "DDkFr311AYe6ABMsdSnjv8yoSr1Tppokp8",
            CoinType::Firo => "a9Kd3gVz5vjegicNuG7K8f8iB5QWkUuTxW",
            CoinType::BitcoinCash => "bitcoincash:qp0xw4t8xrxae7ed4hq9a96rekynzqry2ydzeh0jgs",
            CoinType::BitcoinGold => "btg1qten42eesehw0ktddcp0fws7d3ycsqez3lht33r",
            CoinType::Ravencoin => "RHtMPHweTxYNhBYUN2nJTu9QKyjm7MRKsF",
            CoinType::eCash => "ecash:qp0xw4t8xrxae7ed4hq9a96rekynzqry2y50du5gw8",
            CoinType::Qtum => "QVD9R5M53bcd4KGJKC7fVebq4yDsnEDwtt",
            CoinType::Stratis => "strax1qten42eesehw0ktddcp0fws7d3ycsqez39n8kr5",
            CoinType::Aptos => "0x9006fa46f038224e8004bdda97f2e7a60c2c3d135bce7cb15541e5c0aae907a4",
            CoinType::Cosmos => "cosmos1ten42eesehw0ktddcp0fws7d3ycsqez3lynlqx",
            CoinType::Stargaze => "stars1ten42eesehw0ktddcp0fws7d3ycsqez3tcyzth",
            CoinType::Juno => "juno1ten42eesehw0ktddcp0fws7d3ycsqez3fksy86",
            CoinType::Stride => "stride1ten42eesehw0ktddcp0fws7d3ycsqez3u0nr52",
            CoinType::Axelar => "axelar1ten42eesehw0ktddcp0fws7d3ycsqez3m29ht8",
            CoinType::Crescent => "cre1ten42eesehw0ktddcp0fws7d3ycsqez3mvq64t",
            CoinType::Kujira => "kujira1ten42eesehw0ktddcp0fws7d3ycsqez3wv38dv",
            CoinType::Comdex => "comdex1ten42eesehw0ktddcp0fws7d3ycsqez3ct3ae3",
            CoinType::Neutron => "neutron1ten42eesehw0ktddcp0fws7d3ycsqez3mm6a6p",
            CoinType::Sommelier => "somm1ten42eesehw0ktddcp0fws7d3ycsqez3ncun3v",
            CoinType::FetchAI => "fetch1ten42eesehw0ktddcp0fws7d3ycsqez3ve6mz3",
            CoinType::Mars => "mars1ten42eesehw0ktddcp0fws7d3ycsqez3ze2x4a",
            CoinType::Umee => "umee1ten42eesehw0ktddcp0fws7d3ycsqez3djwqy5",
            CoinType::Noble => "noble1ten42eesehw0ktddcp0fws7d3ycsqez3h8xhcg",
            CoinType::Sei => "sei1ten42eesehw0ktddcp0fws7d3ycsqez3jgzfx8",
            CoinType::Tia => "celestia1ten42eesehw0ktddcp0fws7d3ycsqez3wwz06t",
            CoinType::Coreum => "core1ten42eesehw0ktddcp0fws7d3ycsqez3v2ty8a",
            CoinType::Quasar => "quasar1ten42eesehw0ktddcp0fws7d3ycsqez338fzdr",
            CoinType::Persistence => "persistence1ten42eesehw0ktddcp0fws7d3ycsqez33g4vwz",
            CoinType::Akash => "akash1ten42eesehw0ktddcp0fws7d3ycsqez3jl7ceu",
            CoinType::Terra => "terra1ten42eesehw0ktddcp0fws7d3ycsqez3eqflzx",
            CoinType::TerraV2 => "terra1ten42eesehw0ktddcp0fws7d3ycsqez3eqflzx",
            CoinType::Kava => "kava1ten42eesehw0ktddcp0fws7d3ycsqez3r38zkp",
            CoinType::Bluzelle => "bluzelle1ten42eesehw0ktddcp0fws7d3ycsqez32usaxh",
            CoinType::BandChain => "band1ten42eesehw0ktddcp0fws7d3ycsqez3xtnacw",
            CoinType::Rootstock => "0xAc1ec44E4f0ca7D172B7803f6836De87Fb72b309",
            CoinType::THORChain => "thor1ten42eesehw0ktddcp0fws7d3ycsqez3er2y4e",
            CoinType::CryptoOrg => "cro1ten42eesehw0ktddcp0fws7d3ycsqez38lmxuh",
            CoinType::Ronin => "ronin:Ac1ec44E4f0ca7D172B7803f6836De87Fb72b309",
            CoinType::Secret => "secret1ten42eesehw0ktddcp0fws7d3ycsqez3ap8ka6",
            CoinType::Osmosis => "osmo1ten42eesehw0ktddcp0fws7d3ycsqez3hlq0k5",
            CoinType::NativeEvmos => "evmos14s0vgnj0pjnazu4hsqlksdk7slah9vcfvt8ssm",
            CoinType::Agoric => "agoric1ten42eesehw0ktddcp0fws7d3ycsqez3de3qss",
            CoinType::NativeInjective => "inj14s0vgnj0pjnazu4hsqlksdk7slah9vcfyrp6ct",
            CoinType::NativeCanto => "canto14s0vgnj0pjnazu4hsqlksdk7slah9vcfuuhw7m",
            CoinType::InternetComputer => "290cc7c359f44c8516fc169c5ed4f0f3ae2e24bf5de0d4c51f5e7545b5474faa",
            CoinType::Binance => "bnb1ten42eesehw0ktddcp0fws7d3ycsqez3aqvnpg",
            CoinType::TBinance => "tbnb1ten42eesehw0ktddcp0fws7d3ycsqez3n49hpe",
            CoinType::NativeZetaChain => "zeta14s0vgnj0pjnazu4hsqlksdk7slah9vcfcwctsr",
            CoinType::Dydx => "dydx1ten42eesehw0ktddcp0fws7d3ycsqez3kaamq3",
            CoinType::Solana => "5sn9QYhDaq61jLXJ8Li5BKqGL4DDMJQvU1rdN8XgVuwC",
            CoinType::Sui => "0x01a5c6c1b74cec4fbd12b3e17252b83448136065afcdf24954dc3a9c26df4905",
            CoinType::TON => "UQCj3jAU_Ec2kXdAqweKt4rYjiwTNwiCfaUnIDHGh7wTwx_G",
            // end_of_coin_address_derivation_tests_marker_do_not_modify
            _ => panic!("{:?} must be covered", coin),
        };

        let any_address = TWAnyAddressHelper::wrap(unsafe {
            tw_any_address_create_with_public_key_derivation(
                public_key.ptr(),
                coin as u32,
                TWDerivation::Default as u32,
            )
        });

        let description =
            TWStringHelper::wrap(unsafe { tw_any_address_description(any_address.ptr()) });
        assert_eq!(
            description.to_string(),
            Some(expected_address.to_string()),
            "Invalid {:?} address",
            coin
        );
    }
}
