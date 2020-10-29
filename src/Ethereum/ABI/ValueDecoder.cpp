// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "ValueDecoder.h"
#include "Array.h"
#include "ParamFactory.h"

namespace TW::Ethereum::ABI {

uint256_t ValueDecoder::decodeUInt256(const Data& data) {
    if (data.size() > 32) {
        return load(subData(data, 0, 32));
    }
    return load(data);
}

std::string ValueDecoder::decodeValue(const Data& data, const std::string& type) {
    auto param = ParamFactory::make(type);
    if (!param) {
        return "";
    }
    size_t offset = 0;
    if (!param->decode(data, offset)) {
        return "";
    }
    return ParamFactory::getValue(param, param->getType());
}

std::vector<std::string> ValueDecoder::decodeArray(const Data& data, const std::string& elementType) {
    auto elemParam = ParamFactory::make(elementType);
    if (!elemParam) {
        return std::vector<std::string>{};
    }
    ParamArray paramArray(elemParam);
    size_t offset = 0;
    auto res = paramArray.decode(data, offset);
    if (!res) {
        return std::vector<std::string>{};
    }
    auto values = paramArray.getVal();
    auto size = values.size();
    auto result = std::vector<std::string>(size);
    for(auto i = 0; i < size; ++i) {
        result[i] = ParamFactory::getValue(values[i], values[i]->getType());
    }
    return result;
}

} // namespace TW::Ethereum::ABI
