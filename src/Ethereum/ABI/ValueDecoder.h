// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <Data.h>
#include <uint256.h>

namespace TW::Ethereum::ABI {

class ValueDecoder {
public:
    static uint256_t decodeUInt256(Data& data);
};
} // namespace TW::Ethereum::ABI
