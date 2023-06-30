// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <ctype.h>

#include "InvocationTransaction.h"
#include "MinerTransaction.h"
#include "Transaction.h"
#include "Data.h"
#include "../Hash.h"
#include "../uint256.h"

using namespace std;
using namespace TW;

namespace TW::NEO {

size_t Transaction::size() const {
    return serialize().size();
}

void Transaction::deserialize(const Data& data, size_t initial_pos) {
    type = (TransactionType)data[initial_pos++];
    version = data[initial_pos++];
    initial_pos = deserializeExclusiveData(data, initial_pos);
    attributes.clear();
    initial_pos = Serializable::deserialize<TransactionAttribute>(attributes, data, initial_pos);
    inInputs.clear();
    initial_pos = Serializable::deserialize<CoinReference>(inInputs, data, initial_pos);
    outputs.clear();
    Serializable::deserialize<TransactionOutput>(outputs, data, initial_pos);
}

Transaction* Transaction::deserializeFrom(const Data& data, size_t initial_pos) {
    Transaction* resp = nullptr;
    switch ((TransactionType)data[initial_pos]) {
    case TransactionType::TT_MinerTransaction:
        resp = new MinerTransaction();
        break;
    case TransactionType::TT_ContractTransaction:
        resp = new Transaction();
        break;
    case TransactionType::TT_InvocationTransaction:
        resp = new InvocationTransaction();
        break;
    default:
        throw std::invalid_argument("Transaction::deserializeFrom Invalid transaction type");
        break;
    }
    resp->deserialize(data, initial_pos);
    return resp;
}

Data Transaction::serialize() const {
    Data resp;
    resp.push_back((byte)type);
    resp.push_back(version);
    append(resp, serializeExclusiveData());

    append(resp, Serializable::serialize(attributes));
    append(resp, Serializable::serialize(inInputs));
    append(resp, Serializable::serialize(outputs));
    if (witnesses.size()) {
        resp.push_back((byte)witnesses.size());
        for (const auto& witnesse : witnesses)
            append(resp, witnesse.serialize());
    }

    return resp;
}

bool Transaction::operator==(const Transaction& other) const {
    if (this == &other) {
        return true;
    }
    // clang-format off
    return this->type == other.type
        && this->version == other.version
        && this->attributes.size() == other.attributes.size()
        && this->inInputs.size() == other.inInputs.size()
        && this->outputs.size() == other.outputs.size()
        && this->attributes == other.attributes
        && this->inInputs == other.inInputs
        && this->outputs == other.outputs;
    // clang-format on
}

Data Transaction::getHash() const {
    return Hash::sha256(Hash::sha256(serialize()));
}

uint256_t Transaction::getHashUInt256() const {
    return load(getHash());
}

} // namespace TW::NEO
