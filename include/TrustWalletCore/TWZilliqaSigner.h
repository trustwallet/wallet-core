// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWZilliqaProto.h"

TW_EXTERN_C_BEGIN

const uint32_t TWZilliqaTxVersion = 65537;

/// Helper class to sign Zilliqa transactions.
TW_EXPORT_CLASS
struct TWZilliqaSigner;

/// Signs a transaction.
TW_EXPORT_STATIC_METHOD
TW_Zilliqa_Proto_SigningOutput TWZilliqaSignerSign(TW_Zilliqa_Proto_SigningInput input);

TW_EXTERN_C_END
