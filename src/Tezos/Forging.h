// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Michelson.h"
#include "uint256.h"
#include "../PublicKey.h"
#include "../proto/Tezos.pb.h"

#include <string>
#include <variant>
#include <vector>

using namespace TW;

namespace TW::Tezos {

Data forgeBool(bool input);
Data forgeOperation(const Proto::Operation& operation);
Data forgeAddress(const std::string& address);
Data forgeArray(const Data& data);
Data forgePublicKeyHash(const std::string& publicKeyHash);
Data forgePrefix(std::array<TW::byte, 3> prefix, const std::string& val);
Data forgePublicKey(PublicKey publicKey);
Data forgeZarith(uint64_t input);
Data forgeInt32(int value, int len = 4);
Data forgeString(const std::string& value, std::size_t len = 4);
Data forgeEntrypoint(const std::string& value);
Data forgeMichelson(const MichelsonValue::MichelsonVariant& value);
Data forgeMichelInt(const TW::int256_t& value);
Data forgePrim(const PrimValue& value);

} // namespace TW::Tezos
