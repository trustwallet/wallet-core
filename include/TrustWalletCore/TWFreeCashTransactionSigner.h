// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWFreeCashProto.h"
#include "TWCommonProto.h"

TW_EXTERN_C_BEGIN

/// Helper class to sign FreeCash transactions.
TW_EXPORT_CLASS
struct TWFreeCashTransactionSigner;

/// Creates a transaction signer with input data (serialized from FreeCashSigningInput)
TW_EXPORT_STATIC_METHOD
struct TWFreeCashTransactionSigner *_Nonnull TWFreeCashTransactionSignerCreate(TW_FreeCash_Proto_SigningInput input);

/// Creates a transaction signer with input data (serialized from Proto::SigningInput) and plan
TW_EXPORT_STATIC_METHOD
struct TWFreeCashTransactionSigner *_Nonnull TWFreeCashTransactionSignerCreateWithPlan(TW_FreeCash_Proto_SigningInput input, TW_FreeCash_Proto_TransactionPlan plan);

TW_EXPORT_METHOD
void TWFreeCashTransactionSignerDelete(struct TWFreeCashTransactionSigner *_Nonnull signer);

/// Plans a transaction.
TW_EXPORT_METHOD
TW_FreeCash_Proto_TransactionPlan TWFreeCashTransactionSignerPlan(struct TWFreeCashTransactionSigner *_Nonnull signer);

/// Signs the transaction.
///
/// On success the result will contain a FreeCashSigningOutput.
TW_EXPORT_METHOD
TW_Proto_Result TWFreeCashTransactionSignerSign(struct TWFreeCashTransactionSigner *_Nonnull signer);

TW_EXTERN_C_END
