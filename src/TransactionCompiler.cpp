// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TransactionCompiler.h"

#include "Coin.h"

using namespace TW;

Data TransactionCompiler::preImageHashes(TWCoinType coinType, const Data& txInputData) {
    return anyCoinPreImageHashes(coinType, txInputData);
}

Data TransactionCompiler::compileWithSignatures(TWCoinType coinType, const Data& txInputData, const std::vector<Data>& signatures, const std::vector<Data>& publicKeys) {
    // input parameter conversion
    const auto publicKeyType = ::publicKeyType(coinType);
    std::vector<PublicKey> pubs;
    for (auto& p: publicKeys) {
        if (!PublicKey::isValid(p, publicKeyType)) {
            throw std::invalid_argument("Invalid public key");
        }
        pubs.emplace_back(p, publicKeyType);
    }

    Data txOutput;
    anyCoinCompileWithSignatures(coinType, txInputData, signatures, pubs, txOutput);
    return txOutput;
}

Data TransactionCompiler::compileWithSignaturesAndPubKeyType(TWCoinType coinType, const Data& txInputData, const std::vector<Data>& signatures, const std::vector<Data>& publicKeys, enum TWPublicKeyType pubKeyType) {
    std::vector<PublicKey> pubs;
    for (auto& p: publicKeys) {
        if (!PublicKey::isValid(p, pubKeyType)) {
            throw std::invalid_argument("Invalid public key");
        }
        pubs.push_back(PublicKey(p, pubKeyType));
    }

    Data txOutput;
    anyCoinCompileWithSignatures(coinType, txInputData, signatures, pubs, txOutput);
    return txOutput;
}
