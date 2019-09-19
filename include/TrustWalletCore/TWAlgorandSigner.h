// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
#pragma once

#include "TWAlgorandProto.h"

TW_EXTERN_C_BEGIN

/// Helper class to sign Algorand transactions.
TW_EXPORT_CLASS
struct TWAlgorandSigner;

/// Signs a transaction.
TW_EXPORT_STATIC_METHOD
TW_Algorand_Proto_SigningOutput TWAlgorandSignerSign(TW_Algorand_Proto_SigningInput input);

TW_EXTERN_C_END
