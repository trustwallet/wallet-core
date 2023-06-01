// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "uint256.h"

namespace TW::Ethereum {

Data getERC4337ExecuteBytecode(const Data& toAddress, const uint256_t& value, const Data& data);

}
