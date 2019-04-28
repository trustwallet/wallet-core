#pragma once

#include <vector>

#include "ISerializable.hpp"
#include "../uint256.h"
#include "../Data.h"

namespace TW::NEO {
    class CoinReference : public ISerializable {
    public:
        uint256_t prevHash;
        ushort prevIndex;

        int64_t size() const {
            return prevHash.precision() + sizeof(prevIndex);
        }

        void deserialize(const Data &data) {
            prevHash = load(data);
            prevIndex = readUInt16(data, size() - sizeof(prevIndex));
        }

        Data serialize() const {
            return concat(store(prevHash), write(prevIndex));
        }
    };
}