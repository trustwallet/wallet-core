#pragma once

#include <array>
#include <memory>
#include <vector>

#include "Cell.h"

namespace TW::Everscale {

class BuilderData {
public:
    std::vector<std::uint8_t> data{};
    std::vector<std::shared_ptr<Cell>> references{};

    std::size_t lengthInBits = 0;

    BuilderData(): lengthInBits(0) {
        data.reserve(128);
        references.reserve(4);
    }

    void appendBitZero();
    void appendBitOne();
    void appendU32(uint32_t value);
    void appendRaw(const Data& data, std::size_t bits);
    void appendReferenceCell(std::shared_ptr<Cell> child);

    Cell intoCell();

private:
    inline size_t bitsCapacity() { return 1023; }

    void appendWithoutShifting(const Data& data, std::size_t bits);
    void appendWithSliceShifting(const Data& data, std::size_t bits);
    void appendWithDoubleShifting(const Data& data, std::size_t bits);

    static void appendTag(Data& appendedData, size_t bits);
};

} // namespace TW::Everscale
