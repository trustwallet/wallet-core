#pragma once

#include "ISerializable.h"
#include "../uint256.h"
#include "../uint160.hpp"
#include "../Data.h"

namespace TW::NEO {
    class TransactionOutput : public ISerializable {
    public:
        uint256_t assetId;
        int64_t value;
        uint160_t scriptHash;

        int64_t size() const override {
            //return assetId.precision() + sizeof(value) + scriptHash.precision();
            return sizeof(value);
        }

        void deserialize(const Data &data) override {
            assetId = load(data);
            value = readInt64(data, assetId.precision());
            scriptHash = load160(data, assetId.precision() + sizeof(value));
        }

        Data serialize() const override {
            return concat(concat(store(assetId), writeLong(value)), storeUint160(scriptHash));
        }
    };
}