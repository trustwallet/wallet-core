//! This file contains custom implementations for two TW-* enums that implement
//! string representations for their variants. Given that we want to deprecate
//! the C header files and just maintain the manifest in the future - where the
//! string representations are saved in an explicit `as_string` field -  it's
//! easier to just handle those enums like this instead of having to handle
//! `static const char * ...` types. Once that change happens, this module will
//! be removed.
//!
//! See https://github.com/trustwallet/wallet-core/pull/3122 for more info.

use crate::manifest::{EnumInfo, EnumVariantInfo, TypeVariant};

pub fn custom_handle_stellar_passphrase() -> EnumInfo {
    let variants = vec![
        EnumVariantInfo {
            name: "stellar".to_string(),
            value: 0,
            as_string: Some("Public Global Stellar Network ; September 2015".to_string()),
        },
        EnumVariantInfo {
            name: "kin".to_string(),
            value: 1,
            as_string: Some("Kin Mainnet ; December 2018".to_string()),
        },
    ];

    EnumInfo {
        name: "TWStellarPassphrase".to_string(),
        is_public: true,
        value_type: TypeVariant::UInt32T,
        variants,
    }
}

pub fn custom_handle_hrp() -> EnumInfo {
    let raw = vec![
        ("unknown", ""),
        ("bitcoin", "bc"),
        ("litecoin", "ltc"),
        ("viacoin", "via"),
        ("groestlcoin", "grs"),
        ("digiByte", "dgb"),
        ("monacoin", "mona"),
        ("cosmos", "cosmos"),
        ("bitcoinCash", "bitcoincash"),
        ("bitcoinGold", "btg"),
        ("ioTeX", "io"),
        ("nervos", "ckb"),
        ("zilliqa", "zil"),
        ("terra", "terra"),
        ("cryptoOrg", "cro"),
        ("kava", "kava"),
        ("oasis", "oasis"),
        ("bluzelle", "bluzelle"),
        ("bandChain", "band"),
        ("multiversX", "erd"),
        ("secret", "secret"),
        ("agoric", "agoric"),
        ("binance", "bnb"),
        ("ecash", "ecash"),
        ("thorchain", "thor"),
        ("harmony", "one"),
        ("cardano", "addr"),
        ("qtum", "qc"),
        ("nativeInjective", "inj"),
        ("osmosis", "osmo"),
        ("terraV2", "terra"),
        ("coreum", "core"),
        ("nativeCanto", "canto"),
        ("sommelier", "somm"),
        ("fetchAI", "fetch"),
        ("mars", "mars"),
        ("umee", "umee"),
        ("quasar", "quasar"),
        ("persistence", "persistence"),
        ("akash", "akash"),
        ("noble", "noble"),
        ("stargaze", "stars"),
        ("nativeEvmos", "evmos"),
        ("juno", "juno"),
        ("stride", "stride"),
        ("axelar", "axelar"),
        ("crescent", "cre"),
        ("kujira", "kujira"),
        ("comdex", "comdex"),
        ("neutron", "neutron"),
    ];

    let variants = raw
        .into_iter()
        .enumerate()
        .map(|(value, (name, as_string))| EnumVariantInfo {
            name: name.to_string(),
            value,
            as_string: Some(as_string.to_string()),
        })
        .collect();

    EnumInfo {
        name: "TWHRP".to_string(),
        is_public: true,
        value_type: TypeVariant::UInt32T,
        variants,
    }
}
