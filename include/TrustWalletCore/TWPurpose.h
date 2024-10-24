// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"

TW_EXTERN_C_BEGIN

/// HD wallet purpose
///
/// \see https://github.com/bitcoin/bips/blob/master/bip-0044.mediawiki
/// \see https://github.com/bitcoin/bips/blob/master/bip-0049.mediawiki
/// \see https://github.com/bitcoin/bips/blob/master/bip-0084.mediawiki
TW_EXPORT_ENUM(uint32_t)
enum TWPurpose {
    TWPurposeBIP44 = 44,
    TWPurposeBIP49 = 49, // Derivation scheme for P2WPKH-nested-in-P2SH
    TWPurposeBIP84 = 84, // Derivation scheme for P2WPKH
    TWPurposeBIP86 = 86, // Derivation scheme for P2TR
    TWPurposeBIP1852 = 1852, // Derivation scheme used by Cardano-Shelley
};

TW_EXTERN_C_END
