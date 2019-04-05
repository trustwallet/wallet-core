// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWNULSProto.h"

TW_EXTERN_C_BEGIN

/// Helper class to sign NULS transactions.
TW_EXPORT_CLASS
struct TWNULSSigner;

/// Make a transaction plan
TW_EXPORT_STATIC_METHOD
TW_NULS_Proto_TransactionPlan TWNULSSignerPlan(TW_NULS_Proto_TransactionPurpose purpose);

/// Signs a transaction.
TW_EXPORT_STATIC_METHOD
TW_NULS_Proto_SigningOutput TWNULSSignerSign(TW_NULS_Proto_TransactionPlan plan);

TW_EXTERN_C_END
