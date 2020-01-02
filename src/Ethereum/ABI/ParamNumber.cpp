// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "ParamNumber.h"
#include "Util.h"

#include "../../Data.h"
#include "../../uint256.h"

#include <string>
#include <cassert>

using namespace TW;
using namespace TW::Ethereum::ABI;

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
    assert(bits >= 8 && bits <= 256 && (bits % 8) == 0);
    _mask = (uint256_t(1) << bits) - 1;
}

void ParamIntN::setVal(int256_t val) {
    // mask it to the given bits
    if (val < 0) {
        _val = Util::int256FromUint256(~((~((uint256_t)val)) & _mask));
    } else {
        _val = Util::int256FromUint256(((uint256_t)val) & _mask);
    }
}

bool ParamIntN::decodeNumber(const Data& encoded, int256_t& decoded, size_t& offset_inout) {
    uint256_t valU;
    auto res = ABI::decode(encoded, valU, offset_inout);
    decoded = Util::int256FromUint256(valU);
    return res;
}

bool ParamIntN::decode(const Data& encoded, size_t& offset_inout) {
    int256_t temp;
    auto res = decodeNumber(encoded, temp, offset_inout);
    setVal(temp);
    return res;
}

void ParamIntN::init()
{
    assert(bits >= 8 && bits <= 256 && (bits % 8) == 0);
    _mask = (uint256_t(1) << bits) - 1;
}
