#pragma once

#include <array>
#include <memory>
#include <vector>

#include "Cell.h"
#include "CellSlice.h"

namespace TW::Everscale {

class CellBuilder {
    uint16_t bitLen = 0;
    std::vector<std::uint8_t> data{};
    std::vector<Cell::Ref> references{};
public:
    using uint128_t = boost::multiprecision::uint128_t;

    CellBuilder() = default;
    CellBuilder(Data& appendedData, std::size_t bits);

    void appendBitZero();
    void appendBitOne();
    void appendU8(uint8_t value);
    void appendBitBool(bool bit);
    void appendU32(uint32_t value);
    void appendU64(uint64_t value);
    void appendU128(uint128_t value);
    void appendi8(int8_t value);
    void appendRaw(const Data& data, std::size_t bits);
    void prependRaw(Data& appendedData, std::size_t bits);
    void appendReferenceCell(Cell::Ref child);
    void appendCellSlice(const CellSlice &other);

    Cell::Ref intoCell();

private:
    void appendWithoutShifting(const Data& data, uint16_t bits);
    void appendWithSliceShifting(const Data& data, uint16_t  bits);
    void appendWithDoubleShifting(const Data& data, uint16_t bits);

    void encode128BE(uint128_t value, Data& data);
};

} // namespace TW::Everscale
