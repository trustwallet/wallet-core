// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWNEOProto.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_CLASS
struct TWNEOSigner;

TW_EXPORT_STATIC_METHOD
TW_NEO_Proto_TransactionPlan TWNEOSignerPlanTransaction(TW_NEO_Proto_SigningInput input);

TW_EXPORT_STATIC_METHOD
TW_NEO_Proto_SigningOutput TWNEOSignerSign(TW_NEO_Proto_SigningInput input, TW_NEO_Proto_TransactionPlan plan);

TW_EXTERN_C_END
