// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "ValueEncoder.h"

#include <Ethereum/Address.h>

#include <Data.h>
#include <Hash.h>

#include <cassert>

namespace TW::Ethereum::ABI {

using namespace TW;

void ValueEncoder::encodeBool(bool value, Data& inout) {
    append(inout, Data(encodedIntSize));
    inout[inout.size() - 1] = value ? 1 : 0;
}

void ValueEncoder::encodeInt32(int32_t value, Data& inout) {
    // cast up
    encodeInt256(static_cast<int256_t>(value), inout);
}

void ValueEncoder::encodeUInt32(uint32_t value, Data& inout) {
    // cast up
    encodeUInt256(static_cast<uint256_t>(value), inout);
}

void ValueEncoder::encodeInt256(const int256_t& value, Data& inout) {
    encodeUInt256(uint256FromInt256(value), inout);
}

inline Data paddedOnLeft(const Data& inout) {
    int paddingSize = (int)(ValueEncoder::encodedIntSize - inout.size());
    if (paddingSize <= 0) { return inout; }
    assert(inout.size() < ValueEncoder::encodedIntSize);
    Data res(paddingSize); // padding on left
    append(res, inout);
    return res;
}

void ValueEncoder::encodeUInt256(const uint256_t& value, Data& inout) {
    append(inout, store(value, 32));
}

/// Encoding primitive: encode a number of bytes by taking hash
inline void encodeBytesWithHash(const byte* data, size_t length, Data& inout) {
    Data res = Hash::keccak256(data, length);
    assert(res.size() == ValueEncoder::encodedIntSize);
    append(inout, res);
}

void ValueEncoder::encodeAddress(const Data& value, Data& inout) {
    const Address address(value);
    append(inout, paddedOnLeft(data(address.bytes.data(), address.bytes.size())));
}

void ValueEncoder::encodeString(const std::string& value, Data& inout) {
    Data valueData = data(value);
    encodeBytesWithHash(valueData.data(), valueData.size(), inout);
}

void ValueEncoder::encodeBytes(const Data& value, Data& inout) {
    auto len = value.size();
    if (len > encodedIntSize) {
        // too long, truncating
        len = encodedIntSize;
    }
    assert(len <= encodedIntSize);
    append(inout, Data(encodedIntSize));
    std::copy(value.begin(), value.begin() + len, inout.end() - encodedIntSize);
    // note: padded on right
}

void ValueEncoder::encodeBytesDyn(const Data& value, Data& inout) {
    encodeBytesWithHash(value.data(), value.size(), inout);
}

int256_t ValueEncoder::int256FromUint256(uint256_t x) {
    static const uint256_t maxNeg = uint256_t(1) << 255;
    if (x <= maxNeg) { return static_cast<int256_t>(x); }
    return -(static_cast<int256_t>(~x)) - 1;
}

uint256_t ValueEncoder::uint256FromInt256(int256_t x) {
    if (x >= 0) { return static_cast<uint256_t>(x); }
    return ~static_cast<uint256_t>(-(x + 1));
}

} // namespace TW::Ethereum::ABI
