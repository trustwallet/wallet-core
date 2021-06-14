// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Array.h"
#include "Bytes.h"
#include "ParamBase.h"
#include "ParamStruct.h"

#include <string>
#include <vector>

namespace TW::Ethereum::ABI {

/// Factory creates concrete ParamBase class from string type.
class ParamFactory
{
public:
    /// Create a param of given type
    static std::shared_ptr<ParamBase> make(const std::string& type);
    /// Create a named param, with given name and type
    static std::shared_ptr<ParamNamed> makeNamed(const std::string& name, const std::string& type);

    static std::string getValue(const std::shared_ptr<ParamBase>& param, const std::string& type);
    static std::vector<std::string> getArrayValue(const std::shared_ptr<ParamBase>& param, const std::string& type);

    /// Make a names struct, with the values from the values json, and type info from type (or types) info.
    static std::shared_ptr<ParamStruct> makeStruct(const std::string& structType, const std::string& valueJson, const std::string& typesJson);

    /// Parse a json with an array of types, and build a vector of named structs.  Structs params have the given name and type, and empty value.
    /// Ex. input: "[{\"Person\": [{\"name\": \"name\", \"type\": \"string\"}, {\"name\": \"wallet\", \"type\": \"address\" }]}, {\"Mail\": [{\"name\": \"from\", \"type\": \"Person\"}, {\"name\": \"to\", \"type\": \"Person\"}, {\"name\": \"contents\", \"type\": \"string\"}]}]"
    /// Order is important, if a type is references it must come first.
    /// Note the quote delimiters.
    /// On error empty is returned. 
    static std::vector<std::shared_ptr<ParamStruct>> makeTypes(const std::string& structTypes);

    /// Make a named struct, with the given types, with empty values.
    /// Similar to makeTypes, but works with only one type.
    /// Ex. input: "{\"Person\": [{\"name\": \"name\", \"type\": \"string\"}, {\"name\": \"wallet\", \"type\": \"address\" }]}"
    static std::shared_ptr<ParamStruct> makeType(const std::string& structType, const std::vector<std::shared_ptr<ParamStruct>>& extraTypes = {});
};

} // namespace TW::Ethereum::ABI
