// Copyright © 2017-2022 Trust Wallet.
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

struct TWDataVector* _Nonnull createFromVector(const HashPubkeyList& dataVector) {
    auto ret = TWDataVectorCreate();
    for (auto& i: dataVector) {
        const auto& hash = std::get<0>(i);
        const auto& pubkeyhash = std::get<1>(i);
        auto newElem1 = TWDataCreateWithBytes(hash.data(), hash.size());
        TWDataVectorAdd(ret, newElem1);
        TWDataDelete(newElem1);
        auto newElem2 = TWDataCreateWithBytes(pubkeyhash.data(), pubkeyhash.size());
        TWDataVectorAdd(ret, newElem2);
        TWDataDelete(newElem2);
    }
    return ret;
}

std::vector<Data> createFromTWDataVector(const struct TWDataVector* _Nonnull dataVector) {
    std::vector<Data> ret;
    const auto n = TWDataVectorSize(dataVector);
    for (auto i = 0; i < n; ++i) {
        auto elem = TWDataVectorGet(dataVector, i);
        ret.push_back(*(static_cast<const Data*>(elem)));
        TWDataDelete(elem);
    }
    return ret;
}

struct TWDataVector *_Nonnull TWTransactionCompilerPreImageHashes(enum TWCoinType coinType, TWData *_Nonnull txInputData) {
    HashPubkeyList result;
    try {
        assert(txInputData != nullptr);
        const Data inputData = data(TWDataBytes(txInputData), TWDataSize(txInputData));

        result = TransactionCompiler::preImageHashes(coinType, inputData);
    } catch (...) {} // return empty
    return createFromVector(result);
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
