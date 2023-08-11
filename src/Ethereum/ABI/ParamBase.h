// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"

#include <string>
#include <memory>

namespace TW::Ethereum::ABI {

/// Abstract base class for parameters.
class ParamBase
{
public:
    virtual ~ParamBase() = default;
    virtual std::string getType() const = 0;
    virtual size_t getSize() const = 0;
    virtual bool isDynamic() const = 0;
    virtual void encode(Data& data) const = 0;
    virtual bool decode(const Data& encoded, size_t& offset_inout) = 0;
    virtual bool setValueJson(const std::string& value) = 0;
    // EIP712-style hash of the value (used for signing); default implementation
    virtual Data hashStruct() const;
    // Helper for EIP712 encoding; provide full type of all used types (recursively).  Default is empty implementation.
    virtual std::string getExtraTypes([[maybe_unused]] std::vector<std::string>& ignoreList) const { return ""; }
    // Creates a copy of this element.
    // This method **must** be implemented in a `final` class only.
    virtual std::shared_ptr<ParamBase> clone() const = 0;
};

/// Collection of parameters base class
class ParamCollection: public ParamBase
{
public:
    virtual size_t getCount() const = 0;
};

} // namespace TW::Ethereum::ABI
