// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"

namespace TW::Base64 {

// Decode a Base64-format string
Data decode(const std::string& val);

// Encode bytes into Base64 string
std::string encode(const Data& val);

// Decode a Base64Url-format or a regular Base64 string.
// Base64Url format uses '-' and '_' as the two special characters, Base64 uses '+'and '/'.
Data decodeBase64Url(const std::string& val);

// Encode bytes into Base64Url string (uses '-' and '_' as pecial characters)
std::string encodeBase64Url(const Data& val);

} // namespace TW::Base64
