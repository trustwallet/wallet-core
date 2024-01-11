// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Data.h"
#include "../PublicKey.h"
#include "../PrivateKey.h"

#include <string>

namespace TW::Tezos {

PublicKey parsePublicKey(const std::string& publicKey);
PrivateKey parsePrivateKey(const std::string& privateKey);
std::string base58ToHex(const std::string& data, size_t prefixLength);

} // namespace TW::Tezos
