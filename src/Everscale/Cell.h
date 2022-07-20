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
    bool finalized = false;
    uint8_t refCount = 0;
    uint16_t bitLen = 0;
    std::vector<uint8_t> data{};
    std::array<std::shared_ptr<Cell>, 4> references{};

    uint16_t depth = 0;
    std::array<uint8_t, Hash::sha256Size> hash{};

    Cell(uint16_t bitLen, std::vector<uint8_t> data)
        : bitLen(bitLen), data(std::move(data)) {}

    // Serialize to binary stream
    void serialize(Data& os) const;

    // Compute cell depth and hash
    void finalize();

    inline std::pair<uint8_t, uint8_t> getDescriptorBytes() const noexcept {
        const uint8_t d1 = refCount;
        const uint8_t d2 = static_cast<uint8_t>(bitLen >> 2) & ~uint8_t(1) + (bitLen % 8 != 0);
        return std::pair<uint8_t, uint8_t>{d1, d2};
    }

    inline size_t serializedSize() const noexcept {
        return 2 + (bitLen + 7) / 8 + refCount * (2 + Hash::sha256Size);
    }
};

} // namespace TW::Everscale
