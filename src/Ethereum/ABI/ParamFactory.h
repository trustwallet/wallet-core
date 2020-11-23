// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Array.h"
#include "Bytes.h"
#include "ParamAddress.h"

#include <string>
#include <vector>

namespace TW::Ethereum::ABI {

/// Factory creates concrete ParamBase class from string type.
class ParamFactory
{
public:
    static std::shared_ptr<ParamBase> make(const std::string& type);
    static std::string getValue(const std::shared_ptr<ParamBase>& param, const std::string& type);
    static std::vector<std::string> getArrayValue(const std::shared_ptr<ParamBase>& param, const std::string& type);
};

} // namespace TW::Ethereum::ABI
