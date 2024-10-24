// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.
//
// This is a GENERATED FILE from \registry.json, changes made here WILL BE LOST.
//

#pragma once

#include "TWBase.h"

TW_EXTERN_C_BEGIN

/// Non-default coin address derivation names (default, unnamed derivations are not included).
/// Note the enum variant must be sync with `TWDerivation` enum in Rust:
/// https://github.com/trustwallet/wallet-core/blob/master/rust/tw_coin_registry/src/tw_derivation.rs
TW_EXPORT_ENUM()
enum TWDerivation {
    TWDerivationDefault = 0, // default, for any coin
    TWDerivationCustom = 1, // custom, for any coin
    TWDerivationBitcoinSegwit = 2,
    TWDerivationBitcoinLegacy = 3,
    TWDerivationBitcoinTestnet = 4,
    TWDerivationLitecoinLegacy = 5,
    TWDerivationSolanaSolana = 6,
    TWDerivationStratisSegwit = 7,
    TWDerivationBitcoinTaproot = 8,
    // end_of_derivation_enum - USED TO GENERATE CODE
};

TW_EXTERN_C_END
