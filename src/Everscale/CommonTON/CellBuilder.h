// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <array>
#include <memory>
#include <vector>

#include "Cell.h"
#include "CellSlice.h"
#include "RawAddress.h"

namespace TW::CommonTON {

class CellBuilder {
    uint16_t bitLen = 0;
    std::vector<std::uint8_t> data{};
    std::vector<Cell::Ref> references{};

public:
    using uint128_t = boost::multiprecision::uint128_t;

    CellBuilder() = default;
    CellBuilder(Data& appendedData, uint16_t bits);

    void appendBitZero();
    void appendBitOne();
    void appendU8(uint8_t value);
    void appendBitBool(bool bit);
    void appendU32(uint32_t value);
    void appendU64(uint64_t value);
    void appendU128(const uint128_t& value);
    void appendI8(int8_t value);
    void appendBits(uint64_t value, uint8_t bits);
    void appendRaw(const Data& appendedData, uint16_t bits);
    void prependRaw(Data& appendedData, uint16_t bits);
    void appendReferenceCell(Cell::Ref child);
    void appendBuilder(const CellBuilder& builder);
    void appendCellSlice(const CellSlice& other);
    void appendAddress(const AddressData& addressData);

    Cell::Ref intoCell();

private:
    void appendWithoutShifting(const Data& data, uint16_t bits);
    void appendWithSliceShifting(const Data& data, uint16_t bits);
    void appendWithDoubleShifting(const Data& data, uint16_t bits);

    static uint8_t clzU128(const uint128_t& u);
    static void encode128BE(const uint128_t& value, Data& data);
};

} // namespace TW::CommonTON
