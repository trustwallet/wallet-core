#pragma once

#include <array>
#include <memory>
#include <vector>

#include "Cell.h"

namespace TW::Everscale {

class CellSlice {
public:
    const Cell* _Nonnull cell;
    uint16_t dataOffset = 0;
    uint8_t refsOffset = 0;

    explicit CellSlice(const Cell* _Nonnull cell) noexcept
        : cell(cell), dataOffset(cell->bitLen), refsOffset(cell->refCount) {}

    /// Checks and increases dataOffset
    void advance(uint16_t bits);
    /// Tries to read next bit
    bool getNextBit();
    /// Tries to read next bits (at most 8)
    uint8_t getNextBits(uint16_t bits);
    uint32_t getNextU32();
    Data getNextBytes(uint8_t bytes);
    const Cell* _Nonnull getNextReferenceCell();

private:
    void require(uint16_t bits) const;
};

} // namespace TW::Everscale
