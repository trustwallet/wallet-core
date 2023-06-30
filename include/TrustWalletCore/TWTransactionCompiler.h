// Copyright © 2017-2023 Trust Wallet.
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

/// Builds a coin-specific SigningInput (proto object) from a simple transaction.
///
/// \param coin coin type.
/// \param from sender of the transaction.
/// \param to receiver of the transaction.
/// \param amount transaction amount in string
/// \param asset optional asset name, like "BNB"
/// \param memo optional memo
/// \param chainId optional chainId to override default
/// \return serialized data of the SigningInput proto object.
TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWTransactionCompilerBuildInput(enum TWCoinType coinType, TWString* _Nonnull from,
                                                 TWString* _Nonnull to, TWString* _Nonnull amount,
                                                 TWString* _Nonnull asset, TWString* _Nonnull memo,
                                                 TWString* _Nonnull chainId);

/// Obtains pre-signing hashes of a transaction.
///
/// We provide a default `PreSigningOutput` in TransactionCompiler.proto. 
/// For some special coins, such as bitcoin, we will create a custom `PreSigningOutput` object in its proto file.
/// \param coin coin type.
/// \param txInputData The serialized data of a signing input
/// \return serialized data of a proto object `PreSigningOutput` includes hash.
TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWTransactionCompilerPreImageHashes(enum TWCoinType coinType,
                                                     TWData* _Nonnull txInputData);

/// Compiles a complete transation with one or more external signatures.
/// 
/// Puts together from transaction input and provided public keys and signatures. The signatures must match the hashes
/// returned by TWTransactionCompilerPreImageHashes, in the same order. The publicKeyHash attached
/// to the hashes enable identifying the private key needed for signing the hash.
/// \param coin coin type.
/// \param txInputData The serialized data of a signing input.
/// \param signatures signatures to compile, using TWDataVector.
/// \param publicKeys public keys for signers to match private keys, using TWDataVector.
/// \return serialized data of a proto object `SigningOutput`.
TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWTransactionCompilerCompileWithSignatures(
    enum TWCoinType coinType, TWData* _Nonnull txInputData,
    const struct TWDataVector* _Nonnull signatures, const struct TWDataVector* _Nonnull publicKeys);

TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWTransactionCompilerCompileWithSignaturesAndPubKeyType(
    enum TWCoinType coinType, TWData *_Nonnull txInputData,
    const struct TWDataVector *_Nonnull signatures, const struct TWDataVector *_Nonnull publicKeys,
    enum TWPublicKeyType pubKeyType);

TW_EXTERN_C_END
