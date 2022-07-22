#pragma once

#include <array>
#include <memory>
#include <vector>

#include "Cell.h"

namespace TW::Everscale {

class BuilderData {
public:
    std::vector<std::uint8_t> data{};
    std::array<std::shared_ptr<Cell>, 4> references{};

    std::size_t lengthInBits = 0;

    BuilderData(): lengthInBits(0) {
        data.reserve(128);
    }

    void append_u32(uint32_t value);
    void appendRaw(Data& data, std::size_t bits);

    Cell finalize(uint16_t max_depth);

private:
    inline size_t bitsCapacity() { return 1023; }

    void appendWithoutShifting(Data& data, std::size_t bits);
    void appendWithSliceShifting(Data& data, std::size_t bits);
    void appendWithDoubleShifting(Data& data, std::size_t bits);
};

} // namespace TW::Everscale
