// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "CellSlice.h"

#include <cassert>

#include "BinaryCoding.h"

using namespace TW;

namespace TW::CommonTON {

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

void CellSlice::require(uint16_t bits) const {
    if (dataOffset + bits > cell->bitLen) {
        throw std::runtime_error("cell data underflow");
    }
}

} // namespace TW::CommonTON
