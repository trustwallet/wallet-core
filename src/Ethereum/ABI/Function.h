// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "ParamBase.h"
#include "Parameters.h"
#include "Bytes.h"
#include "proto/EthereumAbi.pb.h"
#include "rust/Wrapper.h"
#include "TrustWalletCore/TWCoinType.h"

#include "../../uint256.h"
#include "../../Hash.h"

#include <string>

namespace TW::Ethereum::ABI {

class FunctionV2 {
public:
    std::string name;

    explicit FunctionV2(std::string name): name(std::move(name)) {}

    int addParam(EthereumAbi::Proto::NamedParamType paramType, EthereumAbi::Proto::NamedParam paramValue, bool isOutput = false) {
        if (isOutput) {
            auto idx = outputValues.size();
            *outputs.add_params() = std::move(paramType);
            outputValues.emplace_back(std::move(paramValue));
            return static_cast<int>(idx);
        }

        auto idx = inputValues.size();
        *inputs.add_params() = std::move(paramType);
        inputValues.emplace_back(std::move(paramValue));
        return static_cast<int>(idx);
    }

    int addUintParam(uint32_t bits, const Data& encodedValue, bool isOutput = false) {
        EthereumAbi::Proto::NamedParamType paramType;
        paramType.mutable_param()->mutable_number_uint()->set_bits(bits);

        EthereumAbi::Proto::NamedParam paramValue;
        auto* numValue = paramValue.mutable_value()->mutable_number_uint();
        numValue->set_bits(bits);
        numValue->set_value(encodedValue.data(), encodedValue.size());

        return addParam(std::move(paramType), std::move(paramValue), isOutput);
    }

    int addIntParam(uint32_t bits, const Data& encodedValue, bool isOutput = false) {
        EthereumAbi::Proto::NamedParamType paramType;
        paramType.mutable_param()->mutable_number_int()->set_bits(bits);

        EthereumAbi::Proto::NamedParam paramValue;
        auto* numValue = paramValue.mutable_value()->mutable_number_int();
        numValue->set_bits(bits);
        numValue->set_value(encodedValue.data(), encodedValue.size());

        return addParam(std::move(paramType), std::move(paramValue), isOutput);
    }

    int addInArrayParam(int idx, EthereumAbi::Proto::ParamType paramType, EthereumAbi::Proto::ParamValue paramValue) {
        if (idx < 0) {
            return -1;
        }

        auto idxSize = static_cast<std::size_t>(idx);
        if (idxSize >= inputValues.size()) {
            return -1;
        }

        auto& arrayValue = *inputValues[idxSize].mutable_value();
        auto& arrayType = *inputs.mutable_params(idx)->mutable_param();

        if (!arrayValue.has_array() || !arrayType.has_array()) {
            return -1;
        }

        auto arrayInElementIdx = arrayValue.array().values_size();

        *arrayValue.mutable_array()->add_values() = std::move(paramValue);
        // Override the element type.
        *arrayType.mutable_array()->mutable_element_type() = std::move(paramType);

        return arrayInElementIdx;
    }

    int addInArrayUintParam(int idx, uint32_t bits, const Data& encodedValue) {
        EthereumAbi::Proto::ParamType paramType;
        paramType.mutable_number_uint()->set_bits(bits);

        EthereumAbi::Proto::ParamValue paramValue;
        auto* numValue = paramValue.mutable_number_uint();
        numValue->set_bits(bits);
        numValue->set_value(encodedValue.data(), encodedValue.size());

        return addInArrayParam(idx, std::move(paramType), std::move(paramValue));
    }

    int addInArrayIntParam(int idx, uint32_t bits, const Data& encodedValue) {
        EthereumAbi::Proto::ParamType paramType;
        paramType.mutable_number_int()->set_bits(bits);

        EthereumAbi::Proto::ParamValue paramValue;
        auto* numValue = paramValue.mutable_number_int();
        numValue->set_bits(bits);
        numValue->set_value(encodedValue.data(), encodedValue.size());

        return addInArrayParam(idx, std::move(paramType), std::move(paramValue));
    }

    std::optional<EthereumAbi::Proto::NamedParam> getParam(int idx, bool isOutput = false) const {
        const auto& values = isOutput ? outputValues : inputValues;

        if (idx < 0) {
            return {};
        }

        auto idxSize = static_cast<std::size_t>(idx);
        if (idxSize >= values.size()) {
            return {};
        }

        return values[idxSize];
    }

    Data getUintParamData(int idx, uint32_t bits, bool isOutput) const {
        auto param = getParam(idx, isOutput);
        if (!param.has_value() || !param->value().has_number_uint() || param->value().number_uint().bits() != bits) {
            return store(0);
        }
        return data(param->value().number_uint().value());
    }

