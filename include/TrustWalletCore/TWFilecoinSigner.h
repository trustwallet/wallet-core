// Copyright © 2017-2020 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWFilecoinProto.h"

TW_EXTERN_C_BEGIN

/// Helper class to sign Filecoin transactions.
TW_EXPORT_CLASS
struct TWFilecoinSigner;

/// Signs a transaction.
TW_EXPORT_STATIC_METHOD
TW_Filecoin_Proto_SigningOutput TWFilecoinSignerSign(TW_Filecoin_Proto_SigningInput input);

TW_EXTERN_C_END
