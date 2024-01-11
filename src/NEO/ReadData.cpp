// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Data.h"
#include "ReadData.h"
#include <stdexcept>


TW::Data TW::readBytes(const TW::Data& from, size_t max, size_t initial_pos) {
    if (from.size() < initial_pos + max) {
        throw std::invalid_argument("Data::Cannot read enough bytes!");
    }
    return TW::subData(from, initial_pos, max);
}

TW::Data TW::readVarBytes(const Data& from, size_t initial_pos, uint32_t* dataRead) {
    uint64_t size = readVar<uint64_t>(from, initial_pos);
    auto shift = varIntSize(size);
    if (dataRead) {
        *dataRead = uint32_t(shift + size);
    }
    return readBytes(from, int(size), initial_pos + static_cast<size_t>(shift));
}

template<> uint64_t TW::readVar(const TW::Data& from, size_t initial_pos, const uint64_t &max) {
    byte fb = from[initial_pos];
    uint64_t value;
    if (fb == 0xFD) {
        value = decode16LE(from.data() + initial_pos + 1);
    } else if (fb == 0xFE) {
        value = decode32LE(from.data() + initial_pos + 1);
    } else if (fb == 0xFF) {
        value = decode64LE(from.data() + initial_pos + 1);
    } else {
        value = fb;
    }
    if (value > max) {
        // std::cout << "TOO HUGE VALUE: " << value << " max=" << max << std::endl;
        throw std::invalid_argument("ReadData::ReadVarInt error: Too huge value! FormatException");
    }
    return value;
}

template<> int64_t TW::readVar(const TW::Data& from, size_t initial_pos, const int64_t &max) {
    return (int64_t) readVar<uint64_t>(from, initial_pos, uint64_t(max));
}

TW::Data TW::writeVarBytes(const Data& from, size_t initial_pos) {
    Data resp;
    encodeVarInt(uint64_t(from.size() - initial_pos), resp);
    resp.insert(resp.end(), from.begin() + initial_pos, from.end());
    return resp;
}
