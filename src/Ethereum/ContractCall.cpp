// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "ContractCall.h"
#include "ABI.h"
#include "ABI/ParamAddress.h"
#include "HexCoding.h"
#include "uint256.h"
#include <iostream>

using namespace TW;
using namespace TW::Ethereum;

static void fill(ABI::Function& func, const std::string& type) {
    if (type == "address") {
        auto param = std::make_shared<ABI::ParamAddress>();
        func.addParam(param, false);
    } else if (type == "uint256") {
        auto param = std::make_shared<ABI::ParamUInt256>();
        func.addParam(param, false);
    } else if (type == "bool") {
        auto param = std::make_shared<ABI::ParamBool>();
        func.addParam(param, false);
    } else if (type == "bytes") {
        auto param = std::make_shared<ABI::ParamByteArray>();
        func.addParam(param, false);
    } else if (type == "address[]") {
        auto param = std::make_shared<ABI::ParamArray>();
        param->addParam(std::make_shared<ABI::ParamAddress>());
        func.addParam(param, false);
    }
}

static std::string getValue(ABI::Function& func, const std::string& type, int idx) {
    std::shared_ptr<ABI::ParamBase> param;
    func.getInParam(idx, param);
    if (type == "address") {
        auto value = std::dynamic_pointer_cast<ABI::ParamAddress>(param);
        if (value == nullptr) {
            return "";
        }
        return hex(value->getData());
    } else if (type == "uint256") {
        auto value = std::dynamic_pointer_cast<ABI::ParamUInt256>(param);
        if (value == nullptr) {
            return "0";
        }
        return (toString(value->getVal()));
    } else if (type == "bool") {
        auto value = std::dynamic_pointer_cast<ABI::ParamBool>(param);
        if (value == nullptr) {
            return "false";
        }
        return value->getVal() ? "true" : "false";
    } else if (type == "bytes") {
        // FIXME
        return "";
    } else if (type == "address[]") {
        // FIXME
        return "";
    }
    return "";
}

std::optional<std::string> Ethereum::decodeCall(const Data& call, const nlohmann::json& abi) {
    if (call.size() <= 4) {
        return {};
    }
    auto prefix = hex(Data(call.begin(), call.begin() + 4));
    if (abi[prefix] == nullptr) {
        return {};
    }
    auto info = abi[prefix];
    auto func = ABI::Function(info["name"]);
    for (auto& input : info["inputs"]) {
        fill(func, input["type"]);
    }
    size_t offset = 0;
    auto result = func.decodeInput(call, offset);
    if (!result) {
        return {};
    }
    for (int i = 0; i < info["inputs"].size(); i++) {
        std::cout<<getValue(func, info["inputs"][i]["type"], i)<<std::endl;
    }
    return "";
}
