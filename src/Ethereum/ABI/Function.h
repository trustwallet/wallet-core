// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "ProtoParam.h"
#include "proto/EthereumAbi.pb.h"

#include "../../HexCoding.h"
#include "../../uint256.h"

#include <memory>
#include <optional>
#include <string>

namespace TW::Ethereum::ABI {

namespace AbiProto = EthereumAbi::Proto;

using MaybeToken = std::optional<AbiProto::Token>;
using MaybeData = std::optional<Data>;
using Tokens = std::vector<AbiProto::Token>;

class Function {
public:
    explicit Function(std::string name): name(std::move(name)) {}

    /// Adds an input or output parameter. Returns the index of the parameter.
    int addParam(AbiProto::Param paramType, AbiProto::Token paramValue, bool isOutput = false);

    /// Adds an input or output uint parameter. Returns the index of the parameter.
    int addUintParam(uint32_t bits, const Data& encodedValue, bool isOutput = false);

    /// Adds an input or output int parameter. Returns the index of the parameter.
    int addIntParam(uint32_t bits, const Data& encodedValue, bool isOutput = false);

    /// Adds a parameter to the input array.
    /// Please note the array should be present at `inputValues[idx]`.
    int addInArrayParam(int idx, AbiProto::ParamType paramType, AbiProto::Token paramValue);

    /// Adds a uint parameter to the input array.
    /// Please note the array should be present at `inputValues[idx]`.
    int addInArrayUintParam(int idx, uint32_t bits, const Data& encodedValue);

    /// Adds an int parameter to the input array.
    /// Please note the array should be present at `inputValues[idx]`.
    int addInArrayIntParam(int idx, uint32_t bits, const Data& encodedValue);

    /// Returns an input or output parameter.
    MaybeToken getParam(int idx, bool isOutput = false) const;

    /// Returns the data of an input or output uint parameter.
    Data getUintParamData(int idx, uint32_t bits, bool isOutput = false) const {
        auto param = getParam(idx, isOutput);
        if (!param.has_value() || !param->has_number_uint() || param->number_uint().bits() != bits) {
            return store(0);
        }
        return data(param->number_uint().value());
    }

    /// Returns an input or output uint parameter.
    template <typename T>
    T getUintParam(int idx, uint32_t bits, bool isOutput = false) const  {
        auto valueData = getUintParamData(idx, bits, isOutput);
        auto val256 = load(valueData);
        return static_cast<T>(val256);
    }

    /// Encodes a function call to Eth ABI binary.
    MaybeData encodeInput() const {
        return encodeFunctionCall(name, inputValues);
    }

    /// Decode binary, fill input or output parameters.
    bool decode(const Data& encoded, bool isOutput = false);

    /// Returns the function type signature, of the form "baz(int32,uint256)".
    std::string getType() const;

    /// Encodes a function call to Eth ABI binary.
    static MaybeData encodeFunctionCall(const std::string& functionName, const Tokens& params);

    /// Encodes a function call to Eth ABI binary.
    static MaybeData encodeFunctionCall(const std::string& functionName, const BaseParams& params);

    /// Encodes params to Eth ABI binary.
    static MaybeData encodeParams(const BaseParams& params);

private:
    std::string name;
    AbiProto::AbiParams inputs;
    AbiProto::AbiParams outputs;

    Tokens inputValues;
    Tokens outputValues;
};

} // namespace TW::Ethereum::ABI

/// Wrapper for C interface.
struct TWEthereumAbiFunction {
    TW::Ethereum::ABI::Function impl;
};
