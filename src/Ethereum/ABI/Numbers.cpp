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

void encode(bool v, Data& data) {
    append(data, Data(encodedIntSize - 1));
    data.push_back(v ? 1 : 0);
}

} // namespace TW::Ethereum
