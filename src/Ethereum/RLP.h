// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "proto/EthereumRLP.pb.h"
#include "uint256.h"

#include <cstdint>
#include <string>
#include <vector>

namespace TW::Ethereum {

/// Implementation of Ethereum's RLP encoding.
///
/// - SeeAlso: https://github.com/ethereum/wiki/wiki/RLP
struct RLP {
    static Data encode(const EthereumRLP::Proto::EncodingInput& input);

    static Data encodeData(const Data& data);

    static Data encodeString(const std::string& s);

    static Data encodeU256(const uint256_t& num);

    struct DecodedItem {
        std::vector<Data> decoded;
        Data remainder;
    };

    static DecodedItem decodeList(const Data& input);
    /// Decodes data, remainder from RLP encoded data
    static DecodedItem decode(const Data& data);

    /// Parses an integer of given size, between 1 and 8 bytes, big endian
    static uint64_t parseVarInt(size_t size, const Data& data, size_t index);
};

} // namespace TW::Ethereum
