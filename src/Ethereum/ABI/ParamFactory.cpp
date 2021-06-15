// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "ParamFactory.h"
#include "ParamAddress.h"
#include "HexCoding.h"

#include <nlohmann/json.hpp>
#include <boost/algorithm/string/predicate.hpp>

using namespace std;
using namespace boost::algorithm;
using json = nlohmann::json;

namespace TW::Ethereum::ABI {

static int parseBitSize(const std::string& type) {
    int size = stoi(type);
    if (size < 8 || size > 256 || size % 8 != 0 ||
        size == 8 || size == 16 || size == 32 || size == 64 || size == 256) {
        throw invalid_argument("invalid bit size");
    }
    return size;
}

static std::shared_ptr<ParamBase> makeUInt(const std::string& type) {
    auto bits = parseBitSize(type);
    return make_shared<ParamUIntN>(bits);
}

static std::shared_ptr<ParamBase> makeInt(const std::string& type) {
    auto bits = parseBitSize(type);
    return make_shared<ParamIntN>(bits);
}

static bool isArrayType(const std::string& type) {
    return ends_with(type, "[]") && type.length() >= 3;
}

static std::string getArrayElemType(const std::string& arrayType) {
    if (ends_with(arrayType, "[]") && arrayType.length() >= 3) {
        return arrayType.substr(0, arrayType.length() - 2);
    }
    return "";
}

std::shared_ptr<ParamBase> ParamFactory::make(const std::string& type) {
    shared_ptr<ParamBase> param;
    if (isArrayType(type)) {
        auto elemType = getArrayElemType(type);
        auto elemParam = make(elemType);
        if (!elemParam) {
            return param;
        }
        param = make_shared<ParamArray>(elemParam);
    } else if (type == "address") {
        param = make_shared<ParamAddress>();
    } else if (type == "uint8") {
        param = make_shared<ParamUInt8>();
    } else if (type == "uint16") {
        param = make_shared<ParamUInt16>();
    } else if (type == "uint32") {
        param = make_shared<ParamUInt32>();
    } else if (type == "uint64") {
        param = make_shared<ParamUInt64>();
    } else if (type == "uint256" || type == "uint") {
        param = make_shared<ParamUInt256>();
    } else if (type == "int8") {
        param = make_shared<ParamInt8>();
    } else if (type == "int16") {
        param = make_shared<ParamInt16>();
    } else if (type == "int32") {
        param = make_shared<ParamInt32>();
    } else if (type == "int64") {
        param = make_shared<ParamInt64>();
    } else if (type == "int256" || type == "int") {
        param = make_shared<ParamInt256>();
    } else if (starts_with(type, "uint")) {
        param = makeUInt(type.substr(4, type.size() - 1));
    } else if (starts_with(type, "int")) {
        param = makeInt(type.substr(3, type.size() - 1));
    } else if (type == "bool") {
        param = make_shared<ParamBool>();
    } else if (type == "bytes") {
        param = make_shared<ParamByteArray>();
    } else if (starts_with(type, "bytes")) {
        auto bits = stoi(type.substr(5, type.size() - 1));
        param = make_shared<ParamByteArrayFix>(bits);
    } else if (type == "string") {
        param = make_shared<ParamString>();
    }
    return param;
}

std::string joinArrayElems(const std::vector<std::string>& strings) {
    auto array = json::array();
    for (auto i = 0; i < strings.size(); ++i) {
        // parse to prevent quotes on simple values
        auto value = json::parse(strings[i], nullptr, false);
        if (value.is_discarded()) {
            // fallback
            value = json(strings[i]);
        }
        array.push_back(value);
    }
    return array.dump();
}

std::shared_ptr<ParamNamed> ParamFactory::makeNamed(const std::string& name, const std::string& type) {
    auto param = make(type);
    if (!param) {
        return nullptr;
    }
    return std::make_shared<ParamNamed>(name, param);
}

std::string ParamFactory::getValue(const std::shared_ptr<ParamBase>& param, const std::string& type) {
    std::string result = "";
    if (isArrayType(type)) {
        auto values = getArrayValue(param, type);
        result = joinArrayElems(values);
    } else if (type == "address") {
        auto value = dynamic_pointer_cast<ParamAddress>(param);
        result = hexEncoded(value->getData());
    } else if (type == "uint8") {
        result = boost::lexical_cast<std::string>((uint)dynamic_pointer_cast<ParamUInt8>(param)->getVal());
    } else if (type == "uint16") {
        result = boost::lexical_cast<std::string>(dynamic_pointer_cast<ParamUInt16>(param)->getVal());
    } else if (type == "uint32") {
        result = boost::lexical_cast<std::string>(dynamic_pointer_cast<ParamUInt32>(param)->getVal());
    } else if (type == "uint64") {
        result = boost::lexical_cast<std::string>(dynamic_pointer_cast<ParamUInt64>(param)->getVal());
    } else if (type == "uint256" || type == "uint") {
        result = boost::lexical_cast<std::string>(dynamic_pointer_cast<ParamUInt256>(param)->getVal());
    } else if (type == "int8") {
        result = boost::lexical_cast<std::string>((int)dynamic_pointer_cast<ParamInt8>(param)->getVal());
    } else if (type == "int16") {
        result = boost::lexical_cast<std::string>(dynamic_pointer_cast<ParamInt16>(param)->getVal());
    } else if (type == "int32") {
        result = boost::lexical_cast<std::string>(dynamic_pointer_cast<ParamInt32>(param)->getVal());
    } else if (type == "int64") {
        result = boost::lexical_cast<std::string>(dynamic_pointer_cast<ParamInt64>(param)->getVal());
    } else if (type == "int256" || type == "int") {
        result = boost::lexical_cast<std::string>(dynamic_pointer_cast<ParamInt256>(param)->getVal());
    } else if (starts_with(type, "uint")) {
        auto value = dynamic_pointer_cast<ParamUIntN>(param);
        result = boost::lexical_cast<std::string>(value->getVal());
    } else if (starts_with(type, "int")) {
        auto value = dynamic_pointer_cast<ParamIntN>(param);
        result = boost::lexical_cast<std::string>(value->getVal());
    } else if (type == "bool") {
        auto value = dynamic_pointer_cast<ParamBool>(param);
        result = value->getVal() ? "true" : "false";
    } else if (type == "bytes") {
        auto value = dynamic_pointer_cast<ParamByteArray>(param);
        result = hexEncoded(value->getVal());
    } else if (starts_with(type, "bytes")) {
        auto value = dynamic_pointer_cast<ParamByteArrayFix>(param);
        result = hexEncoded(value->getVal());
    } else if (type == "string") {
        auto value = dynamic_pointer_cast<ParamString>(param);
        result = value->getVal();
    }
    return result;
}

std::vector<std::string> ParamFactory::getArrayValue(const std::shared_ptr<ParamBase>& param, const std::string& type) {
    if (!isArrayType(type)) {
        return std::vector<std::string>();
    }
    auto array = dynamic_pointer_cast<ParamArray>(param);
    if (!array) {
        return std::vector<std::string>();
    }
    auto elemType = getArrayElemType(type);
    auto elems = array->getVal();
    std::vector<std::string> values(elems.size());
    for (auto i = 0; i < elems.size(); ++i) {
        values[i] = getValue(elems[i], elemType);
    }
    return values;
}

std::shared_ptr<ParamStruct> findType(const std::string& typeName, const std::vector<std::shared_ptr<ParamStruct>>& types) {
    for (auto& t: types) {
        if (t->getType() == typeName) {
            return t;
        }
    }
    return nullptr;
}

std::shared_ptr<ParamStruct> ParamFactory::makeStruct(const std::string& structType, const std::string& valueJson, const std::string& typesJson) {
    try {
        // parse types
        auto types = makeTypes(typesJson);
        // find type info
        auto typeInfo = findType(structType, types);
        if (!typeInfo) {
            throw std::invalid_argument("Type not found, " + structType);
        }
        auto values = json::parse(valueJson, nullptr, false);
        if (values.is_discarded()) {
            throw std::invalid_argument("Could not parse value Json");
        }
        if (!values.is_object()) {
            throw std::invalid_argument("Expecting object");
        }
        std::vector<std::shared_ptr<ParamNamed>> params;
        for (json::iterator it = values.begin(); it != values.end(); ++it) {
            std::string key = it.key();
            auto paramType = typeInfo->findParamByName(key);
            if (paramType) {
                const std::string paramTypeString = paramType->getParam()->getType();
                auto paramVal = make(paramTypeString);
                if (paramVal) {
                    // TODO set value
                    // TODO handle sub structs
                    if (paramTypeString == "string") {
                        std::dynamic_pointer_cast<ParamString>(paramVal)->setVal(it.value().get<std::string>());
                    } else if (paramTypeString == "address") {
                        std::dynamic_pointer_cast<ParamAddress>(paramVal)->setVal(TW::load(parse_hex(it.value().get<std::string>())));
                    }
                    auto paramNamed = std::make_shared<ParamNamed>(key, paramVal);
                    params.push_back(paramNamed);
                }
            }
        }
        return std::make_shared<ParamStruct>(structType, params);
    } catch (const std::invalid_argument& ex) {
        throw;
    } catch (...) {
        throw std::invalid_argument("Could not process Json");
    }
}

std::shared_ptr<ParamStruct> ParamFactory::makeType(const std::string& structType, const std::vector<std::shared_ptr<ParamStruct>>& extraTypes) {
    try {
        auto jsonValue = json::parse(structType, nullptr, false);
        if (jsonValue.is_discarded()) {
            throw std::invalid_argument("Could not parse type Json");
        }
        std::vector<std::shared_ptr<ParamNamed>> params;
        std::string structName;
        for (json::iterator it1 = jsonValue.begin(); it1 != jsonValue.end(); ++it1) {
            structName = it1.key();
            if (!it1.value().is_array()) {
                throw std::invalid_argument("Expecting array, " + structName);
            }
            for(auto& p2: it1.value()) {
                auto name = p2["name"].get<std::string>();
                auto type = p2["type"].get<std::string>();
                if (name.empty() || type.empty()) {
                    throw std::invalid_argument("Expecting 'name' and 'type', in " + structName);
                }
                auto named = makeNamed(name, type);
                if (named) {
                    // simple type
                    params.push_back(named);
                } else {
                    // try struct from extra types
                    auto p2struct = findType(type, extraTypes);
                    if (!p2struct) {
                        throw std::invalid_argument("Unknown type " + type);
                    }
                    params.push_back(std::make_shared<ParamNamed>(name, p2struct));
                }
            }
            break;
        }
        if (params.size() == 0) {
            throw std::invalid_argument("No valid params found");
        }
        return std::make_shared<ParamStruct>(structName, params);
    } catch (const std::invalid_argument& ex) {
        throw;
    } catch (...) {
        throw std::invalid_argument("Could not process Json");
    }
}

std::vector<std::shared_ptr<ParamStruct>> ParamFactory::makeTypes(const std::string& structTypes) {
    try {
        std::vector<std::shared_ptr<ParamStruct>> types;
        auto jsonValue = json::parse(structTypes, nullptr, false);
        if (jsonValue.is_discarded()) {
            throw std::invalid_argument("Could not parse types Json");
        }
        if (!jsonValue.is_array()) {
            throw std::invalid_argument("Expecting array");
        }
        for (auto& t: jsonValue) {
            // may throw
            auto struct1 = makeType(t.dump(), types);
            types.push_back(struct1);
        }
        return types;
    } catch (std::exception& ex) {
        throw;
    } catch (...) {
        throw std::invalid_argument("Could not process Json");
    }
}

} // namespace TW::Ethereum::ABI
