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
    auto resp = sizeof(type) + sizeof(version);
    SIZE_INC(resp, attributes);
    SIZE_INC(resp, inInputs);
    SIZE_INC(resp, outputs);
//    SIZE_INC(resp, witnesses);

    return resp;
}
#undef SIZE_INC

#define DESERIALIZE_V(P, TYPE, VEC) {\
    auto vsize_ = readUInt64(data, 5); \
    (VEC) = std::vector<TYPE>(vsize_); \
    (P) += 8; \
    for (int __i_ = 0; __i_ < vsize_; ++__i_) { \
        TYPE ta; \
        ta.deserialize(data, pos); \
        pos += ta.size(); \
        (VEC).push_back(ta); \
    }\
}
void Transaction::deserialize(const Data &data, int initial_pos) {
    TransactionType txType = (TransactionType) readUInt16(data, initial_pos);
    if (txType != type) {
        throw std::invalid_argument("Transaction::DeserializeUnsigned FormatException");
    }

    version = data[initial_pos + 4];
    int pos = initial_pos + 5;
    DESERIALIZE_V(pos, TransactionAttribute, attributes);
    DESERIALIZE_V(pos, CoinReference, inInputs);
    DESERIALIZE_V(pos, TransactionOutput, outputs);
    DESERIALIZE_V(pos, Witness, witnesses);
}
#undef DESERIALIZE_V

#define SERIALIZE_V(R, V) { \
    append((R), writeUlong((V).size())); \
    for (auto &__x_: (V)) { \
        append((R), __x_.serialize()); \
    } \
}
Data Transaction::serialize() const {
    Data resp(size());
    append(resp, writeInt(type));
    resp.push_back(version);

    SERIALIZE_V(resp, attributes);
    SERIALIZE_V(resp, inInputs);
    SERIALIZE_V(resp, outputs);
    SERIALIZE_V(resp, witnesses);

    return resp;
}
#undef SERIALIZE_V

Data Transaction::getHash() const {
    return Hash::sha256(Hash::sha256(serialize()));
}
