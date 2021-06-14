// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "ParamStruct.h"
#include "ValueEncoder.h"
#include "ParamFactory.h"
#include <Hash.h>

#include <cassert>
#include <string>

using namespace TW::Ethereum::ABI;
using namespace TW;

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

Data ParamSetNamed::encodeHashes() const {
    Data hashes;
    for (auto p: _params) {
        append(hashes, p->hashStruct());
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

std::shared_ptr<ParamNamed> ParamSetNamed::findParamByName(const std::string& name) const {
    for (auto& p: _params) {
        if (p->_name == name) {
            return p;
        }
    }
    return nullptr;
}

Data ParamStruct::hashType() const {
    return Hash::keccak256(TW::data(encodeType()));
}

Data ParamStruct::encodeHashes() const {
    Data hashes;
    Data paramsHashes = _params.encodeHashes();
    if (paramsHashes.size() > 0) {
        auto fullType = encodeType();
        hashes = Hash::keccak256(TW::data(fullType));
        append(hashes, paramsHashes);
    }
    return hashes;
}

Data ParamStruct::hashStruct() const {
    Data hash(32);
    Data hashes = encodeHashes();
    if (hashes.size() > 0) {
        hash = Hash::keccak256(hashes);
    }
    return hash;
}

std::string ParamStruct::getExtraTypes(std::vector<std::string>& ignoreList) const {
    std::string types;
    if (std::find(ignoreList.begin(), ignoreList.end(), _name) == ignoreList.end()) {
        types += _name + _params.getType();
        ignoreList.push_back(_name);
    }
    types += _params.getExtraTypes(ignoreList);
    return types;
}

Data ParamStruct::hashStructJson(const std::string& structType, const std::string& valueJson, const std::string& typesJson) {
    auto str = ParamFactory::makeStruct(structType, valueJson, typesJson);
    if (!str) {
        return {};
    }
    return str->hashStruct();
}
