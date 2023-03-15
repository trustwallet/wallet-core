// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
