#pragma once

#include <vector>

#include "ISerializable.h"
#include "../uint256.h"
#include "../Data.h"
#include "../ReadData.h"

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
            prevHash = load<uint256_t>(readBytes(data, 32, initial_pos));
            prevIndex = readUInt16(data, initial_pos + 32);
        }

        Data serialize() const override {
            return concat(store(prevHash), writeUshort(prevIndex));
        }
    };
}