// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWIocoinProto.h"
#include "TWCommonProto.h"

TW_EXTERN_C_BEGIN

/// Helper class to sign Iocoin transactions.
TW_EXPORT_CLASS
struct TWIocoinTransactionSigner;

/// Creates a transaction signer with input data (serialized from IocoinSigningInput)
TW_EXPORT_STATIC_METHOD
struct TWIocoinTransactionSigner *_Nonnull TWIocoinTransactionSignerCreate(TW_Iocoin_Proto_SigningInput input);

/// Creates a transaction signer with input data (serialized from Proto::SigningInput) and plan
TW_EXPORT_STATIC_METHOD
struct TWIocoinTransactionSigner *_Nonnull TWIocoinTransactionSignerCreateWithPlan(TW_Iocoin_Proto_SigningInput input, TW_Iocoin_Proto_TransactionPlan plan);

TW_EXPORT_METHOD
void TWIocoinTransactionSignerDelete(struct TWIocoinTransactionSigner *_Nonnull signer);

/// Plans a transaction.
TW_EXPORT_METHOD
TW_Iocoin_Proto_TransactionPlan TWIocoinTransactionSignerPlan(struct TWIocoinTransactionSigner *_Nonnull signer);

/// Signs the transaction.
///
/// On success the result will contain a IocoinSigningOutput.
TW_EXPORT_METHOD
TW_Proto_Result TWIocoinTransactionSignerSign(struct TWIocoinTransactionSigner *_Nonnull signer);

TW_EXTERN_C_END
