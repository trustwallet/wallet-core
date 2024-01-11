// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Data.h"

namespace TW::Ethereum {

Data getEIP1967ProxyInitCode(const std::string& logicAddress, const Data& data);

}
