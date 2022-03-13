// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWCoinType.h"
#include "TWData.h"
#include "TWDataVector.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

/// Non-core transaction utility methods, like building a transaction using an external signature.
TW_EXPORT_STRUCT
struct TWTransactionCompiler;

/// Build a coin-specific SigningInput protobuf transaction input, from simple transaction parameters
/// - amount: decimal number as string
/// - asset: optional asset name, like "BNB"
/// - memo: optional memo
/// - chainId: optional chainId to override default
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWTransactionCompilerBuildInput(enum TWCoinType coinType, TWString *_Nonnull from, TWString *_Nonnull to, TWString *_Nonnull amount, TWString *_Nonnull asset, TWString *_Nonnull memo, TWString *_Nonnull chainId);

/// Obtain pre-signing hashes of a transaction. For each hash its corresponding publickKeyHash follows, so return is always even size.
/// For chains with one signature only, the result contains 2 elements, but the second may be ignored.
TW_EXPORT_STATIC_METHOD
struct TWDataVector *_Nonnull TWTransactionCompilerPreImageHashes(enum TWCoinType coinType, TWData *_Nonnull txInputData);

/// Compile a complete transation with one or more external signatures, put together from transaction input and provided public keys and signatures.
/// The signatures must match the hashes returned by TWTransactionCompilerPreImageHashes, in the same order.
/// The publicKeyHash attached to the hashes enable identifying the private key needed for signing the hash.
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWTransactionCompilerCompileWithSignatures(enum TWCoinType coinType, TWData *_Nonnull txInputData, const struct TWDataVector *_Nonnull signatures, const struct TWDataVector *_Nonnull publicKeys);

TW_EXTERN_C_END
