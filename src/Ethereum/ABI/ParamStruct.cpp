// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "ParamStruct.h"
#include "ValueEncoder.h"
#include "ParamFactory.h"
#include <Hash.h>
#include <HexCoding.h>

#include <algorithm>
#include <nlohmann/json.hpp>

#include <cassert>
#include <string>

namespace TW::Ethereum::ABI {

using json = nlohmann::json;

static const Data EipStructPrefix = parse_hex("1901");
static const auto Eip712Domain = "EIP712Domain";

std::string ParamNamed::getType() const {
    return _param->getType() + " " + _name;
}

std::shared_ptr<ParamNamed> ParamNamed::cloneNamed() const {
    return std::make_shared<ParamNamed>(_name, _param->clone());
}

ParamSetNamed::~ParamSetNamed() {
    _params.clear();
}

/// Returns the index of the parameter
int ParamSetNamed::addParam(const std::shared_ptr<ParamNamed>& param) {
    if (param.get() == nullptr) {
        return -1;
    }
    assert(param.get() != nullptr);
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
    for (auto p : _params) {
        append(hashes, p->hashStruct());
    }
    return hashes;
}

std::string ParamSetNamed::getExtraTypes(std::vector<std::string>& ignoreList) const {
    std::string types;

    auto params = _params;
    /// referenced struct type should be sorted by name see: https://eips.ethereum.org/EIPS/eip-712#definition-of-encodetype
    std::stable_sort(params.begin(), params.end(), [](auto& a, auto& b) {
        auto lhs = a->getType();
        auto rhs = b->getType();
        if (ParamFactory::isPrimitive(lhs)) {
            return true;
        }
        if (ParamFactory::isPrimitive(rhs)) {
            return true;
        }
        return lhs.compare(rhs) < 0;
    });

    for (auto& p : params) {
        auto pType = p->_param->getType();
        if (std::find(ignoreList.begin(), ignoreList.end(), pType) == ignoreList.end()) {
            types += p->getExtraTypes(ignoreList);
            ignoreList.push_back(pType);
        }
    }
    return types;
}

std::shared_ptr<ParamNamed> ParamSetNamed::findParamByName(const std::string& name) const {
    for (auto& p : _params) {
        if (p->_name == name) {
            return p;
        }
    }
    return nullptr;
}

ParamSetNamed ParamSetNamed::clone() const {
    ParamSetNamed newSet;
    for (const auto& p : _params) {
        newSet.addParam(p->cloneNamed());
    }
    return newSet;
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

std::shared_ptr<ParamBase> ParamStruct::clone() const {
    return std::make_shared<ParamStruct>(_name, _params.clone());
}

Data ParamStruct::hashStructJson(const std::string& messageJson) {
    auto message = json::parse(messageJson, nullptr, false);
    if (message.is_discarded()) {
        throw std::invalid_argument("Could not parse Json");
    }
    if (!message.is_object()) {
        throw std::invalid_argument("Expecting Json object");
    }
    if (!message.contains("primaryType") || !message["primaryType"].is_string()) {
        throw std::invalid_argument("Top-level string field 'primaryType' missing");
    }
    if (!message.contains("domain") || !message["domain"].is_object()) {
        throw std::invalid_argument("Top-level object field 'domain' missing");
    }
    if (!message.contains("message") || !message["message"].is_object()) {
        throw std::invalid_argument("Top-level object field 'message' missing");
    }
    if (!message.contains("types") || !message["types"].is_object()) {
        throw std::invalid_argument("Top-level object field 'types' missing");
    }

    // concatenate hashes
    Data hashes = EipStructPrefix;

    auto domainStruct = makeStruct(
        Eip712Domain,
        message["domain"].dump(),
        message["types"].dump());
    if (domainStruct) {
        TW::append(hashes, domainStruct->hashStruct());

        auto messageStruct = makeStruct(
            message["primaryType"].get<std::string>(),
            message["message"].dump(),
            message["types"].dump());
        if (messageStruct) {
            const auto messageHash = messageStruct->hashStruct();
            TW::append(hashes, messageHash);
            return Hash::keccak256(hashes);
        }
    }
    return {}; // fallback
}

std::shared_ptr<ParamStruct> findType(const std::string& typeName, const std::vector<std::shared_ptr<ParamStruct>>& types) {
    for (auto& t : types) {
        if (t->getType() == typeName) {
            return t;
        }
    }
    return nullptr;
}

std::shared_ptr<ParamStruct> ParamStruct::makeStruct(const std::string& structType, const std::string& valueJson, const std::string& typesJson) {
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
        const auto& typeParams = typeInfo->getParams();
        // iterate through the type; order is important and field order in the value json is not defined
        for (auto i = 0ul; i < typeParams.getCount(); ++i) {
            auto name = typeParams.getParam(static_cast<int>(i))->getName();
            auto type = typeParams.getParam(static_cast<int>(i))->getParam()->getType();
            // look for it in value (may throw)
            auto value = values[name];
            // first try simple params
            auto paramVal = ParamFactory::make(type);
            if (paramVal) {
                if (!values.is_null()) {
                    std::string valueString = value.is_string() ? value.get<std::string>() : value.dump();
                    if (!paramVal->setValueJson(valueString)) {
                        throw std::invalid_argument("Could not set type for param " + name);
                    }
                }
                params.push_back(std::make_shared<ParamNamed>(name, paramVal));
            } else if (type.length() >= 2 && type.substr(type.length() - 2, 2) == "[]") {
                // array of struct
                auto arrayType = type.substr(0, type.length() - 2);
                auto subTypeInfo = findType(arrayType, types);
                if (!subTypeInfo) {
                    throw std::invalid_argument("Could not find type for array sub-struct " + arrayType);
                }
                if (!value.is_array()) {
                    throw std::invalid_argument("Value must be array for type " + type);
                }
                std::vector<std::shared_ptr<ParamBase>> paramsArray;
                if (value.size() == 0) {
                    // empty array
                    auto subStruct = makeStruct(arrayType, "{}", typesJson);
                    if (!subStruct) {
                        throw std::invalid_argument("Could not process array sub-struct " + arrayType + " " + "{}");
                    }
                    assert(subStruct);
                    auto tmp = std::make_shared<ParamArray>(paramsArray);
                    tmp->setProto(subStruct);
                    params.push_back(std::make_shared<ParamNamed>(name, tmp));
                } else {
                    for (const auto& e : value) {
                        auto subStruct = makeStruct(arrayType, e.dump(), typesJson);
                        if (!subStruct) {
                            throw std::invalid_argument("Could not process array sub-struct " + arrayType + " " + e.dump());
                        }
                        assert(subStruct);
                        paramsArray.push_back(subStruct);
                    }
                    params.push_back(std::make_shared<ParamNamed>(name, std::make_shared<ParamArray>(paramsArray)));
                }
            } else {
                // try if sub struct
                auto subTypeInfo = findType(type, types);
                if (!subTypeInfo) {
                    throw std::invalid_argument("Could not find type for sub-struct " + type);
                }
                if (value.is_null()) {
                    params.push_back(std::make_shared<ParamNamed>(name, std::make_shared<ParamStruct>(type, std::vector<std::shared_ptr<ParamNamed>>{})));
                } else {
                    auto subStruct = makeStruct(type, value.dump(), typesJson);
                    if (!subStruct) {
                        throw std::invalid_argument("Could not process sub-struct " + type);
                    }
                    assert(subStruct);
                    params.push_back(std::make_shared<ParamNamed>(name, subStruct));
                }
            }
        }
        return std::make_shared<ParamStruct>(structType, params);
    } catch (const std::invalid_argument& ex) {
        throw;
    } catch (const std::exception& ex) {
        throw std::invalid_argument(std::string("Could not process Json: ") + ex.what());
    } catch (...) {
        throw std::invalid_argument("Could not process Json");
    }
}

std::shared_ptr<ParamStruct> ParamStruct::makeType(const std::string& structName, const std::string& structJson, const std::vector<std::shared_ptr<ParamStruct>>& extraTypes, bool ignoreMissingType) {
    try {
        if (structName.empty()) {
            throw std::invalid_argument("Missing type name");
        }
        auto jsonValue = json::parse(structJson, nullptr, false);
        if (jsonValue.is_discarded()) {
            throw std::invalid_argument("Could not parse type Json");
        }
        if (!jsonValue.is_array()) {
            throw std::invalid_argument("Expecting array");
        }
        std::vector<std::shared_ptr<ParamNamed>> params;
        for (auto& p2 : jsonValue) {
            auto name = p2["name"].get<std::string>();
            auto type = p2["type"].get<std::string>();
            if (name.empty() || type.empty()) {
                throw std::invalid_argument("Expecting 'name' and 'type', in " + structName);
            }
            auto named = ParamFactory::makeNamed(name, type);
            if (named) {
                // simple type (incl. array of simple type)
                params.push_back(named);
            } else if (type == structName) {
                // recursive to self
                params.push_back(std::make_shared<ParamNamed>(name, std::make_shared<ParamStruct>(type, std::vector<std::shared_ptr<ParamNamed>>{})));
            } else if (type.length() >= 2 && type.substr(type.length() - 2, 2) == "[]") {
                // array of struct
                auto arrayType = type.substr(0, type.length() - 2);
                if (ignoreMissingType) {
                    params.push_back(std::make_shared<ParamNamed>(name, std::make_shared<ParamArray>(std::make_shared<ParamStruct>(arrayType, std::vector<std::shared_ptr<ParamNamed>>{}))));
                } else {
                    // try array struct from extra types
                    auto p2struct = findType(arrayType, extraTypes);
                    if (!p2struct) {
                        throw std::invalid_argument("Unknown struct array type " + arrayType);
                    }
                    params.push_back(std::make_shared<ParamNamed>(name, std::make_shared<ParamArray>(p2struct)));
                }
            } else {
                if (ignoreMissingType) {
                    params.push_back(std::make_shared<ParamNamed>(name, std::make_shared<ParamStruct>(type, std::vector<std::shared_ptr<ParamNamed>>{})));
                } else {
                    // try struct from extra types
                    auto p2struct = findType(type, extraTypes);
                    if (!p2struct) {
                        throw std::invalid_argument("Unknown type " + type);
                    }
                    params.push_back(std::make_shared<ParamNamed>(name, p2struct));
                }
            }
        }
        if (params.size() == 0) {
            throw std::invalid_argument("No valid params found");
        }
        return std::make_shared<ParamStruct>(structName, params);
    } catch (const std::invalid_argument& ex) {
        throw;
    } catch (const std::exception& ex) {
        throw std::invalid_argument(std::string("Could not process Json: ") + ex.what());
    } catch (...) {
        throw std::invalid_argument("Could not process Json");
    }
}

std::vector<std::shared_ptr<ParamStruct>> ParamStruct::makeTypes(const std::string& structTypes) {
    try {
        auto jsonValue = json::parse(structTypes, nullptr, false);
        if (jsonValue.is_discarded()) {
            throw std::invalid_argument("Could not parse types Json");
        }
        if (!jsonValue.is_object()) {
            throw std::invalid_argument("Expecting object");
        }
        // do it in 2 passes, as type order may be undefined
        std::vector<std::shared_ptr<ParamStruct>> types1;
        for (json::iterator it = jsonValue.begin(); it != jsonValue.end(); it++) {
            // may throw
            auto struct1 = makeType(it.key(), it.value().dump(), {}, true);
            types1.push_back(struct1);
        }
        std::vector<std::shared_ptr<ParamStruct>> types2;
        for (json::iterator it = jsonValue.begin(); it != jsonValue.end(); it++) {
            // may throw
            auto struct1 = makeType(it.key(), it.value().dump(), types1, false);
            types2.push_back(struct1);
        }
        return types2;
    } catch (const std::invalid_argument& ex) {
        throw;
    } catch (const std::exception& ex) {
        throw std::invalid_argument(std::string("Could not process Json: ") + ex.what());
    } catch (...) {
        throw std::invalid_argument("Could not process Json");
    }
}

} // namespace TW::Ethereum::ABI
