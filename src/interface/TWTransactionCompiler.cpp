// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWTransactionCompiler.h>

#include "TransactionCompiler.h"
#include "Data.h"
#include "uint256.h"

#include <cassert>

using namespace TW;


TWData *_Nonnull TWTransactionCompilerBuildInput(enum TWCoinType coinType, TWString *_Nonnull from, TWString *_Nonnull to, TWString *_Nonnull amount, TWString *_Nonnull asset, TWString *_Nonnull memo, TWString *_Nonnull chainId) {
    Data result;
    try {
        result = TransactionCompiler::buildInput(
            coinType,
            std::string(TWStringUTF8Bytes(from)),
            std::string(TWStringUTF8Bytes(to)),
            std::string(TWStringUTF8Bytes(amount)),
            std::string(TWStringUTF8Bytes(asset)),
            std::string(TWStringUTF8Bytes(memo)),
            std::string(TWStringUTF8Bytes(chainId))
        );
    } catch (...) {} // return empty
    return TWDataCreateWithBytes(result.data(), result.size());
}

static std::vector<Data> createFromTWDataVector(const struct TWDataVector* _Nonnull dataVector) {
    std::vector<Data> ret;
    const auto n = TWDataVectorSize(dataVector);
    for (auto i = 0uL; i < n; ++i) {
        const auto* const elem = TWDataVectorGet(dataVector, i);
        if (const auto* const data = reinterpret_cast<const Data *>(elem); data) {
            ret.emplace_back(*data);
            TWDataDelete(elem);
        }
    }
    return ret;
}

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
