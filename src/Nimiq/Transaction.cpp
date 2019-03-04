// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Transaction.h"
#include "BinaryCoding.h"
#include "../HexCoding.h"

using namespace TW;
using namespace TW::Nimiq;

const uint8_t NETWORK_ID = 42;

std::vector<uint8_t> Transaction::serialize() const {
    std::vector<uint8_t> data;
    encode64(amount, data);
    encode64(fee, data);
    return data;
}
