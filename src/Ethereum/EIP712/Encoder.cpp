// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Encoder.h"

#include <Ethereum/Address.h>

#include <Data.h>
#include <Hash.h>

#include <cassert>

namespace TW::Ethereum::EIP712 {

using namespace TW;

Data Encoder::encodeBool(bool value) {
    Data res(encodedIntSize);
    res[encodedIntSize - 1] = value ? 1 : 0;
    return res;
}

Data Encoder::encodeInt32(int32_t value) {
    uint256_t value256 = static_cast<uint256_t>(value); // cast up
    return encodeUInt256(value256);
}

Data Encoder::encodeUInt32(uint32_t value) {
    uint256_t value256 = static_cast<uint256_t>(value); // cast up
    return encodeUInt256(value256);
}

// int256_t -> uint256_t conversion, non trivial
uint256_t Encoder::uint256FromInt256(int256_t x)
{
    if (x >= 0) { return static_cast<uint256_t>(x); }
    return ~static_cast<uint256_t>(-(x + 1));
}

Data Encoder::encodeInt256(const int256_t& value) {
    return encodeUInt256(uint256FromInt256(value));
}

inline Data paddedOnLeft(const Data& data) {
    int paddingSize = Encoder::encodedIntSize - data.size();
    if (paddingSize <= 0) { return data; }
    assert(data.size() < Encoder::encodedIntSize);
    Data res(paddingSize); // padding on left
    TW::append(res, data);
    return res;
}

Data Encoder::encodeUInt256(const uint256_t& value) {
    return paddedOnLeft(TW::store(value));
}

/// Encoding primitive: encode a number of bytes by taking hash
inline Data encodeBytesWithHash(const byte* data, size_t length) {
    Data res = Hash::keccak256(data, length);
    assert(res.size() == Encoder::encodedIntSize);
    return res;
}

Data Encoder::encodeAddress(const Data& value) {
    const Address address(value);
    return paddedOnLeft(TW::data(address.bytes.data(), address.bytes.size()));
}

Data Encoder::encodeString(const std::string& value) {
    Data valueData = TW::data(value);
    return encodeBytesWithHash(valueData.data(), valueData.size());
}

Data Encoder::encodeBytes(const Data& value) {
    auto len = value.size();
    if (len > encodedIntSize) {
        // too long, truncating
        len = encodedIntSize;
    }
    assert(len <= encodedIntSize);
    Data res(encodedIntSize);
    std::copy(value.begin(), value.begin() + len, res.begin());
    // padded on right
    return res;
}

Data Encoder::encodeBytesDyn(const Data& value) {
    return encodeBytesWithHash(value.data(), value.size());
}

} // namespace TW::Ethereum::EIP712
