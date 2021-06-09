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

ParamSet::~ParamSet() {
    _params.clear();
}

/// Returns the index of the parameter
int ParamSet::addParam(const std::shared_ptr<ParamBase>& param) {
    assert(param.get() != nullptr);
    if (param.get() == nullptr) {
        return -1;
    }
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

TW::Data ParamSet::encodeHashes() const {
    TW::Data hashes;
    for (auto p: _params) {
        TW::append(hashes, p->hashStruct());
    }
    return hashes;
}

TW::Data Parameters::hashStruct() const {
    return Hash::keccak256(_params.encodeHashes());
}

std::string ParamNamed::getType() const {
    return _param->getType() + " " + _name;
}

ParamSetNamed::~ParamSetNamed() {
    _params.clear();
}

/// Returns the index of the parameter
int ParamSetNamed::addParam(const std::shared_ptr<ParamNamed>& param) {
    assert(param.get() != nullptr);
    if (param.get() == nullptr) {
        return -1;
    }
    _params.push_back(param);
    return static_cast<int>(_params.size() - 1);
}

void ParamSetNamed::addParams(const std::vector<std::shared_ptr<ParamNamed>>& params) {
    for (auto p : params) {
        addParam(p);
    }
}

std::string ParamSetNamed::getType() const {
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

TW::Data ParamSetNamed::encodeHashes() const {
    TW::Data hashes;
    for (auto p: _params) {
        TW::append(hashes, p->hashStruct());
    }
    return hashes;
}

std::string ParamSetNamed::getExtraTypes(std::vector<std::string>& ignoreList) const {
    std::string types;
    for (auto& p: _params) {
        auto pType = p->_param->getType();
        if (std::find(ignoreList.begin(), ignoreList.end(), pType) == ignoreList.end()) {
            types += p->getExtraTypes(ignoreList);
            ignoreList.push_back(pType);
        }
    }
    return types;
}

TW::Data ParametersNamed::encodeHashes() const {
    std::vector<std::string> ignoreList;
    auto extraTypes = getExtraTypes(ignoreList);
    Data hashes = Hash::keccak256(TW::data(extraTypes));
    TW::append(hashes, _params.encodeHashes());
    return hashes;
}

TW::Data ParametersNamed::hashStruct() const {
    return Hash::keccak256(encodeHashes());
}

std::string ParametersNamed::getExtraTypes(std::vector<std::string>& ignoreList) const {
    std::string types;
    if (std::find(ignoreList.begin(), ignoreList.end(), _name) == ignoreList.end()) {
        types += getTypeLong();
        ignoreList.push_back(_name);
    }
    types += _params.getExtraTypes(ignoreList);
    return types;
}
