// Copyright © 2017-2023 Trust Wallet.
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

#include "Wasm.h"

namespace TW::Ethereum::ABI {

/// Factory creates concrete ParamBase class from string type.
class ParamFactory {
public:
    /// Create a param of given type
    static std::shared_ptr<ParamBase> make(const std::string& type);
    /// Create a named param, with given name and type
    static std::shared_ptr<ParamNamed> makeNamed(const std::string& name, const std::string& type);
    /// Check if given type is a primitive type
    static bool isPrimitive(const std::string& type);

    static std::string getValue(const std::shared_ptr<ParamBase>& param, const std::string& type);
    static std::vector<std::string> getArrayValue(const std::shared_ptr<ParamBase>& param,
                                                  const std::string& type);
};

} // namespace TW::Ethereum::ABI
