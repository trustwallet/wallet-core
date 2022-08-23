// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../PublicKey.h"
#include "../proto/Tezos.pb.h"

#include <string>

using namespace TW;
using namespace TW::Tezos::Proto;

enum PrimType : std::uint8_t {
    Pair = 7,
};

Data forgeBool(bool input);
Data forgeOperation(const Operation& operation);
Data forgeAddress(const std::string& address);
Data forgePublicKeyHash(const std::string& publicKeyHash);
Data forgePublicKey(PublicKey publicKey);
Data forgeZarith(uint64_t input);
Data forgeInt32(int value, int len = 4);
Data forgeString(const std::string& value, std::size_t len = 4);
Data forgeEntrypoint(const std::string& value);
