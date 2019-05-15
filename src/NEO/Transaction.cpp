#include <ctype.h>

#include "../uint256.h"
#include "../UInt.hpp"
#include "../Data.h"
#include "../ReadData.h"
#include "Transaction.h"

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

#define DESERIALIZE_V(SOURCE, P, TYPE, VEC) {\
    auto vsize_ = read<uint64_t>((SOURCE), (P)); \
    (VEC).clear(); \
    (P) += 8; \
    for (int __i_ = 0; __i_ < vsize_; ++__i_) { \
        TYPE ta; \
        ta.deserialize((SOURCE), (P)); \
        (P) += ta.size(); \
        (VEC).push_back(ta); \
    }\
}
void Transaction::deserialize(const Data &data, int initial_pos) {
    type = (TransactionType) data[initial_pos];
    version = data[initial_pos + 1];
    int pos = initial_pos + 2;
    DESERIALIZE_V(data, pos, TransactionAttribute, attributes);
    DESERIALIZE_V(data, pos, CoinReference, inInputs);
    DESERIALIZE_V(data, pos, TransactionOutput, outputs);
    // DESERIALIZE_V(data, pos, Witness, witnesses); // witnessess is not serialized
}
#undef DESERIALIZE_V

#define SERIALIZE_V(R, V) { \
    append((R), write<uint64_t>((V).size())); \
    for (auto &__x_: (V)) { \
        append((R), __x_.serialize()); \
    } \
}
Data Transaction::serialize() const {
    Data resp;
    resp.push_back((byte) type);
    resp.push_back(version);

    SERIALIZE_V(resp, attributes);
    SERIALIZE_V(resp, inInputs);
    SERIALIZE_V(resp, outputs);
    // SERIALIZE_V(resp, witnesses); // witnessess is not serialized

    return resp;
}
#undef SERIALIZE_V

Data Transaction::getHash() const {
    return Hash::sha256(Hash::sha256(serialize()));
}

uint256_t Transaction::getHashUInt256() const {
    return load<uint256_t>(getHash());
}
