// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../PublicKey.h"
#include "../proto/Tezos.pb.h"

#include <string>

using namespace TW;

namespace TW::Tezos {

Data forgeBool(bool input);
Data forgeOperation(const Proto::Operation& operation);
Data forgePublicKeyHash(const std::string& publicKeyHash);
Data forgePublicKey(PublicKey publicKey);
Data forgeZarith(uint64_t input);

} // namespace TW::Tezos::Proto
