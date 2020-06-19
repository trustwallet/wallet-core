// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "RLP.h"

#include "../Data.h"
#include "../uint256.h"
#include "../BinaryCoding.h"
#include "../HexCoding.h"

#include <nlohmann/json.hpp>
#include <tuple>

using namespace TW;
using namespace TW::Ethereum;

using json = nlohmann::json;

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
    append(encoded, encode(transaction.to));
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
    if (i < (1ULL << 8))
        return {static_cast<uint8_t>(i)};
    if (i < (1ULL << 16))
        return {
            static_cast<uint8_t>(i >> 8),
            static_cast<uint8_t>(i),
        };
    if (i < (1ULL << 24))
        return {
            static_cast<uint8_t>(i >> 16),
            static_cast<uint8_t>(i >> 8),
            static_cast<uint8_t>(i),
        };
    if (i < (1ULL << 32))
        return {
            static_cast<uint8_t>(i >> 24),
            static_cast<uint8_t>(i >> 16),
            static_cast<uint8_t>(i >> 8),
            static_cast<uint8_t>(i),
        };
    if (i < (1ULL << 40))
        return {
            static_cast<uint8_t>(i >> 32),
            static_cast<uint8_t>(i >> 24),
            static_cast<uint8_t>(i >> 16),
            static_cast<uint8_t>(i >> 8),
            static_cast<uint8_t>(i),
        };
    if (i < (1ULL << 48))
        return {
            static_cast<uint8_t>(i >> 40),
            static_cast<uint8_t>(i >> 32),
            static_cast<uint8_t>(i >> 24),
            static_cast<uint8_t>(i >> 16),
            static_cast<uint8_t>(i >> 8),
            static_cast<uint8_t>(i),
        };
    if (i < (1ULL << 56))
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

Data RLP::decodeRawTransaction(const Data& data) {
    auto decoded = decode(data).decoded;
    if (decoded.size() < 9) {
        return {};
    }
    auto result = json {
        {"nonce", hexEncoded(decoded[0])},
        {"gasPrice", hexEncoded(decoded[1])},
        {"gas", hexEncoded(decoded[2])},
        {"to", hexEncoded(decoded[3])},
        {"value", hexEncoded(decoded[4])},
        {"input", hexEncoded(decoded[5])},
        {"v", hexEncoded(decoded[6])},
        {"r", hexEncoded(decoded[7])},
        {"s", hexEncoded(decoded[8])},
    }.dump();
    return Data(result.begin(), result.end());
}

static RLP::DecodedItem decodeList(const Data& input) {
    RLP::DecodedItem item;
    auto remainder = input;
    while(true) {
        auto listItem = RLP::decode(remainder);
        item.decoded.push_back(listItem.decoded[0]);
        if (listItem.remainder.size() == 0) {
            break;
        } else {
            remainder = listItem.remainder;
        }
    }
    return item;
}

static uint64_t decodeLength(const Data& data) {
    size_t index = 0;
    auto decodedLen = decodeVarInt(data, index);
    if (!std::get<0>(decodedLen)) {
        throw std::invalid_argument("can't decode length of string/list length");
    }
    return std::get<1>(decodedLen);
}

RLP::DecodedItem RLP::decode(const Data& input) {
    if (input.size() == 0) {
        throw std::invalid_argument("can't decode empty rlp data");
    }
    RLP::DecodedItem item;
    auto inputLen = input.size();
    auto prefix = input[0];
    if (prefix <= 0x7f) {
        // a single byte whose value is in the [0x00, 0x7f] range, that byte is its own RLP encoding.
        item.decoded.push_back(Data{input[0]});
        item.remainder = Data(input.begin() + 1, input.end());
        return item;
    }
    if (prefix <= 0xb7) {
        // short string
        // string is 0-55 bytes long. A single byte with value 0x80 plus the length of the string followed by the string
        // The range of the first byte is [0x80, 0xb7]

        // empty string
        if (prefix == 0x80) {
            item.decoded.push_back(Data());
            item.remainder = Data(input.begin() + 1, input.end());
            return item;
        }

        auto strLen = prefix - 0x80;
        if (strLen == 1 && input[1] <= 0x7f) {
            throw std::invalid_argument("single byte below 128 must be encoded as itself");
        }

        item.decoded.push_back(subData(input, 1, strLen));
        item.remainder = Data(input.begin() + 1 + strLen, input.end());

        return item;
    } 
    if (prefix <= 0xbf) {
        // long string
        auto lenOfStrLen = prefix - 0xb7;
        auto strLen = static_cast<size_t>(decodeLength(subData(input, 1, lenOfStrLen)));
        if (inputLen < lenOfStrLen || inputLen < lenOfStrLen + strLen) {
            throw std::invalid_argument("Invalid rlp encoding length");
        }
        auto data = subData(input, 1 + lenOfStrLen, strLen);
        item.decoded.push_back(data);
        item.remainder = Data(input.begin() + 1 + lenOfStrLen + strLen, input.end());
        return item;
    } 
    if (prefix <= 0xf7) {
        // a list between  0-55 bytes long
        auto listLen = prefix - 0xc0;
        if (inputLen < listLen) {
            throw std::invalid_argument("Invalid rlp string length");
        }
        
        // empty list
        if (listLen == 0) {
            item.remainder = Data(input.begin() + 1, input.end());
            return item;
        }

        // decode list
        auto listItem = decodeList(subData(input, 1, listLen));
        for (auto& data : listItem.decoded) {
            item.decoded.push_back(data);
        }
        item.remainder = Data(input.begin() + 1 + listLen, input.end());
        return item;
    } 
    if (prefix <= 0xff) {
        auto lenOfListLen = prefix - 0xf7;
        auto listLen = static_cast<size_t>(decodeLength(subData(input, 1, lenOfListLen)));
        if (inputLen < lenOfListLen || inputLen < lenOfListLen + listLen) {
            throw std::invalid_argument("Invalid rlp list length");
        }
        if (input[1] == 0) {
            throw std::invalid_argument("multi-byte length must have no leading zero");
        }
        if (listLen < 56) {
            throw std::invalid_argument("length below 56 must be encoded in one byte");
        }
        // decode list
        auto listItem = decodeList(subData(input, 1 + lenOfListLen, listLen));
        for (auto& data : listItem.decoded) {
            item.decoded.push_back(data);
        }
        item.remainder = Data(input.begin() + 1 + lenOfListLen + listLen, input.end());
        return item;
    }
    throw std::invalid_argument("input don't conform RLP encoding form");
}
