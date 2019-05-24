// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWJsonProto.h"
#include "TWCoinType.h"
#include "TWPrivateKey.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_CLASS
struct TWJsonSigner;

/// Signs a transaction.
TW_EXPORT_STATIC_METHOD
TW_Json_Proto_SigningOutput TWJsonSignerSign(enum TWCoinType coin, TWString *_Nonnull transaction, TWString *_Nonnull privateKey);


TW_EXTERN_C_END