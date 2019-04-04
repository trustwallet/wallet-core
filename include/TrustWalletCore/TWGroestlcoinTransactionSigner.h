// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWBitcoinProto.h"
#include "TWCommonProto.h"

TW_EXTERN_C_BEGIN

/// Helper class to sign Groestlcoin transactions.
TW_EXPORT_CLASS
struct TWGroestlcoinTransactionSigner;

/// Creates a transaction signer with input data (serialized from BitcoinSigningInput)
TW_EXPORT_STATIC_METHOD
struct TWGroestlcoinTransactionSigner *_Nonnull TWGroestlcoinTransactionSignerCreate(TW_Bitcoin_Proto_SigningInput input);

/// Creates a transaction signer with input data (serialized from Proto::SigningInput) and plan
TW_EXPORT_STATIC_METHOD
struct TWGroestlcoinTransactionSigner *_Nonnull TWGroestlcoinTransactionSignerCreateWithPlan(TW_Bitcoin_Proto_SigningInput input, TW_Bitcoin_Proto_TransactionPlan plan);

TW_EXPORT_METHOD
void TWGroestlcoinTransactionSignerDelete(struct TWGroestlcoinTransactionSigner *_Nonnull signer);

/// Plans a transaction.
TW_EXPORT_METHOD
TW_Bitcoin_Proto_TransactionPlan TWGroestlcoinTransactionSignerPlan(struct TWGroestlcoinTransactionSigner *_Nonnull signer);

/// Signs the transaction.
///
/// On success the result will contain a BitcoinSigningOutput.
TW_EXPORT_METHOD
TW_Proto_Result TWGroestlcoinTransactionSignerSign(struct TWGroestlcoinTransactionSigner *_Nonnull signer);

TW_EXTERN_C_END
