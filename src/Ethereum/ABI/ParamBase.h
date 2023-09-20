// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"

#include <map>
#include <memory>
#include <string>

namespace TW::Ethereum::ABI {

// A map of `StructName -> StructType` key-values, where `StructType` is a full type including the structure name.
// Referenced struct type should be sorted by name see: https://eips.ethereum.org/EIPS/eip-712#definition-of-encodetype
using ExtraTypesMap = std::map<std::string, std::string, std::less<>>;

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
    // Helper for EIP712 encoding; fill the given `extraTypes` (recursively).
    virtual void fillExtraTypesMap([[maybe_unused]] ExtraTypesMap& extraTypes) const {}
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
