// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWTransactionHelper.h>

#include "TransactionHelper.h"
#include "Data.h"
#include "uint256.h"

#include <cassert>

using namespace TW;


TWData *_Nonnull TWTransactionHelperBuildInput(enum TWCoinType coinType, TWString *_Nonnull from, TWString *_Nonnull to, TWString *_Nonnull amount, TWString *_Nonnull asset, TWString *_Nonnull memo) {
    Data result;
    try {
        result = TransactionHelper::buildInput(
            coinType,
            std::string(TWStringUTF8Bytes(from)),
            std::string(TWStringUTF8Bytes(to)),
            std::string(TWStringUTF8Bytes(amount)),
            std::string(TWStringUTF8Bytes(asset)),
            std::string(TWStringUTF8Bytes(memo))
        );
    } catch (...) {
        // return empty
    }
    return TWDataCreateWithBytes(result.data(), result.size());
}

TWData *_Nonnull TWTransactionHelperPreImageHash(enum TWCoinType coinType, TWData *_Nonnull txInputData) {
    Data result;
    try {
        assert(txInputData != nullptr);
        const Data inputData = data(TWDataBytes(txInputData), TWDataSize(txInputData));

        result = TransactionHelper::preImageHash(coinType, inputData);
    } catch (...) {
        // return empty
    }
    return TWDataCreateWithBytes(result.data(), result.size());
}

TWData *_Nonnull TWTransactionHelperCompileWithSignature(enum TWCoinType coinType, TWData *_Nonnull txInputData, TWData *_Nonnull signature, TWData *_Nonnull publicKey) {
    Data result;
    try {
        assert(txInputData != nullptr);
        const Data inputData = data(TWDataBytes(txInputData), TWDataSize(txInputData));
        assert(signature != nullptr);
        const Data signatureData = data(TWDataBytes(signature), TWDataSize(signature));
        assert(publicKey != nullptr);
        const Data publicKeyData = data(TWDataBytes(publicKey), TWDataSize(publicKey));

        result  = TransactionHelper::compileWithSignature(coinType, inputData, signatureData, publicKeyData);
    } catch (...) {
        // return empty
    }
    return TWDataCreateWithBytes(result.data(), result.size());
}
