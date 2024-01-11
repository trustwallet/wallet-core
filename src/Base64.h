// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Data.h"

namespace TW::Base64 {

// Checks if as string is in Base64-format or Base64Url-format
bool isBase64orBase64Url(const std::string& val);

// Decode a Base64-format string
Data decode(const std::string& val);

// Encode bytes into Base64 string
std::string encode(const TW::Data& val);

// Decode a Base64Url-format or a regular Base64 string.
// Base64Url format uses '-' and '_' as the two special characters, Base64 uses '+'and '/'.
Data decodeBase64Url(const std::string& val);

// Encode bytes into Base64Url string (uses '-' and '_' as special characters)
std::string encodeBase64Url(const Data& val);

} // namespace TW::Base64
