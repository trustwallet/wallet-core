#pragma once

#include "../uint256.h"
#include "../UInt.hpp"
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
            return 32 + 8 + 20;
        }

        void deserialize(const Data &data, int initial_pos = 0) override {
            assetId = load<uint256_t>(readBytes(data, 32, initial_pos));
            value = readInt64(data, initial_pos + 32);
            scriptHash = load<uint160_t>(readBytes(data, 20, initial_pos + 32 + 8));
        }

        Data serialize() const override {
            return concat(concat(store(assetId), writeLong(value)), store<uint160_t>(scriptHash));
        }
    };
}