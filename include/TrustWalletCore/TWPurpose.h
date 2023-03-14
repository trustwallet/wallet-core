// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
    TWPurposeBIP1852 = 1852, // Derivation scheme used by Cardano-Shelley
};

TW_EXTERN_C_END
