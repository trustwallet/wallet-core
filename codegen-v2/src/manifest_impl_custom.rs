//! This file contains custom implementations for two TW-* enums that implement
//! string representations for their variants. Given that we want to deprecate
//! the C header files and just maintain the manifest in the future - where the
//! string representations are saved in an explicit `as_string` field -  it's
//! easier to just handle those enums like this instead of having to handle
//! `static const char * ...` types. Once that change happens, this module will
//! be removed.
//!
//! See https://github.com/trustwallet/wallet-core/pull/3122 for more info.

use crate::manifest::{EnumInfo, EnumVariantInfo};

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
        variants,
        tags: vec![],
    }
}

pub fn custom_handle_hrp() -> EnumInfo {
    let raw = vec![
        ("unknown", ""),
        ("bitcoin", "bc"),
        ("litecoin", "ltc"),
        ("viacoin", "via"),
        ("groestlcoin", "grs"),
        ("digibyte", "dgb"),
        ("monacoin", "mona"),
        ("cosmos", "cosmos"),
        ("bitcoincash", "bitcoincash"),
        ("bitcoingold", "btg"),
        ("iotex", "io"),
        ("nervos", "ckb"),
        ("zilliqa", "zil"),
        ("terra", "terra"),
        ("cryptoorg", "cro"),
        ("kava", "kava"),
        ("oasis", "oasis"),
        ("bluzelle", "bluzelle"),
        ("bandchain", "band"),
        ("multiversx", "erd"),
        ("secret", "secret"),
        ("agoric", "agoric"),
        ("binance", "bnb"),
        ("ecash", "ecash"),
        ("thorchain", "thor"),
        ("harmony", "one"),
        ("cardano", "addr"),
        ("qtum", "qc"),
        ("nativeinjective", "inj"),
        ("osmosis", "osmo"),
        ("terrav2", "terra"),
        ("coreum", "core"),
        ("nativecanto", "canto"),
        ("sommelier", "somm"),
        ("fetchai", "fetch"),
        ("mars", "mars"),
        ("umee", "umee"),
        ("quasar", "quasar"),
        ("persistence", "persistence"),
        ("akash", "akash"),
        ("noble", "noble"),
        ("stargaze", "stars"),
        ("nativeevmos", "evmos"),
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
        variants,
        tags: vec![],
    }
}
