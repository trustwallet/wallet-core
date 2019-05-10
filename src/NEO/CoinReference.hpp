#pragma once

#include <vector>

#include "ISerializable.h"
#include "../uint256.h"
#include "../Data.h"

namespace TW::NEO {
    class CoinReference : public ISerializable {
    public:
        uint256_t prevHash;
        ushort prevIndex;

        virtual ~CoinReference() {}

        int64_t size() const override {
            return sizeof(prevHash) + sizeof(prevIndex);
        }

        void deserialize(const Data &data, int initial_pos = 0) override {
            prevHash = load(data, initial_pos);
            prevIndex = readUInt16(data, size() - sizeof(prevIndex));
        }

        Data serialize() const override {
            return concat(store(prevHash), store(prevIndex));
        }
    };
}