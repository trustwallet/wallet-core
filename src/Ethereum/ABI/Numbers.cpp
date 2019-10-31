// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Numbers.h"

namespace TW::Ethereum {

void encode(uint256_t value, Data& data) {
    Data bytes = store(value);

    append(data, Data(encodedIntSize - bytes.size()));
    append(data, bytes);
}

bool decode(const Data& encoded, uint256_t& decoded, size_t& offset_inout) {
    decoded = 0u;
    if (encoded.empty() || (encoded.size() < (encodedIntSize + offset_inout))) {
        return false;
    }
    decoded = loadWithOffset(encoded, offset_inout);
    offset_inout += encodedIntSize;
    return true;
}

void encode(bool v, Data& data) {
    append(data, Data(encodedIntSize - 1));
    data.push_back(v ? 1 : 0);
}

bool decode(const Data& encoded, uint8_t& decoded, size_t& offset_inout) {
    decoded = 0u;
    if (encoded.empty() || (encoded.size() < (offset_inout + 1))) {
        return false;
    }
    decoded = encoded[offset_inout];
    offset_inout += 1;
    return true;
}

} // namespace TW::Ethereum
