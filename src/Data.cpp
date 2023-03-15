// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Data.h"

namespace TW {

Data subData(const Data& data, size_t startIndex, size_t length) {
    if (startIndex >= data.size()) {
        return Data();
    }
    const size_t subLength = std::min(length, data.size() - startIndex); // guard against over-length
    return TW::data(data.data() + startIndex, subLength);
}

Data subData(const Data& data, size_t startIndex) {
    if (startIndex >= data.size()) {
        return Data();
    }
    const size_t subLength = data.size() - startIndex;
    return TW::data(data.data() + startIndex, subLength);
}

} // namespace TW
