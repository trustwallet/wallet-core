// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Data.h"

namespace TW::Ethereum {

Data create2Address(const std::string& from, const Data& salt, const Data& initCodeHash);

}
