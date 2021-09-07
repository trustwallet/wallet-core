// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "ContractCall.h"
#include "ABI.h"
#include "HexCoding.h"
#include "uint256.h"
#include <boost/algorithm/string/predicate.hpp>

using namespace std;
using json = nlohmann::json;

namespace TW::Ethereum::ABI {

static void fillArray(ParamSet& paramSet, const string& type) {
    auto param = make_shared<ParamArray>();
    auto baseType = string(type.begin(), type.end() - 2);
    auto value = ParamFactory::make(baseType);
    param->addParam(value);
    paramSet.addParam(param);
}

static void fill(ParamSet& paramSet, const string& type) {
    if (boost::algorithm::ends_with(type, "[]")) {
        fillArray(paramSet, type);
    } else {
        auto param = ParamFactory::make(type);
        paramSet.addParam(param);
    }
}

static vector<string> getArrayValue(Function& func, const string& type, int idx) {
    shared_ptr<ParamBase> param;
    func.getInParam(idx, param);
    return ParamFactory::getArrayValue(param, type);
}

static string getValue(Function& func, const string& type, int idx) {
    shared_ptr<ParamBase> param;
    func.getInParam(idx, param);
    return ParamFactory::getValue(param, type);
}

static json buildInputs(Function& func, const json& registry) {
    auto inputs = json::array();
    for (int i = 0; i < registry["inputs"].size(); i++) {
        auto info = registry["inputs"][i];
        auto type = info["type"];
        auto input = json{
            {"name", info["name"]},
            {"type", type}
        };
        if (boost::algorithm::ends_with(type.get<string>(), "[]")) {
            input["value"] = json(getArrayValue(func, type, i));
        } else if (type == "bool") {
            input["value"] = getValue(func, type, i) == "true" ? json(true) : json(false);
        } else {
            input["value"] = getValue(func, type, i);
        }
        inputs.push_back(input);
    }
    return inputs;
}

void fillTuple(ParamSet& paramSet, const json& jsonSet); // forward

void decodeParamSet(ParamSet& paramSet, const json& jsonSet) {
    for (auto& comp : jsonSet) {
        if (comp["type"] == "tuple") {
            fillTuple(paramSet, comp["components"]);
        } else {        
            fill(paramSet, comp["type"]);
        }
    }    
}

void fillTuple(ParamSet& paramSet, const json& jsonSet) {
    std::shared_ptr<ParamTuple> param = make_shared<ParamTuple>();
    decodeParamSet(param->_params, jsonSet);
    paramSet.addParam(param);
}

optional<string> decodeCall(const Data& call, const json& abi) {
    // check bytes length
    if (call.size() <= 4) {
        return {};
    }

    auto methodId = hex(Data(call.begin(), call.begin() + 4));

    if (abi.find(methodId) == abi.end()) {
        return {};
    }

    // build Function with types
    const auto registry = abi[methodId];
    auto func = Function(registry["name"]);
    decodeParamSet(func._inParams, registry["inputs"]);

    // decode inputs
    size_t offset = 0;
    auto success = func.decodeInput(call, offset);
    if (!success) {
        return {};
    }

    // build output json
    auto decoded = json{
        {"function", func.getType()},
        {"inputs", buildInputs(func, registry)},
    };
    return decoded.dump();
}

} // namespace TW::Ethereum::ABI
