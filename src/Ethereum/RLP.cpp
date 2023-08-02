// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "RLP.h"

#include "BinaryCoding.h"
#include "Numeric.h"
#include "TrustWalletCore/TWCoinType.h"
#include "rust/Wrapper.h"

#include <tuple>

namespace TW::Ethereum {

Data RLP::encode(const EthereumRLP::Proto::EncodingInput& input) {
    Rust::TWDataWrapper inputData(data(input.SerializeAsString()));
    Rust::TWDataWrapper outputPtr = Rust::tw_ethereum_rlp_encode(TWCoinTypeEthereum, inputData.get());

    auto outputData = outputPtr.toDataOrDefault();
    if (outputData.empty()) {
        return {};
    }

    EthereumRLP::Proto::EncodingOutput output;
    output.ParseFromArray(outputData.data(), static_cast<int>(outputData.size()));

    return data(output.encoded());
}

Data RLP::encodeData(const Data& data) {
    EthereumRLP::Proto::EncodingInput input;
    input.mutable_item()->set_data(data.data(), data.size());
    return encode(input);
}

Data RLP::encodeString(const std::string& s) {
    EthereumRLP::Proto::EncodingInput input;
    input.mutable_item()->set_string_item(s);
    return encode(input);
}

Data RLP::encodeU256(const uint256_t & num) {
    auto numData = store(num);

    EthereumRLP::Proto::EncodingInput input;
    input.mutable_item()->set_number_u256(numData.data(), numData.size());
    return encode(input);
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
