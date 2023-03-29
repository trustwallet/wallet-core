// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "ParamNumber.h"

#include <Data.h>
#include <uint256.h>
#include <HexCoding.h>

#include <boost/lexical_cast.hpp>

#include <string>
#include <cassert>

namespace TW::Ethereum::ABI {

bool ParamUInt256::setUInt256FromValueJson(uint256_t& dest, const std::string& value) {
    // try hex string or number
    if (value.length() >= 3 && value.substr(0, 2) == "0x") {
        dest = load(parse_hex(value));
        return true;
    }
    return boost::conversion::detail::try_lexical_convert(value, dest);
}

bool ParamInt256::setInt256FromValueJson(int256_t& dest, const std::string& value) {
    // try hex string or number
    if (value.length() >= 3 && value.substr(0, 2) == "0x") {
        dest = ValueEncoder::int256FromUint256(load(parse_hex(value)));
        return true;
    }
    return boost::conversion::detail::try_lexical_convert(value, dest);
}

bool ParamBool::setValueJson(const std::string& value) {
    if (value == "true" || value == "1") {
        setVal(true);
        return true;
    }
    if (value == "false" || value == "0") {
        setVal(false);
        return true;
    }
    return false;
}

bool ParamUInt8::setValueJson(const std::string& value) {
    uint16_t val;
    if (!boost::conversion::detail::try_lexical_convert(value, val)) {
        return false;
    }
    setVal(static_cast<uint8_t>(val));
    return true;
}

bool ParamInt8::setValueJson(const std::string& value) {
    int16_t val;
    if (!boost::conversion::detail::try_lexical_convert(value, val)) {
        return false;
    }
    setVal(static_cast<int8_t>(val));
    return true;
}

void ParamUIntN::setVal(uint256_t val) {
    // mask it to the given bits
    _val = val & _mask;
}

bool ParamUIntN::decode(const Data& encoded, size_t& offset_inout) {
    uint256_t temp;
    auto res = decodeNumber(encoded, temp, offset_inout);
    setVal(temp);
    return res;
}

void ParamUIntN::init() {
    _mask = maskForBits(bits);
}

uint256_t ParamUIntN::maskForBits(size_t bits) {
    assert(bits >= 8 && bits <= 256 && (bits % 8) == 0);
    // exclude predefined sizes
    assert(bits != 8 && bits != 16 && bits != 32 && bits != 64 && bits != 256);
    return (uint256_t(1) << bits) - 1;
}

void ParamIntN::setVal(int256_t val) {
    // mask it to the given bits
    if (val < 0) {
        _val = ValueEncoder::int256FromUint256(~((~((uint256_t)val)) & _mask));
    } else {
        _val = ValueEncoder::int256FromUint256(((uint256_t)val) & _mask);
    }
}

bool ParamIntN::decodeNumber(const Data& encoded, int256_t& decoded, size_t& offset_inout) {
    uint256_t valU;
    auto res = ABI::decode(encoded, valU, offset_inout);
    decoded = ValueEncoder::int256FromUint256(valU);
    return res;
}

bool ParamIntN::decode(const Data& encoded, size_t& offset_inout) {
    int256_t temp;
    auto res = decodeNumber(encoded, temp, offset_inout);
    setVal(temp);
    return res;
}

void ParamIntN::init() {
    _mask = ParamUIntN::maskForBits(bits);
}

} // namespace TW::Ethereum::ABI
