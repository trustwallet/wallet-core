// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Function.h"

#include "rust/Wrapper.h"
#include "TrustWalletCore/TWCoinType.h"

namespace TW::Ethereum::ABI {

static constexpr std::size_t FUNCTION_SIGNATURE_LEN = 4;

int Function::addParam(AbiProto::Param paramType, AbiProto::Token paramValue, bool isOutput) {
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

int Function::addUintParam(uint32_t bits, const Data& encodedValue, bool isOutput) {
    AbiProto::Param paramType;
    paramType.mutable_param()->mutable_number_uint()->set_bits(bits);

    AbiProto::Token paramValue;
    auto* numValue = paramValue.mutable_number_uint();
    numValue->set_bits(bits);
    numValue->set_value(encodedValue.data(), encodedValue.size());

    return addParam(std::move(paramType), std::move(paramValue), isOutput);
}

int Function::addIntParam(uint32_t bits, const Data& encodedValue, bool isOutput) {
    AbiProto::Param paramType;
    paramType.mutable_param()->mutable_number_int()->set_bits(bits);

    AbiProto::Token paramValue;
    auto* numValue = paramValue.mutable_number_int();
    numValue->set_bits(bits);
    numValue->set_value(encodedValue.data(), encodedValue.size());

    return addParam(std::move(paramType), std::move(paramValue), isOutput);
}

int Function::addInArrayParam(int idx, AbiProto::ParamType paramType, AbiProto::Token paramValue) {
    if (idx < 0) {
        return -1;
    }

    auto idxSize = static_cast<std::size_t>(idx);
    if (idxSize >= inputValues.size()) {
        return -1;
    }

    auto& arrayToken = inputValues[idxSize];
    auto& arrayType = *inputs.mutable_params(idx)->mutable_param();

    if (!arrayToken.has_array() || !arrayType.has_array()) {
        return -1;
    }

    auto arrayInElementIdx = arrayToken.array().elements_size();

    *arrayToken.mutable_array()->add_elements() = std::move(paramValue);
    *arrayToken.mutable_array()->mutable_element_type() = paramType;
    // Override the element type.
    *arrayType.mutable_array()->mutable_element_type() = std::move(paramType);

    return arrayInElementIdx;
}

int Function::addInArrayUintParam(int idx, uint32_t bits, const Data& encodedValue) {
    AbiProto::ParamType paramType;
    paramType.mutable_number_uint()->set_bits(bits);

    AbiProto::Token paramValue;
    auto* numValue = paramValue.mutable_number_uint();
    numValue->set_bits(bits);
    numValue->set_value(encodedValue.data(), encodedValue.size());

    return addInArrayParam(idx, std::move(paramType), std::move(paramValue));
}

int Function::addInArrayIntParam(int idx, uint32_t bits, const Data& encodedValue) {
    AbiProto::ParamType paramType;
    paramType.mutable_number_int()->set_bits(bits);

    AbiProto::Token paramValue;
    auto* numValue = paramValue.mutable_number_int();
    numValue->set_bits(bits);
    numValue->set_value(encodedValue.data(), encodedValue.size());

    return addInArrayParam(idx, std::move(paramType), std::move(paramValue));
}

MaybeToken Function::getParam(int idx, bool isOutput) const {
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

bool Function::decode(const Data& encoded, bool isOutput) {
    AbiProto::ParamsDecodingInput input;

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

    AbiProto::ParamsDecodingOutput output;
    output.ParseFromArray(outputData.data(), static_cast<int>(outputData.size()));

    if (output.error() != AbiProto::AbiError::OK) {
        return false;
    }

    std::vector<AbiProto::Token> decoded;
    for (const auto &param : output.tokens()) {
        decoded.emplace_back(param);
    }

    if (isOutput) {
        outputValues = decoded;
    } else {
        inputValues = decoded;
    }
    return true;
}

std::string Function::getType() const {
    AbiProto::FunctionGetTypeInput input;
    input.set_function_name(name);
    *input.mutable_inputs() = inputs.params();

    Rust::TWDataWrapper inputData(data(input.SerializeAsString()));
    Rust::TWStringWrapper outputPtr = Rust::tw_ethereum_abi_function_get_signature(TWCoinTypeEthereum, inputData.get());

    return outputPtr.toStringOrDefault();
}

MaybeData Function::encodeFunctionCall(const std::string& functionName, const Tokens& tokens) {
    AbiProto::FunctionEncodingInput input;
    input.set_function_name(functionName);
    for (const auto& token : tokens) {
        *input.add_tokens() = token;
    }

    Rust::TWDataWrapper inputData(data(input.SerializeAsString()));
    Rust::TWDataWrapper outputPtr = Rust::tw_ethereum_abi_encode_function(TWCoinTypeEthereum, inputData.get());

    auto outputData = outputPtr.toDataOrDefault();
    if (outputData.empty()) {
        return {};
    }

    AbiProto::FunctionEncodingOutput output;
    output.ParseFromArray(outputData.data(), static_cast<int>(outputData.size()));

    if (output.error() != AbiProto::AbiError::OK) {
        return {};
    }

    return data(output.encoded());
}

MaybeData Function::encodeFunctionCall(const std::string& functionName, const BaseParams& params) {
    Tokens namedParams;
    for (const auto& param : params) {
        namedParams.push_back(param->toToken());
    }
    return encodeFunctionCall(functionName, namedParams);
}

MaybeData Function::encodeParams(const BaseParams& params) {
    auto encoded = encodeFunctionCall("", params);
    if (!encoded.has_value() || encoded.value().size() < FUNCTION_SIGNATURE_LEN) {
        return {};
    }

    // The encoded data includes the function call signature (4 bytes). Erase it.
    return subData(encoded.value(), FUNCTION_SIGNATURE_LEN);
}

} // namespace TW::Ethereum::ABI
