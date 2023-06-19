// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Transaction.h"
#include "Address.h"
#include "ParamsBuilder.h"

#include <string>

namespace TW::Ontology {

const std::string Transaction::ZERO_PAYER = "AFmseVrdL9f9oyCzZefL9tG6UbvhPbdYzM";

std::vector<uint8_t> Transaction::serializeUnsigned() {
    ParamsBuilder builder;
    builder.pushBack(version);
    builder.pushBack(txType);
    builder.pushBack(nonce);
    builder.pushBack(gasPrice);
    builder.pushBack(gasLimit);
    builder.pushBack(Address(payer)._data);
    if (!payload.empty()) {
        builder.pushVar(payload);
    }
    builder.pushBack((uint8_t)0x00);
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
    return Hash::sha256(Hash::sha256(Hash::sha256(txSerialized)));
}

std::vector<uint8_t> Transaction::serialize(const PublicKey& pk) {
    ParamsBuilder builder;
    builder.push(pk.bytes);
    builder.pushBack((uint8_t)0xAC);
    return builder.getBytes();
}

} // namespace TW::Ontology
