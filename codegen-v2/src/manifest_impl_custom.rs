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
        name: "StellarPassphrase".to_string(),
        is_public: true,
        variants,
        tags: vec![],
    }
}
