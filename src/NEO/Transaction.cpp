#include <ctype.h>

#include "../uint256.h"
#include "../UInt.hpp"
#include "../Data.h"
#include "../ReadData.h"
#include "../HexCoding.h"
#include "Transaction.h"
#include "MinerTransaction.hpp"

#include <iostream>
using namespace std;

using namespace TW;
using namespace TW::NEO;

#define SIZE_INC(V, X) {\
    (V) += 8; \
    for (auto &__x_ : (X)) { \
        (V) += __x_.size(); \
    } \
}
int64_t Transaction::size() const {
    auto resp = 1 + 1; // sizeof(type) + sizeof(version);
    SIZE_INC(resp, attributes);
    SIZE_INC(resp, inInputs);
    SIZE_INC(resp, outputs);
//    SIZE_INC(resp, witnesses); // witnessess is not serialized

    return resp;
}
#undef SIZE_INC

void Transaction::deserialize(const Data &data, int initial_pos) {
    type = (TransactionType) data[initial_pos++];
    version = data[initial_pos++];
    initial_pos = deserializeExclusiveData(data, initial_pos);
    attributes.clear();
    initial_pos = ISerializable::deserialize<TransactionAttribute>(attributes, data, initial_pos);
    inInputs.clear();
    initial_pos = ISerializable::deserialize<CoinReference>(inInputs, data, initial_pos);
    outputs.clear();
    ISerializable::deserialize<TransactionOutput>(outputs, data, initial_pos);

    // witnesses.clear();
    // initial_pos = ISerializable::deserialize<Witness>(witnesses, data, initial_pos);
}

Transaction * Transaction::deserializeFrom(const Data &data, int initial_pos) {
    Transaction * resp = nullptr;
    switch ((TransactionType) data[initial_pos]) {
        case TransactionType::TT_MinerTransaction:
            resp = new MinerTransaction();
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
    resp.push_back((byte) type);
    resp.push_back(version);
    append(resp, serializeExclusiveData());

    append(resp, ISerializable::serialize(attributes));
    append(resp, ISerializable::serialize(inInputs));
    append(resp, ISerializable::serialize(outputs));
    // append(resp, ISerializable::serialize(witnesses));

    return resp;
}

bool Transaction::operator==(const Transaction &other) const {
    if (this == &other) {
        return true;
    }
    return this->type == other.type
        && this->version == other.version
        && this->attributes.size() == other.attributes.size()
        && this->inInputs.size() == other.inInputs.size()
        && this->outputs.size() == other.outputs.size()
        && this->attributes == other.attributes
        && this->inInputs == other.inInputs
        && this->outputs == other.outputs;
}

Data Transaction::getHash() const {
    return Hash::sha256(Hash::sha256(serialize()));
}

uint256_t Transaction::getHashUInt256() const {
    return load<uint256_t>(getHash());
}
