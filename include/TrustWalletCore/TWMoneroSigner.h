// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWMoneroProto.h"

TW_EXTERN_C_BEGIN

/// Helper class to sign Monero transactions
TW_EXPORT_CLASS
struct TWMoneroSigner;

/// Signs a transaction
TW_EXPORT_STATIC_METHOD
TW_Monero_Proto_SigningOutput TWMoneroSignerSign(TW_Monero_Proto_SigningInput);

TW_EXTERN_C_END