// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Parameters.h"
#include "ValueEncoder.h"
#include <Hash.h>

#include <cassert>
#include <string>

using namespace TW::Ethereum::ABI;
using namespace TW;

ParamSet::~ParamSet() {
    _params.clear();
}

/// Returns the index of the parameter
int ParamSet::addParam(const std::shared_ptr<ParamBase>& param) {
    if (param.get() == nullptr) {
        return -1;
    }
    assert(param.get() != nullptr);
    _params.push_back(param);
    return static_cast<int>(_params.size() - 1);
}

void ParamSet::addParams(const std::vector<std::shared_ptr<ParamBase>>& params) {
    for (auto p : params) {
        addParam(p);
    }
}

bool ParamSet::getParam(int paramIndex, std::shared_ptr<ParamBase>& param_out) const {
    if (paramIndex >= _params.size() || paramIndex < 0) {
        return false;
    }
    param_out = _params[paramIndex];
    return true;
}

std::shared_ptr<ParamBase> ParamSet::getParamUnsafe(int paramIndex) const {
    if (_params.size() == 0) {
        // zero parameter, nothing to return.  This may cause trouble (segfault)
        return nullptr;
    }
    if (paramIndex >= _params.size() || paramIndex < 0) {
        // invalid index, return the first instead of nullptr
        return _params[0];
    }
    return _params[paramIndex];
}

/// Return the function type signature, of the form "baz(int32,uint256)"
std::string ParamSet::getType() const {
    std::string t = "(";
    int cnt = 0;
    for (auto p : _params) {
        if (cnt++ > 0) {
            t += ",";
        }
        t += p->getType();
    }
    t += ")";
    return t;
}

bool ParamSet::isDynamic() const {
    for (const auto& p: _params) {
        if (p->isDynamic()) {
            return true;
        }
    }
    return false;
}

size_t ParamSet::getSize() const {
    // 2-pass encoding
    size_t s = 0;
    for (auto p: _params) {
        if (p->isDynamic() || p->getSize() > ValueEncoder::encodedIntSize) {
            // offset used
            s += 32;
        }
        s += p->getSize();
    }
    return ValueEncoder::paddedTo32(s);
}

size_t ParamSet::getHeadSize() const {
    size_t s = 0;
    for (auto p : _params) {
        if (p->isDynamic()) {
            s += 32;
        } else {
            s += p->getSize();
        }
    }
    return s;
}

void ParamSet::encode(Data& data) const {
    // 2-pass encoding
    size_t headSize = getHeadSize();
    size_t dynamicOffset = 0;

    // pass 1: small values or indices
    for (auto p : _params) {
        if (p->isDynamic() || p->getSize() > ValueEncoder::encodedIntSize) {
            // include only offset
            ValueEncoder::encodeUInt256(uint256_t(headSize + dynamicOffset), data);
            dynamicOffset += p->getSize();
        } else {
            // encode small data
            p->encode(data);
        }
    }

    // pass 2: dynamic values
    for (auto p : _params) {
        if (p->isDynamic() || p->getSize() > ValueEncoder::encodedIntSize) {
            // encode large data
            p->encode(data);
        }
    }
}

bool ParamSet::decode(const Data& encoded, size_t& offset_inout) {
    // pass 1: small values
    for (auto p : _params) {
        if (p->isDynamic()) {
            uint256_t index;
            if (!ABI::decode(encoded, index, offset_inout)) {
                return false;
            }
            // index is read but not used
        } else {
            if (!p->decode(encoded, offset_inout)) {
                return false;
            }
        }
    }
    // pass2: large values
    for (auto p : _params) {
        if (p->isDynamic()) {
            if (!p->decode(encoded, offset_inout)) {
                return false;
            }
        }
    }
    return true;
}

Data ParamSet::encodeHashes() const {
    Data hashes;
    for (auto p: _params) {
        append(hashes, p->hashStruct());
    }
    return hashes;
}

Data Parameters::hashStruct() const {
    Data hash(32);
    Data hashes = _params.encodeHashes();
    if (hashes.size() > 0) {
        hash = Hash::keccak256(hashes);
    }
    return hash;
}
