// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "OutPoint.h"

#include "../BinaryCoding.h"

namespace TW::Bitcoin {

void OutPoint::encode(Data& data) const noexcept {
    std::copy(std::begin(hash), std::end(hash), std::back_inserter(data));
    encode32LE(index, data);
    // sequence is encoded in TransactionInputs
    // tree is only for DCR
}

} // namespace TW::Bitcoin

