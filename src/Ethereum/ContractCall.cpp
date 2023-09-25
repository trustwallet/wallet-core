// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "ContractCall.h"
#include "ABI.h"
#include "HexCoding.h"
#include "proto/EthereumAbi.pb.h"
#include "TrustWalletCore/TWCoinType.h"
#include "uint256.h"
#include <boost/algorithm/string/predicate.hpp>

using namespace std;
using json = nlohmann::json;

namespace TW::Ethereum::ABI {

static void fillArray(ParamSet& paramSet, const string& type) {
    auto baseType = string(type.begin(), type.end() - 2);
    auto value = ParamFactory::make(baseType);
    auto param = make_shared<ParamArray>(value);
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

static vector<string> getArrayValue(const ParamSet& paramSet, const string& type, int idx) {
    shared_ptr<ParamBase> param;
    paramSet.getParam(idx, param);
    return ParamFactory::getArrayValue(param, type);
}

static json buildInputs(const ParamSet& paramSet, const json& registry); // forward

static json getTupleValue(const ParamSet& paramSet, [[maybe_unused]] const string& type, int idx, const json& typeInfo) {
    shared_ptr<ParamBase> param;
    paramSet.getParam(idx, param);
    auto paramTuple = dynamic_pointer_cast<ParamTuple>(param);
    if (!paramTuple.get()) {
        return {};
    }
    return buildInputs(paramTuple->_params, typeInfo["components"]);
}

static vector<json> getArrayValueTuple(const ParamSet& paramSet, int idx, const json& typeInfo) {
    shared_ptr<ParamBase> param;
    paramSet.getParam(idx, param);

    auto array = dynamic_pointer_cast<ParamArray>(param);
    if (!array) {
        return {};
    }

    std::vector<json> values;
    for (const auto& tuple : array->getVal()) {
        if (auto paramTuple = dynamic_pointer_cast<ParamTuple>(tuple); paramTuple) {
            values.emplace_back(buildInputs(paramTuple->_params, typeInfo["components"]));
        }
    }

    return values;
}

static string getValue(const ParamSet& paramSet, const string& type, int idx) {
    shared_ptr<ParamBase> param;
    paramSet.getParam(idx, param);
    return ParamFactory::getValue(param, type);
}

static json buildInputs(const ParamSet& paramSet, const json& registry) {
    auto inputs = json::array();
    for (auto i = 0ul; i < registry.size(); i++) {
        auto info = registry[i];
        auto type = info["type"];
        auto input = json{
            {"name", info["name"]},
            {"type", type}
        };
        if (type == "tuple[]") {
            input["components"] = getArrayValueTuple(paramSet, static_cast<int>(i), info);
        } else if (boost::algorithm::ends_with(type.get<string>(), "[]")) {
            input["value"] = json(getArrayValue(paramSet, type, static_cast<int>(i)));
        } else if (type == "tuple") {
            input["components"] = getTupleValue(paramSet, type, static_cast<int>(i), info);
        } else if (type == "bool") {
            input["value"] = getValue(paramSet, type, static_cast<int>(i)) == "true" ? json(true) : json(false);
        } else {
            input["value"] = getValue(paramSet, type, static_cast<int>(i));
        }
        inputs.push_back(input);
    }
    return inputs;
}

void fillTuple(ParamSet& paramSet, const json& jsonSet); // forward

void fillTupleArray(ParamSet& paramSet, const json& jsonSet); // forward

void decodeParamSet(ParamSet& paramSet, const json& jsonSet) {
    for (auto& comp : jsonSet) {
        if (comp["type"] == "tuple") {
            fillTuple(paramSet, comp["components"]);
        } else if (comp["type"] == "tuple[]") {
            fillTupleArray(paramSet, comp["components"]);
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

void fillTupleArray(ParamSet& paramSet, const json& jsonSet) {
    std::shared_ptr<ParamTuple> tuple = make_shared<ParamTuple>();
    decodeParamSet(tuple->_params, jsonSet);

    auto tupleArray = make_shared<ParamArray>(tuple);
    paramSet.addParam(tupleArray);
}

optional<string> decodeCall(const Data& call, const std::string& abi) {
    EthereumAbi::Proto::ContractCallDecodingInput input;
    input.set_encoded(call.data(), call.size());
    input.set_smart_contract_abi_json(abi);

    Rust::TWDataWrapper inputData(data(input.SerializeAsString()));
    Rust::TWDataWrapper outputPtr = Rust::tw_ethereum_abi_decode_contract_call(TWCoinTypeEthereum, inputData.get());

    auto outputData = outputPtr.toDataOrDefault();
    if (outputData.empty()) {
        return {};
    }

    EthereumAbi::Proto::ContractCallDecodingOutput output;
    output.ParseFromArray(outputData.data(), static_cast<int>(outputData.size()));

    if (output.error() != EthereumAbi::Proto::AbiError::OK) {
        return {};
    }

    return output.decoded_json();
}

} // namespace TW::Ethereum::ABI
