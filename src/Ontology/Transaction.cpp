// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <string>

#include "../Hash.h"

#include "Address.h"
#include "Transaction.h"
#include "ParamsBuilder.h"

using namespace TW;
using namespace TW::Ontology;

std::vector<uint8_t> Transaction::serializeUnsigned() {
    ParamsBuilder builder;
    builder.pushBack(version);
    builder.pushBack(txType);
    builder.pushBack(nonce);
    builder.pushBack(gasPrice);
    builder.pushBack(gasLimit);
    if (payer.length() != 0) {
        builder.pushBack(Address(payer).data);
    }
    if (!payload.empty()) {
        builder.pushVar(payload);
    }
    builder.pushBack((uint8_t) 0x00);
    return builder.getBytes();
}

std::vector<uint8_t> Transaction::serialize() {
    std::vector<uint8_t> txData;
    auto unsignedData = serializeUnsigned();
    txData.insert(txData.end(), unsignedData.begin(), unsignedData.end());
    ParamsBuilder builder;
    builder.pushVar(sigVec.size());
    for (auto sig : sigVec) {
        builder.pushBack(sig.serialize());
    }
    auto sigData = builder.getBytes();
    txData.insert(txData.end(), sigData.begin(), sigData.end());
    return txData;
}

std::vector<uint8_t> Transaction::txHash() {
    auto txSerialized = Transaction::serializeUnsigned();
    auto digest = Hash::sha256(Hash::sha256(txSerialized));
    return digest;
}

std::vector<uint8_t> Transaction::serialize(const PublicKey &pk) {
    ParamsBuilder builder;
    builder.push(pk.bytes);
    builder.pushBack((uint8_t) 0xAC);
    return builder.getBytes();
}

void Transaction::sign(const Account &acct) {
    if (sigVec.size() >= sigVecLimit) {
        throw std::runtime_error("the number of transaction signatures should not be over 16.");
    }
    auto signature = acct.getPrivateKey().sign(txHash(), TWCurveNIST256p1);
    signature.pop_back();
    auto sigData = SigData(acct.getPublicKey().bytes, signature, 1);
    sigVec.clear();
    sigVec.push_back(sigData);
}

void Transaction::addSign(const Account &acct) {
    if (sigVec.size() >= sigVecLimit) {
        throw std::runtime_error("the number of transaction signatures should not be over 16.");
    }
    auto signature = acct.getPrivateKey().sign(txHash(), TWCurveNIST256p1);
    signature.pop_back();
    auto sigData = SigData(acct.getPublicKey().bytes, signature, 1);
    sigVec.push_back(sigData);
}