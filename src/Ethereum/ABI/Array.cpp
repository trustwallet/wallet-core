// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Array.h"
#include "ParamFactory.h"
#include "ValueEncoder.h"

#include <cassert>

using namespace TW::Ethereum::ABI;

int ParamArray::addParam(const std::shared_ptr<ParamBase>& param) {
    assert(param != nullptr);
    if (param == nullptr) { return -1; }
    if (_params.getCount() >= 1 && param->getType() != getFirstType()) { return -2; } // do not add different types
    return _params.addParam(param);
}

void ParamArray::addParams(const std::vector<std::shared_ptr<ParamBase>>& params) {
    for (auto p: params) { addParam(p); }
}

std::string ParamArray::getFirstType() const {
    if (_params.getCount() == 0) { return "empty"; }
    return _params.getParamUnsafe(0)->getType();
}

size_t ParamArray::getSize() const
{
    return 32 + _params.getSize();
}

void ParamArray::encode(Data& data) const {
    size_t n = _params.getCount();
    ValueEncoder::encodeUInt256(uint256_t(n), data);
    _params.encode(data);
}

bool ParamArray::decode(const Data& encoded, size_t& offset_inout) {
    size_t origOffset = offset_inout;
    // read length
    uint256_t len256;
    if (!ABI::decode(encoded, len256, offset_inout)) {
        return false;
    }
    // check if length is in the size_t range
    size_t len = static_cast<size_t>(len256);
    if (len256 != uint256_t(len)) {
        return false;
    }
    // check number of values
    auto n = _params.getCount();
    if (n == 0 || n > len) {
        // Encoded length is less than params count, unsafe to continue decoding
        return false;
    }
    if (n < len) {
        // pad with first type
        auto first = _params.getParamUnsafe(0);
        for (size_t i = 0; i < len - n; i++) {
            _params.addParam(ParamFactory::make(first->getType()));
        }
    }

    // read values
    auto res = _params.decode(encoded, offset_inout);

    // padding
    offset_inout = origOffset + ValueEncoder::paddedTo32(offset_inout - origOffset);
    return res;
}
