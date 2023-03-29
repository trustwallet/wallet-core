// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <string>

namespace TW::Ethereum {

std::string accountPathFromAddress(const std::string& ethAddress, const std::string& layer, const std::string& application, const std::string& index) noexcept;;

} // namespace TW::Ethereum
