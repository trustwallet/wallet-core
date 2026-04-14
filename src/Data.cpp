// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Data.h"

#include <stdexcept>

namespace TW {

Data subData(const Data& data, size_t startIndex, size_t length) {
    if (startIndex >= data.size()) {
        throw std::invalid_argument("invalid subData arguments");
    }
    const size_t subLength = std::min(length, data.size() - startIndex); // guard against over-length
    return TW::data(data.data() + startIndex, subLength);
}

Data subData(const Data& data, size_t startIndex) {
    if (startIndex >= data.size()) {
        throw std::invalid_argument("invalid subData arguments");
    }
    const size_t subLength = data.size() - startIndex;
    return TW::data(data.data() + startIndex, subLength);
}

bool isEqualConstantTime(const Data& in_a, const Data& in_b) {
    if (in_a.size() != in_b.size()) {
        return false;
    }

    const volatile unsigned char *a = in_a.data();
    const volatile unsigned char *b = in_b.data();
    unsigned char result = 0;

    for (size_t i = 0; i < in_a.size(); i++) {
        result |= a[i] ^ b[i];
    }
    return result == 0;
}

} // namespace TW
