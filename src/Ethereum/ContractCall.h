// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Data.h"
#include <nlohmann/json.hpp>
#include <optional>
#include <string>

namespace TW::Ethereum::ABI {
    std::optional<std::string> decodeCall(const Data& call, const std::string& abi);
} // namespace TW::Ethereum::ABI
