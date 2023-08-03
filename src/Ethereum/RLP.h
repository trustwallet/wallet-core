// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "proto/EthereumRlp.pb.h"
#include "uint256.h"

#include <cstdint>
#include <string>
#include <vector>

namespace TW::Ethereum {

/// Implementation of Ethereum's RLP encoding.
///
/// - SeeAlso: https://github.com/ethereum/wiki/wiki/RLP
struct RLP {
    static Data encode(const EthereumRlp::Proto::EncodingInput& input);

    static Data encodeString(const std::string& s);

    static Data encodeU256(const uint256_t& num);
};

} // namespace TW::Ethereum
