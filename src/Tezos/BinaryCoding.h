// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
