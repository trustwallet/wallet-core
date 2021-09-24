// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <nlohmann/json.hpp>

namespace TW::Ethereum::Fee {

nlohmann::json suggestFee(const nlohmann::json& feeHistory);

} // namespace TW::Ethereum::Fee
