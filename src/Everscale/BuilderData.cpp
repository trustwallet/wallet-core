#include "BuilderData.h"
#include "Cell.h"

#include <vector>

#include "../BinaryCoding.h"

using namespace TW;
using namespace TW::Everscale;

void BuilderData::appendBitZero() {
    Data appendedData {0x00};
    appendRaw(appendedData, 1);
}

void BuilderData::appendBitOne() {
    Data appendedData {0xFF};
    appendRaw(appendedData, 1);
}

void BuilderData::appendU32(uint32_t value) {
    Data appendedData;
    encode32BE(value, appendedData);
    appendRaw(appendedData, 32);
}

void BuilderData::appendRaw(const Data& appendedData, std::size_t bits) {
    if (appendedData.size() * 8 < bits) {
        throw std::runtime_error("fatal error");
    } else if (lengthInBits + bits > bitsCapacity()) {
        throw std::runtime_error("cell underflow");
    } else if (bits != 0) {
        if ((lengthInBits % 8) == 0) {
            if ((bits % 8) == 0) {
                appendWithoutShifting(appendedData, bits);
            } else {
                appendWithSliceShifting(appendedData, bits);
            }
        } else {
            appendWithDoubleShifting(appendedData, bits);
        }
    }
    assert(lengthInBits <= bitsCapacity());
    assert(data.size() * 8 <= bitsCapacity() + 1);
}

void BuilderData::appendReferenceCell(std::shared_ptr<Cell> child) {
    references.push_back(child);
}

Cell BuilderData::intoCell() {
    appendTag(data, lengthInBits);

    Cell cell; // TODO: Construct Cell from BuilderData
    cell.finalize();
    return cell;
}

void BuilderData::appendWithoutShifting(const Data& appendedData, std::size_t bits) {
    assert(bits % 8 == 0);
    assert(lengthInBits % 8 == 0);

    data.resize(lengthInBits / 8);
    data.insert(data.end(), appendedData.begin(), appendedData.end());
    lengthInBits += bits;
    data.resize(lengthInBits / 8);
}

void BuilderData::appendWithSliceShifting(const Data& appendedData, std::size_t bits) {
    assert(bits % 8 != 0);
    assert(lengthInBits % 8 == 0);

    data.resize(lengthInBits / 8);
    data.insert(data.end(), appendedData.begin(), appendedData.end());
    lengthInBits += bits;
    data.resize(1 + lengthInBits / 8);

    auto sliceShift = bits % 8;

    auto lastByte = data.back();
    data.pop_back();
    lastByte >>= 8 - sliceShift;
    lastByte <<= 8 - sliceShift;
    data.push_back(lastByte);
}

void BuilderData::appendWithDoubleShifting(const Data& appendedData, std::size_t bits) {
    auto selfShift = lengthInBits % 8;
    data.resize(1 + lengthInBits / 8);
    lengthInBits += bits;

    auto lastBits = data.back() >> (8 - selfShift);
    data.pop_back();

    auto y = (uint16_t)lastBits;
    for (auto x : appendedData) {
        y = (y << 8) | ((uint16_t(x)));
        data.push_back((uint8_t)(y >> selfShift));
    }
    data.push_back((uint8_t)(y << (8 - selfShift)));

    auto shift = lengthInBits % 8;
    if (shift  == 0) {
        data.resize(lengthInBits / 8);
    } else {
        data.resize(lengthInBits / 8 + 1);
        auto lastByte = data.back();
        data.pop_back();
        lastByte >>= 8 - shift;
        lastByte <<= 8 - shift;
        data.push_back(lastByte);
    }
}

void BuilderData::appendTag(Data& appendedData, size_t bits) {
    auto shift = bits % 8;
    if (shift == 0 || appendedData.empty()) {
        appendedData.resize(bits / 8);
        appendedData.push_back(0x80);
    } else {
        appendedData.resize(1 + bits / 8);
        auto lastByte = appendedData.back();
        appendedData.pop_back();
        if (shift != 7) {
            lastByte >>= 7 - shift;
        }
        lastByte |= 1;
        if (shift != 7) {
            lastByte <<= 7 - shift;
        }
        appendedData.push_back(lastByte);
    }
}
