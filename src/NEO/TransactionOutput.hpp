#pragma once

#include "../uint256.h"
#include "../uint160.hpp"
#include "../Data.h"
#include "../ReadData.h"
#include "ISerializable.h"

namespace TW::NEO {
    class TransactionOutput : public ISerializable {
    public:
        uint256_t assetId;
        int64_t value;
        uint160_t scriptHash;

        virtual ~TransactionOutput() {}

        int64_t size() const override {
            return sizeof(assetId) + sizeof(value) + sizeof(scriptHash);
        }

        void deserialize(const Data &data, int initial_pos = 0) override {
            assetId = load(data, initial_pos);
            value = readInt64(data, initial_pos + sizeof(assetId));
            scriptHash = load160(data, initial_pos + sizeof(assetId) + sizeof(value));
        }

        Data serialize() const override {
            return concat(concat(store(assetId), writeLong(value)), storeUint160(scriptHash));
        }
    };
}