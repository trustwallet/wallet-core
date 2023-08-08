// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Array.h"
#include "ParamFactory.h"
#include "ValueEncoder.h"
#include <Hash.h>

#include <nlohmann/json.hpp>

#include <cassert>

namespace TW::Ethereum::ABI {

using namespace TW;
using json = nlohmann::json;

int ParamArray::addParam(const std::shared_ptr<ParamBase>& param) {
    assert(param != nullptr);
    if (param == nullptr) {
        return -1;
    }
    if (_params.getCount() >= 1 && param->getType() != getProtoType()) {
        return -2;
    } // do not add different types
    return _params.addParam(param);
}

void ParamArray::addParams(const std::vector<std::shared_ptr<ParamBase>>& params) {
    for (auto p : params) {
        addParam(p);
    }
}

std::shared_ptr<ParamBase> ParamArray::getProtoElem() const {
    if (_params.getCount() >= 1) {
        return _params.getParamUnsafe(0);
    }
    return _proto;
}

std::string ParamArray::getProtoType() const {
    const auto proto = getProtoElem();
    return (proto != nullptr) ? proto->getType() : "__empty__";
}

size_t ParamArray::getSize() const {
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
    auto len = static_cast<size_t>(len256);
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
            _params.addParam(first->clone());
        }
    }

    // read values
    auto res = _params.decode(encoded, offset_inout);

    // padding
    offset_inout = origOffset + ValueEncoder::paddedTo32(offset_inout - origOffset);
    return res;
}

bool ParamArray::setValueJson(const std::string& value) {
    if (_params.getCount() < 1) {
        // no single element
        return false;
    }
    auto valuesJson = json::parse(value, nullptr, false);
    if (valuesJson.is_discarded()) {
        return false;
    }
    if (!valuesJson.is_array()) {
        return false;
    }
    // make sure enough elements are in the array
    while (_params.getCount() < valuesJson.size()) {
        addParam(getProtoElem()->clone());
    }
    int cnt = 0;
    for (const auto& e : valuesJson) {
        std::string eString = e.is_string() ? e.get<std::string>() : e.dump();
        _params.getParamUnsafe(cnt)->setValueJson(eString);
        ++cnt;
    }
    return true;
}

Data ParamArray::hashStruct() const {
    if (_params.getCount() == 0) {
        return Hash::keccak256(Data());
    }
    Data hash(32);
    Data hashes = _params.encodeHashes();
    if (hashes.size() > 0) {
        hash = Hash::keccak256(hashes);
    }
    return hash;
}

std::string ParamArray::getExtraTypes(std::vector<std::string>& ignoreList) const {
    const auto& proto = getProtoElem();
    return (proto != nullptr) ? proto->getExtraTypes(ignoreList) : "";
}

std::shared_ptr<ParamBase> ParamArray::clone() const {
    auto newArray = std::make_shared<ParamArray>();
    newArray->_params = _params.clone();
    newArray->_proto = _proto->clone();
    return newArray;
}

void ParamArrayFix::encode(Data& data) const {
    this->_params.encode(data);
}

bool ParamArrayFix::decode(const Data& encoded, size_t& offset_inout) {
    return this->_params.decode(encoded, offset_inout);
}

bool ParamArrayFix::setValueJson(const std::string& value) {
    auto valuesJson = json::parse(value, nullptr, false);
    if (valuesJson.is_discarded() || !valuesJson.is_array() || _params.getCount() != valuesJson.size()) {
        return false;
    }

    std::size_t idx{0};
    for (auto&& e : valuesJson) {
        std::string eString = e.is_string() ? e.get<std::string>() : e.dump();
        _params.getParamUnsafe(idx)->setValueJson(eString);
        ++idx;
    }
    return true;
}

std::shared_ptr<ParamBase> ParamArrayFix::clone() const {
    auto newArray = std::make_shared<ParamArrayFix>();
    newArray->_params = _params.clone();
    return newArray;
}

void ParamArrayFix::addParams(const Params& params) {
    auto addParamFunctor = [this](auto&& param) {
        if (param == nullptr) {
            throw std::runtime_error("param can't be nullptr");
        }
        if (_params.getCount() >= 1 && param->getType() != _params.getParamUnsafe(0)->getType()) {
            throw std::runtime_error("params need to be the same type");
        } // do not add different types
        _params.addParam(param);
    };
    std::for_each(begin(params), end(params), addParamFunctor);
}

} // namespace TW::Ethereum::ABI
