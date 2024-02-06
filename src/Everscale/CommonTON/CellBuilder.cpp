// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "CellBuilder.h"
#include "Cell.h"

#include <bit>
#include <cassert>
#include <vector>

#include "BinaryCoding.h"

using namespace TW;

namespace TW::CommonTON {

CellBuilder::CellBuilder(Data& appendedData, uint16_t bits) {
    assert(bits <= appendedData.size() * 8);
    assert(bits < Cell::MAX_BITS);
    assert(bits % 8 == 0);

    appendedData.resize(bits / 8);

    data = appendedData;
    bitLen = bits;
    references = {};
}

void CellBuilder::appendBitZero() {
    Data appendedData{0x00};
    appendRaw(appendedData, 1);
}

void CellBuilder::appendBitOne() {
    Data appendedData{0xFF};
    appendRaw(appendedData, 1);
}

void CellBuilder::appendBitBool(bool bit) {
    auto getAppendedData = [](bool bit) {
        if (bit) {
            return Data{0xFF};
        } else {
            return Data{0x00};
        }
    };
    auto appendedData = getAppendedData(bit);
    appendRaw(appendedData, 1);
}

void CellBuilder::appendU8(uint8_t value) {
    Data appendedData{value};
    appendRaw(appendedData, 8);
}

void CellBuilder::appendU32(uint32_t value) {
    Data appendedData;
    encode32BE(value, appendedData);
    appendRaw(appendedData, 32);
}

void CellBuilder::appendU64(uint64_t value) {
    Data appendedData;
    encode64BE(value, appendedData);
    appendRaw(appendedData, 64);
}

void CellBuilder::appendU128(const uint128_t& value) {
    uint8_t bits = 4;
    uint16_t bytes = 16 - clzU128(value) / 8;

    appendBits(bytes, bits);

    Data encodedValue;
    encode128BE(value, encodedValue);

    auto offset = static_cast<uint32_t>(encodedValue.size() - bytes);

    Data appendedData(encodedValue.begin() + offset, encodedValue.end());
    appendRaw(appendedData, bytes * 8);
}

void CellBuilder::appendI8(int8_t value) {
    Data appendedData{static_cast<uint8_t>(value)};
    appendRaw(appendedData, 8);
}

void CellBuilder::appendBits(uint64_t value, uint8_t bits) {
    assert(bits >= 1 && bits <= 7);

    Data appendedData;

    auto val = static_cast<uint8_t>(value << (8 - bits));
    appendedData.push_back(val);

    appendRaw(appendedData, bits);
}

void CellBuilder::appendRaw(const Data& appendedData, uint16_t bits) {
    if (appendedData.size() * 8 < bits) {
        throw std::invalid_argument("invalid builder data");
    } else if (bitLen + bits > Cell::MAX_BITS) {
        throw std::runtime_error("cell data overflow");
    } else if (bits != 0) {
        if ((bitLen % 8) == 0) {
            if ((bits % 8) == 0) {
                appendWithoutShifting(appendedData, bits);
            } else {
                appendWithSliceShifting(appendedData, bits);
            }
        } else {
            appendWithDoubleShifting(appendedData, bits);
        }
    }
    assert(bitLen <= Cell::MAX_BITS);
    assert(data.size() * 8 <= Cell::MAX_BITS + 1);
}

void CellBuilder::prependRaw(Data& appendedData, uint16_t bits) {
    if (bits != 0) {
        auto buffer = CellBuilder(appendedData, bits);
        buffer.appendRaw(data, bitLen);

        data = buffer.data;
        bitLen = buffer.bitLen;
    }
}

void CellBuilder::appendReferenceCell(std::shared_ptr<Cell> child) {
    if (child) {
        if (references.size() + 1 > Cell::MAX_REFS) {
            throw std::runtime_error("cell refs overflow");
        }
        references.emplace_back(std::move(child));
    }
}

void CellBuilder::appendBuilder(const CellBuilder& builder) {
    appendRaw(builder.data, builder.bitLen);
    for (const auto& reference : builder.references) {
        appendReferenceCell(reference);
    }
}

void CellBuilder::appendCellSlice(const CellSlice& other) {
    Data appendedData(other.cell->data);
    appendRaw(appendedData, other.cell->bitLen);

    for (const auto& cell : other.cell->references) {
        appendReferenceCell(cell);
    }
}

Cell::Ref CellBuilder::intoCell() {
    // Append tag
    if (bitLen & 7) {
        const auto mask = static_cast<uint8_t>(0x80 >> (bitLen & 7));
        const auto l = bitLen / 8;
        data[l] = static_cast<uint8_t>((data[l] & ~mask) | mask);
    }

    auto refCount = references.size();

    Cell::Refs refs;
    std::move(references.begin(), references.end(), refs.begin());

    auto cell = std::make_shared<Cell>(bitLen, std::move(data), refCount, std::move(refs));
    cell->finalize();

    bitLen = 0;
    refCount = 0;

    return cell;
}

void CellBuilder::appendWithoutShifting(const Data& appendedData, uint16_t bits) {
    assert(bits % 8 == 0);
    assert(bitLen % 8 == 0);

    data.resize(bitLen / 8);
    data.insert(data.end(), appendedData.begin(), appendedData.end());
    bitLen += bits;
    data.resize(bitLen / 8);
}

void CellBuilder::appendWithSliceShifting(const Data& appendedData, uint16_t bits) {
    assert(bits % 8 != 0);
    assert(bitLen % 8 == 0);

    data.resize(bitLen / 8);
    data.insert(data.end(), appendedData.begin(), appendedData.end());
    bitLen += bits;
    data.resize(1 + bitLen / 8);

    data.back() &= ~static_cast<uint8_t>(0xff >> (bits % 8));
}

void CellBuilder::appendWithDoubleShifting(const Data& appendedData, uint16_t bits) {
    auto selfShift = bitLen % 8;
    data.resize(1 + bitLen / 8);
    bitLen += bits;

    // yyyyy000 -> 00000000 000yyyyy
    auto y = static_cast<uint16_t>(data.back() >> (8 - selfShift));
    data.pop_back();

    for (const auto x : appendedData) {
        // 00000000 000yyyyy -> 000yyyyy xxxxxxxx
        y = static_cast<uint16_t>(y << 8) | x;
        // 000yyyyy xxxxxxxx -> 00000000 yyyyyxxx
        data.push_back(static_cast<uint8_t>(y >> selfShift));
    }
    // 00000000 yyyyyxxx
    data.push_back(static_cast<uint8_t>(y << (8 - selfShift)));

    auto shift = bitLen % 8;
    if (shift == 0) {
        data.resize(bitLen / 8);
    } else {
        data.resize(bitLen / 8 + 1);
        data.back() &= ~static_cast<uint8_t>(0xff >> (bitLen % 8));
    }
}

void CellBuilder::appendAddress(const AddressData& addressData) {
    Data rawData(addressData.hash.begin(), addressData.hash.end());
    Data prefix{0x80};
    appendRaw(prefix, 2);
    appendBitZero();
    appendI8(addressData.workchainId);
    appendRaw(rawData, 256);
}

uint8_t CellBuilder::clzU128(const uint128_t& u) {
    auto hi = static_cast<uint64_t>(u >> 64);
    auto lo = static_cast<uint64_t>(u);

    if (lo == 0 && hi == 0) {
        return 128;
    } else if (hi == 0) {
        return static_cast<uint8_t>(std::countl_zero(lo) + 64);
    } else {
        return static_cast<uint8_t>(std::countl_zero(hi));
    }
}

void CellBuilder::encode128BE(const uint128_t& val, Data& data) {
    data.emplace_back(static_cast<uint8_t>((val >> 120)));
    data.emplace_back(static_cast<uint8_t>((val >> 112)));
    data.emplace_back(static_cast<uint8_t>((val >> 104)));
    data.emplace_back(static_cast<uint8_t>((val >> 96)));
    data.emplace_back(static_cast<uint8_t>((val >> 88)));
    data.emplace_back(static_cast<uint8_t>((val >> 80)));
    data.emplace_back(static_cast<uint8_t>((val >> 72)));
    data.emplace_back(static_cast<uint8_t>((val >> 64)));
    data.emplace_back(static_cast<uint8_t>((val >> 56)));
    data.emplace_back(static_cast<uint8_t>((val >> 48)));
    data.emplace_back(static_cast<uint8_t>((val >> 40)));
    data.emplace_back(static_cast<uint8_t>((val >> 32)));
    data.emplace_back(static_cast<uint8_t>((val >> 24)));
    data.emplace_back(static_cast<uint8_t>((val >> 16)));
    data.emplace_back(static_cast<uint8_t>((val >> 8)));
    data.emplace_back(static_cast<uint8_t>(val));
}

} // namespace TW::CommonTON
