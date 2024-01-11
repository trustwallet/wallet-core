// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"

TW_EXTERN_C_BEGIN

/// Stellar network passphrase string.
TW_EXPORT_ENUM()
enum TWStellarPassphrase {
    TWStellarPassphraseStellar     /* "Public Global Stellar Network ; September 2015" */,
    TWStellarPassphraseKin         /* "Kin Mainnet ; December 2018" */,
};

static const char *_Nonnull TWStellarPassphrase_Stellar = "Public Global Stellar Network ; September 2015";
static const char *_Nonnull TWStellarPassphrase_Kin = "Kin Mainnet ; December 2018";

TW_EXTERN_C_END
