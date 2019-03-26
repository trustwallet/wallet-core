// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "RLP.h"

#include "../Data.h"

#include <tuple>

using namespace TW;
using namespace TW::Ethereum;

Data RLP::encode(const uint256_t& value) noexcept {
    using boost::multiprecision::cpp_int;

    Data bytes;
    export_bits(value, std::back_inserter(bytes), 8);

    if (bytes.empty() || (bytes.size() == 1 && bytes[0] == 0)) {
        return {0x80};
    }

    return encode(bytes);
}

Data RLP::encodeList(const Data& encoded) noexcept {
    auto result = encodeHeader(encoded.size(), 0xc0, 0xf7);
    result.reserve(result.size() + encoded.size());
    result.insert(result.end(), encoded.begin(), encoded.end());
    return result;
}

Data RLP::encode(const Transaction& transaction) noexcept {
    auto encoded = Data();
    append(encoded, encode(transaction.nonce));
    append(encoded, encode(transaction.gasPrice));
    append(encoded, encode(transaction.gasLimit));
    append(encoded, encode(transaction.to.bytes));
    append(encoded, encode(transaction.amount));
    append(encoded, encode(transaction.payload));
    append(encoded, encode(transaction.v));
    append(encoded, encode(transaction.r));
    append(encoded, encode(transaction.s));
    return encodeList(encoded);
}

Data RLP::encode(const Data& data) noexcept {
    if (data.size() == 1 && data[0] <= 0x7f) {
        // Fits in single byte, no header
        return data;
    }

    auto encoded = encodeHeader(data.size(), 0x80, 0xb7);
    encoded.insert(encoded.end(), data.begin(), data.end());
    return encoded;
}

Data RLP::encodeHeader(uint64_t size, uint8_t smallTag, uint8_t largeTag) noexcept {
    if (size < 56) {
        return {static_cast<uint8_t>(smallTag + size)};
    }

    const auto sizeData = putint(size);

    auto header = Data();
    header.reserve(1 + sizeData.size());
    header.push_back(largeTag + static_cast<uint8_t>(sizeData.size()));
    header.insert(header.end(), sizeData.begin(), sizeData.end());
    return header;
}

Data RLP::putint(uint64_t i) noexcept {
    // clang-format off
    if (i < (1l << 8))
        return {static_cast<uint8_t>(i)};
    if (i < (1l << 16))
        return {
            static_cast<uint8_t>(i >> 8),
            static_cast<uint8_t>(i),
        };
    if (i < (1l << 24))
        return {
            static_cast<uint8_t>(i >> 16),
            static_cast<uint8_t>(i >> 8),
            static_cast<uint8_t>(i),
        };
    if (i < (1l << 32))
        return {
            static_cast<uint8_t>(i >> 24),
            static_cast<uint8_t>(i >> 16),
            static_cast<uint8_t>(i >> 8),
            static_cast<uint8_t>(i),
        };
    if (i < (1l << 40))
        return {
            static_cast<uint8_t>(i >> 32),
            static_cast<uint8_t>(i >> 24),
            static_cast<uint8_t>(i >> 16),
            static_cast<uint8_t>(i >> 8),
            static_cast<uint8_t>(i),
        };
    if (i < (1l << 48))
        return {
            static_cast<uint8_t>(i >> 40),
            static_cast<uint8_t>(i >> 32),
            static_cast<uint8_t>(i >> 24),
            static_cast<uint8_t>(i >> 16),
            static_cast<uint8_t>(i >> 8),
            static_cast<uint8_t>(i),
        };
    if (i < (1l << 56))
        return {
            static_cast<uint8_t>(i >> 48),
            static_cast<uint8_t>(i >> 40),
            static_cast<uint8_t>(i >> 32),
            static_cast<uint8_t>(i >> 24),
            static_cast<uint8_t>(i >> 16),
            static_cast<uint8_t>(i >> 8),
            static_cast<uint8_t>(i),
        };

    return {
        static_cast<uint8_t>(i >> 56),
        static_cast<uint8_t>(i >> 48),
        static_cast<uint8_t>(i >> 40),
        static_cast<uint8_t>(i >> 32),
        static_cast<uint8_t>(i >> 24),
        static_cast<uint8_t>(i >> 16),
        static_cast<uint8_t>(i >> 8),
        static_cast<uint8_t>(i),
    };
    // clang-format on
}
