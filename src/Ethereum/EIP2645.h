// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <string>

namespace TW::Ethereum {

std::string accountPathFromAddress(const std::string& ethAddress, const std::string& layer, const std::string& application, const std::string& index) noexcept;;

} // namespace TW::Ethereum
