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

} // namespace TW
