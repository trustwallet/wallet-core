// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Data.h"

namespace TW {

Data subData(const Data& data, size_t index, size_t length) {
    size_t subLength = length;
    if (index + subLength > data.size()) { subLength = data.size() - index; } // guard against over-length
    return TW::data(data.data() + index, subLength);
}

} // namespace TW
