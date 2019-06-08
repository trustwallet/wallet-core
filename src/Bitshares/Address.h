// Copyright � 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Bravo/Address.h"

namespace TW::Bitshares {
const std::string AddressPrefix = "BTS";
}

// Bitshares address class is similar to Bravo. 
// Just creating TWBitsharesAddress struct for C interface 
struct TWBitsharesAddress {
    TW::Bravo::Address impl;
};