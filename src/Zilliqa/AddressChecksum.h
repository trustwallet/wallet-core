// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Data.h"
#include <string>

namespace TW::Zilliqa {

std::string checksum(const Data& address);

} // namespace TW::Zilliqa
