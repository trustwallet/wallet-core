// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Array.h"

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

void ParamArray::encode(Data& data) const {
    size_t n = _params.getCount();
    ABI::encode(uint256_t(n), data);

    size_t headSize = 0;
    for (auto i = 0; i < n; ++i) {
        auto p = _params.getParamUnsafe(i);
        if (p->isDynamic()) {
            headSize += 32;
        } else {
            headSize += p->getSize();
        }
    }

    size_t dynamicOffset = 0;
    for (auto i = 0; i < n; ++i) {
        auto p = _params.getParamUnsafe(i);
        if (p->isDynamic()) {
            ABI::encode(uint256_t(headSize + dynamicOffset), data);
            dynamicOffset += p->getSize();
        } else {
            p->encode(data);
        }
    }

    for (auto i = 0; i < n; ++i) {
        auto p = _params.getParamUnsafe(i);
        if (p->isDynamic()) {
            p->encode(data);
        }
    }
}

bool ParamArray::decode(const Data& encoded, size_t& offset_inout) {
    size_t origOffset = offset_inout;
    // read length
    uint256_t len256;
    if (!ABI::decode(encoded, len256, offset_inout)) { return false; }
    // check if length is in the size_t range
    size_t len = static_cast<size_t>(len256);
    if (len256 != static_cast<uint256_t>(len)) { return false; }
    // read values
    auto n = _params.getCount();
    if (n != len) {
        // Element number mismatch: the proto has to have exact same number of values as in the encoded form
        // Note: this could be handles in a smarter way, and create more elements as needed
        return false;
    }
    for (auto i = 0; i < n; ++i) {
        if (!_params.getParamUnsafe(i)->decode(encoded, offset_inout)) { return false; }
    }
    // padding
    offset_inout = origOffset + Util::paddedTo32(offset_inout - origOffset);
    return true;
}