#include "CellSlice.h"

#include <cassert>

#include "../BinaryCoding.h"

using namespace TW;
using namespace TW::Everscale;

void CellSlice::advance(uint16_t bits) {
    require(bits);
    dataOffset += bits;
}

bool CellSlice::getNextBit() {
    require(1);
    const uint16_t q = dataOffset / 8;
    const uint16_t r = dataOffset % 8;
    const auto bit = ((cell->data[q] >> (7 - r)) & 1) != 0;
    ++dataOffset;
    return bit;
}

uint8_t CellSlice::getNextBits(uint16_t bits) {
    if (bits == 0) {
        return 0;
    }
    assert(bits <= 8);
    require(bits);

    const size_t q = dataOffset / 8;
    const auto r = dataOffset % 8;
    const auto invR = 8 - r;

    dataOffset += bits;

    if (r == 0) {
        return cell->data[q] >> (invR - bits);
    } else if (bits <= invR) {
        return (cell->data[q] >> (invR - bits)) & ((1 << bits) - 1);
    } else {
        auto result = static_cast<uint16_t>(static_cast<uint16_t>(cell->data[q]) << 8);
        if (q + 1 < cell->data.size()) {
            result |= static_cast<uint16_t>(cell->data[q + 1]);
        }
        return static_cast<uint8_t>(result >> (invR + 8 - bits));
    }
}

uint32_t CellSlice::getNextU32() {
    const auto bytes = getNextBytes(sizeof(uint32_t));
    return decode32BE(bytes.data());
}

Data CellSlice::getNextBytes(uint8_t bytes) {
    if (bytes == 0) {
        return Data{};
    }
    require(bytes * 8);
    Data result{};
    result.reserve(bytes);

    const size_t q = dataOffset / 8;
    const auto r = dataOffset % 8;
    const auto invR = 8 - r;

    dataOffset += bytes * 8;

    if (r == 0) {
        const auto begin = cell->data.begin() + q;
        const auto end = begin + bytes;
        std::copy(begin, end, std::back_inserter(result));
        return result;
    }

    for (size_t byte = q; byte < q + bytes; ++byte) {
        auto bits = static_cast<uint16_t>(static_cast<uint16_t>(cell->data[byte]) << 8);
        if (byte + 1 < cell->data.size()) {
            bits |= static_cast<uint16_t>(cell->data[byte + 1]);
        }
        result.push_back(static_cast<uint8_t>(bits >> invR));
    }
    return result;
}

const Cell* _Nonnull CellSlice::getNextReferenceCell() {
    if (refsOffset > cell->refCount) {
        throw std::runtime_error("cell refs underflow");
    }
    const auto* child = cell->references[refsOffset++].get();
    if (child == nullptr) {
        throw std::runtime_error("invalid cell reference");
    }
    return child;
}

void CellSlice::require(uint16_t bits) const {
    if (dataOffset + bits > cell->bitLen) {
        throw std::runtime_error("cell data underflow");
    }
}
