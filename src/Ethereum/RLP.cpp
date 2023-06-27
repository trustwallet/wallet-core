// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "RLP.h"

#include "../BinaryCoding.h"
#include "../Numeric.h"

#include <tuple>

namespace TW::Ethereum {

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

    const auto sizeData = putVarInt(size);

    auto header = Data();
    header.reserve(1 + sizeData.size());
    header.push_back(largeTag + static_cast<uint8_t>(sizeData.size()));
    header.insert(header.end(), sizeData.begin(), sizeData.end());
    return header;
}

Data RLP::putVarInt(uint64_t i) noexcept {
    Data bytes; // accumulate bytes here, in reverse order
    do {
        // take LSB byte, append
        bytes.push_back(i & 0xff);
        i = i >> 8;
    } while (i);
    assert(bytes.size() >= 1 && bytes.size() <= 8);
    std::reverse(bytes.begin(), bytes.end());
    return bytes;
}

uint64_t RLP::parseVarInt(size_t size, const Data& data, size_t index) {
    if (size < 1 || size > 8) {
        throw std::invalid_argument("invalid length length");
    }
    if (data.size() - index < size) {
        throw std::invalid_argument("Not enough data for varInt");
    }
    if (size >= 2 && data[index] == 0) {
        throw std::invalid_argument("multi-byte length must have no leading zero");
    }
    uint64_t val = 0;
    for (auto i = 0U; i < size; ++i) {
        val = val << 8;
        val += data[index + i];
    }
    return static_cast<size_t>(val);
}

RLP::DecodedItem RLP::decodeList(const Data& input) {
    RLP::DecodedItem item;
    auto remainder = input;
    while (true) {
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

RLP::DecodedItem RLP::decode(const Data& input) {
    if (input.size() == 0) {
        throw std::invalid_argument("can't decode empty rlp data");
    }
    RLP::DecodedItem item;
    auto inputLen = input.size();
    auto prefix = input[0];
    if (prefix <= 0x7f) {
        // 00--7f: a single byte whose value is in the [0x00, 0x7f] range, that byte is its own RLP encoding.
        item.decoded.emplace_back(Data{input[0]});
        item.remainder = subData(input, 1);
        return item;
    }
    if (prefix <= 0xb7) {
        // 80--b7: short string
        // string is 0-55 bytes long. A single byte with value 0x80 plus the length of the string followed by the string
        // The range of the first byte is [0x80, 0xb7]

        // empty string
        if (prefix == 0x80) {
            item.decoded.emplace_back();
            item.remainder = subData(input, 1);
            return item;
        }

        auto strLen = prefix - 0x80;
        if (strLen == 1 && input[1] <= 0x7f) {
            throw std::invalid_argument("single byte below 128 must be encoded as itself");
        }

        if (inputLen < (1U + strLen)) {
            throw std::invalid_argument(std::string("invalid short string, length ") + std::to_string(strLen));
        }
        item.decoded.push_back(subData(input, 1, strLen));
        item.remainder = subData(input, 1 + strLen);

        return item;
    }
    if (prefix <= 0xbf) {
        // b8--bf: long string
        auto lenOfStrLen = size_t(prefix - 0xb7);
        auto strLen = static_cast<size_t>(parseVarInt(lenOfStrLen, input, 1));
        bool isStrLenInvalid = inputLen < lenOfStrLen
            || checkAddUnsignedOverflow(1U + lenOfStrLen, strLen)
            || inputLen < (1U + lenOfStrLen + strLen);
        if (isStrLenInvalid) {
            throw std::invalid_argument(std::string("Invalid rlp encoding length, length ") + std::to_string(strLen));
        }
        auto data = subData(input, 1 + lenOfStrLen, strLen);
        item.decoded.push_back(data);
        item.remainder = subData(input, 1 + lenOfStrLen + strLen);
        return item;
    }
    if (prefix <= 0xf7) {
        // c0--f7: a list between  0-55 bytes long
        auto listLen = size_t(prefix - 0xc0);
        if (inputLen < (1 + listLen)) {
            throw std::invalid_argument(std::string("Invalid rlp string length, length ") + std::to_string(listLen));
        }
        // empty list
        if (listLen == 0) {
            item.remainder = subData(input, 1);
            return item;
        }

        // decode list
        auto listItem = decodeList(subData(input, 1, listLen));
        for (auto& data : listItem.decoded) {
            item.decoded.push_back(data);
        }
        item.remainder = subData(input, 1 + listLen);
        return item;
    }
    // f8--ff
    auto lenOfListLen = size_t(prefix - 0xf7);
    auto listLen = static_cast<size_t>(parseVarInt(lenOfListLen, input, 1));
    if (listLen < 56) {
        throw std::invalid_argument("length below 56 must be encoded in one byte");
    }
    auto isListLenInvalid = inputLen < lenOfListLen
        || checkAddUnsignedOverflow(1U + lenOfListLen, listLen)
        || inputLen < (1U + lenOfListLen + listLen);
    if (isListLenInvalid) {
        throw std::invalid_argument(std::string("Invalid rlp list length, length ") + std::to_string(listLen));
    }

    // decode list
    auto listItem = decodeList(subData(input, 1 + lenOfListLen, listLen));
    for (auto& data : listItem.decoded) {
        item.decoded.push_back(data);
    }
    item.remainder = subData(input, 1 + lenOfListLen + listLen);
    return item;
}

} // namespace TW::Ethereum
