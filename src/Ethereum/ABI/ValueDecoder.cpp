// Copyright © 2017-2023 Trust Wallet.
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

std::vector<std::string> ValueDecoder::decodeArray(const Data& data, const std::string& type) {
    auto param = ParamFactory::make(type);
    if (!param) {
        return std::vector<std::string>{};
    }
    size_t offset = 0;
    if (!param->decode(data, offset)) {
        return std::vector<std::string>{};
    }
    auto values = ParamFactory::getArrayValue(param, type);
    return values;
}

} // namespace TW::Ethereum::ABI
