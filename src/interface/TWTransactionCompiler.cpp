// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <TrustWalletCore/TWTransactionCompiler.h>

#include "TransactionCompiler.h"
#include "DataVector.h"

#include <cassert>

using namespace TW;

TWData *_Nonnull TWTransactionCompilerPreImageHashes(enum TWCoinType coinType, TWData *_Nonnull txInputData) {
    Data result;
    try {
        assert(txInputData != nullptr);
        const Data inputData = data(TWDataBytes(txInputData), TWDataSize(txInputData));

        result = TransactionCompiler::preImageHashes(coinType, inputData);
    } catch (...) {} // return empty
    return TWDataCreateWithBytes(result.data(), result.size());
}

TWData *_Nonnull TWTransactionCompilerCompileWithSignatures(enum TWCoinType coinType, TWData *_Nonnull txInputData, const struct TWDataVector *_Nonnull signatures, const struct TWDataVector *_Nonnull publicKeys) {
    Data result;
    try {
        assert(txInputData != nullptr);
        const Data inputData = data(TWDataBytes(txInputData), TWDataSize(txInputData));
        assert(signatures != nullptr);
        const auto signaturesVec = createFromTWDataVector(signatures);
        assert(publicKeys != nullptr);
        const auto publicKeysVec = createFromTWDataVector(publicKeys);

        result  = TransactionCompiler::compileWithSignatures(coinType, inputData, signaturesVec, publicKeysVec);
    } catch (...) {} // return empty
    return TWDataCreateWithBytes(result.data(), result.size());
}

TWData *_Nonnull TWTransactionCompilerCompileWithSignaturesAndPubKeyType(enum TWCoinType coinType, TWData *_Nonnull txInputData, const struct TWDataVector *_Nonnull signatures, const struct TWDataVector *_Nonnull publicKeys, enum TWPublicKeyType pubKeyType) {
    Data result;
    try {
        assert(txInputData != nullptr);
        const Data inputData = data(TWDataBytes(txInputData), TWDataSize(txInputData));
        assert(signatures != nullptr);
        const auto signaturesVec = createFromTWDataVector(signatures);
        assert(publicKeys != nullptr);
        const auto publicKeysVec = createFromTWDataVector(publicKeys);

        result  = TransactionCompiler::compileWithSignaturesAndPubKeyType(coinType, inputData, signaturesVec, publicKeysVec, pubKeyType);
    } catch (...) {} // return empty
    return TWDataCreateWithBytes(result.data(), result.size());
}
