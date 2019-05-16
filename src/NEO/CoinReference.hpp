#pragma once

#include <vector>

#include "ISerializable.h"
#include "../uint256.h"
#include "../UInt.hpp"
#include "../Data.h"
#include "../ReadData.h"

namespace TW::NEO {
    class CoinReference : public ISerializable {
    public:
        uint256_t prevHash;
        ushort prevIndex;

        virtual ~CoinReference() {}

        int64_t size() const override {
            return 32 + 2; // sizeof(prevHash) + sizeof(prevIndex);
        }

        void deserialize(const Data &data, int initial_pos = 0) override {
            prevHash = load<uint256_t>(readBytes(data, 32, initial_pos));
            prevIndex = read<uint16_t>(data, initial_pos + 32);
        }

        Data serialize() const override {
            return concat(store(prevHash), write<uint16_t>(prevIndex));
        }

        bool operator==(const CoinReference &other) const {
            return this->prevHash == other.prevHash
                && this->prevIndex == other.prevIndex;
        }
    };
}