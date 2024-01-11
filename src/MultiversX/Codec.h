// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Address.h"
#include "uint256.h"

namespace TW::MultiversX {

/// A stripped-down variant of the MultiversX codec.
/// For reference, see:
/// - https://docs.multiversx.com/developers/developer-reference/overview
/// - https://github.com/multiversx/mx-sdk-erdjs/tree/main/src/smartcontracts/codec
/// - https://github.com/multiversx/mx-sdk-rs/tree/master/framework/codec
class Codec {
public:
    static std::string encodeString(const std::string& value);
    static std::string encodeUint64(uint64_t value);
    static std::string encodeBigInt(const std::string& value);
    static std::string encodeBigInt(TW::uint256_t value);
    static std::string encodeAddress(const std::string& bech32Address);
    static std::string encodeAddress(const Address& address);
};

} // namespace TW::MultiversX