    template <typename T>
    T getUintParam(int idx, uint32_t bits, bool isOutput) const  {
        auto valueData = getUintParamData(idx, bits, isOutput);
        auto val256 = load(valueData);
        return static_cast<T>(val256);
    }

    bool decode(const Data& encoded, bool isOutput) {
        EthereumAbi::Proto::ParamsDecodingInput input;

        input.set_encoded(encoded.data(), encoded.size());
        if (isOutput) {
            *input.mutable_abi_params() = outputs;
        } else {
            *input.mutable_abi_params() = inputs;
        }

        Rust::TWDataWrapper inputData(data(input.SerializeAsString()));
        Rust::TWDataWrapper outputPtr = Rust::tw_ethereum_abi_decode_params(TWCoinTypeEthereum, inputData.get());

        auto outputData = outputPtr.toDataOrDefault();
        if (outputData.empty()) {
            return false;
        }

        EthereumAbi::Proto::ParamsDecodingOutput output;
        output.ParseFromArray(outputData.data(), static_cast<int>(outputData.size()));

        if (output.error() != Common::Proto::SigningError::OK) {
            return false;
        }

        std::vector<EthereumAbi::Proto::NamedParam> decoded;
        for (const auto &param : output.params()) {
            decoded.emplace_back(param);
        }

        if (isOutput) {
            outputValues = decoded;
        } else {
            inputValues = decoded;
        }
        return true;
    }

    std::string getType() const {
        EthereumAbi::Proto::FunctionGetTypeInput input;
        input.set_function_name(name);
        *input.mutable_inputs() = inputs.params();

        Rust::TWDataWrapper inputData(data(input.SerializeAsString()));
        Rust::TWStringWrapper outputPtr = Rust::tw_ethereum_abi_function_get_signature(TWCoinTypeEthereum, inputData.get());

        return outputPtr.toStringOrDefault();
    }

private:
    EthereumAbi::Proto::AbiParams inputs;
    EthereumAbi::Proto::AbiParams outputs;

    std::vector<EthereumAbi::Proto::NamedParam> inputValues;
    std::vector<EthereumAbi::Proto::NamedParam> outputValues;
};

/// Non-generic version of Function, templated version is impossible to pass around to and back over C interface
/// (void* looses the template parameters).
class Function {
public:
    std::string name;
    ParamSet _inParams;
    ParamSet _outParams;

    Function(std::string name) : name(std::move(name)) {}
    Function(std::string name, const std::vector<std::shared_ptr<ParamBase>>& inParams)
        : name(std::move(name)), _inParams(ParamSet(inParams)) {}
    virtual ~Function() {}
    /// Add an input parameter. Returns the index of the parameter.
    int addInParam(std::shared_ptr<ParamBase> param) {
        return _inParams.addParam(param);
    }
    /// Add an output parameter. Returns the index of the parameter.
    int addOutParam(std::shared_ptr<ParamBase> param) {
        return _outParams.addParam(param);
    }
    /// Add an input or output parameter. Returns the index of the parameter.
    int addParam(std::shared_ptr<ParamBase> param, bool isOutput = false) {
        return isOutput ? _outParams.addParam(param) : _inParams.addParam(param);
    }
    /// Get an input parameter.
    bool getInParam(int paramIndex, std::shared_ptr<ParamBase>& param_out) {
        return _inParams.getParam(paramIndex, param_out);
    }
    /// Get an output parameter.
    bool getOutParam(int paramIndex, std::shared_ptr<ParamBase>& param_out) {
        return _outParams.getParam(paramIndex, param_out);
    }
    /// Get an input or output parameter.
    bool getParam(int paramIndex, std::shared_ptr<ParamBase>& param_out, bool isOutput = false) {
        return isOutput ? _outParams.getParam(paramIndex, param_out) : _inParams.getParam(paramIndex, param_out);
    }
    /// Return the function type signature, of the form "baz(int32,uint256)"
    std::string getType() const {
        return name + _inParams.getType();
    }

    /// Return the 4-byte function signature
    Data getSignature() const;

    virtual void encode(Data& data) const;

    /// Decode binary, fill output parameters
    bool decodeOutput(const Data& encoded, size_t& offset_inout);
    /// Decode binary, fill input parameters
    bool decodeInput(const Data& encoded, size_t& offset_inout);
};

inline void encode(const Function& func, Data& data) {
    func.encode(data);
}

} // namespace TW::Ethereum::ABI

/// Wrapper for C interface.
struct TWEthereumAbiFunction {
    TW::Ethereum::ABI::Function impl;
    TW::Ethereum::ABI::FunctionV2 implV2;
};
