#pragma once

#include <array>
#include <memory>
#include <vector>

#include <boost/multiprecision/cpp_int.hpp>

#include "../Data.h"
#include "../Hash.h"

namespace TW::Everscale {

class Cell {
public:
    using Ref = std::shared_ptr<Cell>;
    using Refs = std::array<Ref, 4>;

    bool finalized = false;
    uint16_t bitLen = 0;
    std::vector<uint8_t> data{};
    uint8_t refCount = 0;
    Refs references{};

    uint16_t depth = 0;
    std::array<uint8_t, Hash::sha256Size> hash{};

    Cell() = default;

    Cell(uint16_t bitLen, std::vector<uint8_t> data, uint8_t refCount, Refs references)
        : bitLen(bitLen), data(std::move(data)), refCount(refCount), references(std::move(references)) {}

    // Deserialize from BOC representation
    static std::shared_ptr<Cell> deserialize(const uint8_t* _Nonnull data, size_t len);

    // Serialize to binary stream
    void serialize(Data& os) const;

    // Compute cell depth and hash
    void finalize();

    inline std::pair<uint8_t, uint8_t> getDescriptorBytes() const noexcept {
        const uint8_t d1 = refCount;
        const uint8_t d2 = (static_cast<uint8_t>(bitLen >> 2) & 0b11111110) | (bitLen % 8 != 0);
        return std::pair<uint8_t, uint8_t>{d1, d2};
    }

    inline size_t serializedSize() const noexcept {
        return 2 + (bitLen + 7) / 8 + refCount * (2 + Hash::sha256Size);
    }

    static uint16_t findTag(Data& bitstring);
    static void appendTag(Data& appendedData, size_t bits);
};

} // namespace TW::Everscale
