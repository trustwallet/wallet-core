#pragma once

#include <array>
#include <memory>
#include <vector>

#include "Cell.h"

namespace TW::Everscale {

class CellBuilder {
public:
    uint16_t bitLen = 0;
    std::vector<std::uint8_t> data{};
    std::vector<Cell::Ref> references{};

    void appendBitZero();
    void appendBitOne();
    void appendU32(uint32_t value);
    void appendRaw(const Data& data, std::size_t bits);
    void appendReferenceCell(Cell::Ref child);

    Cell::Ref intoCell();

private:
    void appendWithoutShifting(const Data& data, uint16_t bits);
    void appendWithSliceShifting(const Data& data, uint16_t  bits);
    void appendWithDoubleShifting(const Data& data, uint16_t bits);
};

} // namespace TW::Everscale
