#pragma once

#include "ISerializable.hpp"
#include "../uint256.h"
#include "../uint160.h"
#include "../Data.h"

namespace TW::NEO {
    class TransactionOutput : public ISerializable {
    public:
        uint256_t assetId;
        int64_t value;
        uint160_t scriptHash;

        int64_t size() const {
            return assetId.precision() + sizeof(value) + scriptHash.precision();
        }

        void deserialize(const Data &data) {
            assetId = load(data);
            value = readInt64(data, assetId.precision());
            scriptHash = load160(data, assetId.precision() + sizeof(value));
        }

        Data serialize() const {
            return concat(concat(store(assetId), write(value)), store(scriptHash));
        }
    };
}