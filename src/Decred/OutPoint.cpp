// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "OutPoint.h"

#include "../BinaryCoding.h"

namespace TW::Decred {

void OutPoint::encode(Data& data) const {
    std::copy(std::begin(hash), std::end(hash), std::back_inserter(data));
    encode32LE(index, data);
    data.push_back(static_cast<byte>(tree));
}

} // namespace TW::Decred
