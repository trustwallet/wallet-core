// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Address.h"
#include "uint256.h"

namespace TW::Elrond {

/// A stripped-down variant of the Elrond codec.
/// For reference, see:
/// - https://docs.elrond.com/developers/developer-reference/elrond-serialization-format
/// - https://github.com/ElrondNetwork/elrond-sdk-erdjs/tree/main/src/smartcontracts/codec
/// - https://github.com/ElrondNetwork/elrond-wasm-rs/tree/master/elrond-codec
class Codec {
public:
    static std::string encodeString(const std::string& value);
    static std::string encodeUint64(uint64_t value);
    static std::string encodeBigInt(const std::string& value);
    static std::string encodeBigInt(TW::uint256_t value);
    static std::string encodeAddress(const std::string& bech32Address);
    static std::string encodeAddress(const Address& address);
};

} // namespace
