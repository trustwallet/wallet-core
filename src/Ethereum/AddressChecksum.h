// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Address.h"
#include <string>

namespace TW::Ethereum {

std::string checksumed(const Address& address);

} // namespace TW::Ethereum
