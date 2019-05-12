// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWARKProto.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_CLASS
struct TWARKSigner;

/// Signs a transaction.
TW_EXPORT_STATIC_METHOD
TW_ARK_Proto_SigningOutput TWARKSignerSign(TW_ARK_Proto_SigningInput input);

TW_EXTERN_C_END
