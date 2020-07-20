// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include <string>
#include <optional>
#include <nlohmann/json.hpp>

namespace TW::Ethereum {
    std::optional<std::string> decodeCall(const Data& call, const nlohmann::json& abi);
} // namespace TW::Ethereum
