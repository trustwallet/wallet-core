// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <TrustWalletCore/TWEthereumAbiFunction.h>

#include "Ethereum/ABI/Function.h"
#include "Data.h"
#include "HexCoding.h"

#include <vector>
#include <cassert>

using namespace TW;
namespace EthAbi = TW::Ethereum::ABI;

struct TWEthereumAbiFunction *_Nonnull TWEthereumAbiFunctionCreateWithString(TWString *_Nonnull name) {
    auto func = EthAbi::Function(TWStringUTF8Bytes(name));
    return new TWEthereumAbiFunction{ func };
}

void TWEthereumAbiFunctionDelete(struct TWEthereumAbiFunction *_Nonnull func_in) {
    assert(func_in != nullptr);
    delete func_in;
}

TWString *_Nonnull TWEthereumAbiFunctionGetType(struct TWEthereumAbiFunction *_Nonnull func_in) {
    assert(func_in != nullptr);
    std::string sign = func_in->impl.getType();
    return TWStringCreateWithUTF8Bytes(sign.c_str());
}

///// AddParam

int TWEthereumAbiFunctionAddParamUInt8(struct TWEthereumAbiFunction *_Nonnull func_in, uint8_t val, bool isOutput) {
    assert(func_in != nullptr);
    uint32_t bits = 8;
    auto encodedValue = store(val);
    return func_in->impl.addUintParam(bits, encodedValue, isOutput);
}

int TWEthereumAbiFunctionAddParamUInt16(struct TWEthereumAbiFunction *_Nonnull func_in, uint16_t val, bool isOutput) {
    assert(func_in != nullptr);
    uint32_t bits = 16;
    auto encodedValue = store(val);
    return func_in->impl.addUintParam(bits, encodedValue, isOutput);
}

int TWEthereumAbiFunctionAddParamUInt32(struct TWEthereumAbiFunction *_Nonnull func_in, uint32_t val, bool isOutput) {
    assert(func_in != nullptr);
    uint32_t bits = 32;
    auto encodedValue = store(val);
    return func_in->impl.addUintParam(bits, encodedValue, isOutput);
}

int TWEthereumAbiFunctionAddParamUInt64(struct TWEthereumAbiFunction *_Nonnull func_in, uint64_t val, bool isOutput) {
    assert(func_in != nullptr);
    uint32_t bits = 64;
    auto encodedValue = store(val);
    return func_in->impl.addUintParam(bits, encodedValue, isOutput);
}

int TWEthereumAbiFunctionAddParamUInt256(struct TWEthereumAbiFunction *_Nonnull func_in, TWData *_Nonnull val, bool isOutput) {
    assert(func_in != nullptr);
    uint32_t bits = 256;
    const Data& encodedValue = *(reinterpret_cast<const Data*>(val));
    return func_in->impl.addUintParam(bits, encodedValue, isOutput);
}

int TWEthereumAbiFunctionAddParamUIntN(struct TWEthereumAbiFunction *_Nonnull func_in, int bits, TWData *_Nonnull val, bool isOutput) {
    assert(func_in != nullptr);
    const Data& encodedValue = *(reinterpret_cast<const Data*>(val));
    return func_in->impl.addUintParam(static_cast<uint32_t>(bits), encodedValue, isOutput);
}

int TWEthereumAbiFunctionAddParamInt8(struct TWEthereumAbiFunction *_Nonnull func_in, int8_t val, bool isOutput) {
    assert(func_in != nullptr);
    uint32_t bits = 8;
    auto encodedValue = store(val);
    return func_in->impl.addIntParam(bits, encodedValue, isOutput);
}

int TWEthereumAbiFunctionAddParamInt16(struct TWEthereumAbiFunction *_Nonnull func_in, int16_t val, bool isOutput) {
    assert(func_in != nullptr);
    uint32_t bits = 16;
    auto encodedValue = store(val);
    return func_in->impl.addIntParam(bits, encodedValue, isOutput);
}

int TWEthereumAbiFunctionAddParamInt32(struct TWEthereumAbiFunction *_Nonnull func_in, int32_t val, bool isOutput) {
    assert(func_in != nullptr);
    uint32_t bits = 32;
    auto encodedValue = store(val);
    return func_in->impl.addIntParam(bits, encodedValue, isOutput);
}

int TWEthereumAbiFunctionAddParamInt64(struct TWEthereumAbiFunction *_Nonnull func_in, int64_t val, bool isOutput) {
    assert(func_in != nullptr);
    uint32_t bits = 64;
    auto encodedValue = store(val);
    return func_in->impl.addIntParam(bits, encodedValue, isOutput);
}

int TWEthereumAbiFunctionAddParamInt256(struct TWEthereumAbiFunction *_Nonnull func_in, TWData *_Nonnull val, bool isOutput) {
    assert(func_in != nullptr);
    uint32_t bits = 256;
    const Data& encodedValue = *(reinterpret_cast<const Data*>(val));
    return func_in->impl.addIntParam(bits, encodedValue, isOutput);
}

int TWEthereumAbiFunctionAddParamIntN(struct TWEthereumAbiFunction *_Nonnull func_in, int bits, TWData *_Nonnull val, bool isOutput) {
    assert(func_in != nullptr);
    const Data& encodedValue = *(reinterpret_cast<const Data*>(val));
    return func_in->impl.addIntParam(static_cast<uint32_t>(bits), encodedValue, isOutput);
}

int TWEthereumAbiFunctionAddParamBool(struct TWEthereumAbiFunction *_Nonnull func_in, bool val, bool isOutput) {
    assert(func_in != nullptr);

    EthereumAbi::Proto::Param paramType;
    // Declare the `boolean` type.
    paramType.mutable_param()->mutable_boolean();

    EthereumAbi::Proto::Token token;
    token.set_boolean(val);

    return func_in->impl.addParam(std::move(paramType), std::move(token), isOutput);
}

int TWEthereumAbiFunctionAddParamString(struct TWEthereumAbiFunction *_Nonnull func_in, TWString *_Nonnull val, bool isOutput) {
    assert(func_in != nullptr);

    EthereumAbi::Proto::Param paramType;
    // Declare the `string` type.
    paramType.mutable_param()->mutable_string_param();

    EthereumAbi::Proto::Token token;
    auto* s = reinterpret_cast<const std::string*>(val);
    token.set_string_value(*s);

    return func_in->impl.addParam(std::move(paramType), std::move(token), isOutput);
}

int TWEthereumAbiFunctionAddParamAddress(struct TWEthereumAbiFunction *_Nonnull func_in, TWData *_Nonnull val, bool isOutput) {
    assert(func_in != nullptr);

    EthereumAbi::Proto::Param paramType;
    // Declare the `address` type.
    paramType.mutable_param()->mutable_address();

    EthereumAbi::Proto::Token token;
    const Data& addressData = *(reinterpret_cast<const Data*>(val));
    bool prefixed = true;
    auto addressStr = hex(addressData, prefixed);
    token.set_address(addressStr);

    return func_in->impl.addParam(std::move(paramType), std::move(token), isOutput);
}

int TWEthereumAbiFunctionAddParamBytes(struct TWEthereumAbiFunction *_Nonnull func_in, TWData *_Nonnull val, bool isOutput) {
    assert(func_in != nullptr);

    EthereumAbi::Proto::Param paramType;
    // Declare the `byte_array` type.
    paramType.mutable_param()->mutable_byte_array();

    EthereumAbi::Proto::Token token;
    const Data& bytesData = *(reinterpret_cast<const Data*>(val));
    token.set_byte_array(bytesData.data(), bytesData.size());

    return func_in->impl.addParam(std::move(paramType), std::move(token), isOutput);
}

int TWEthereumAbiFunctionAddParamBytesFix(struct TWEthereumAbiFunction *_Nonnull func_in, size_t count, TWData *_Nonnull val, bool isOutput) {
    assert(func_in != nullptr);

    EthereumAbi::Proto::Param paramType;
    // Declare the `byte_array_fix` type.
    paramType.mutable_param()->mutable_byte_array_fix()->set_size(static_cast<uint64_t>(count));

    EthereumAbi::Proto::Token token;
    Data bytesData = *(reinterpret_cast<const Data*>(val));
    bytesData.resize(count);
    token.set_byte_array_fix(bytesData.data(), bytesData.size());

    return func_in->impl.addParam(std::move(paramType), std::move(token), isOutput);
}

int TWEthereumAbiFunctionAddParamArray(struct TWEthereumAbiFunction *_Nonnull func_in, bool isOutput) {
    assert(func_in != nullptr);

    EthereumAbi::Proto::Param paramType;
    // Declare the `array` type.
    paramType.mutable_param()->mutable_array();

    EthereumAbi::Proto::Token token;
    // Declare the `array` empty value.
    token.mutable_array();

    return func_in->impl.addParam(std::move(paramType), std::move(token), isOutput);
}

///// GetParam

uint8_t TWEthereumAbiFunctionGetParamUInt8(struct TWEthereumAbiFunction *_Nonnull func_in, int idx, bool isOutput) {
    assert(func_in != nullptr);
    return func_in->impl.getUintParam<uint8_t>(idx, 8, isOutput);
}

uint64_t TWEthereumAbiFunctionGetParamUInt64(struct TWEthereumAbiFunction *_Nonnull func_in, int idx, bool isOutput) {
    assert(func_in != nullptr);
    return func_in->impl.getUintParam<uint64_t>(idx, 64, isOutput);
}

TWData *_Nonnull TWEthereumAbiFunctionGetParamUInt256(struct TWEthereumAbiFunction *_Nonnull func_in, int idx, bool isOutput) {
    assert(func_in != nullptr);
    auto valData = func_in->impl.getUintParamData(idx, 256, isOutput);
    return TWDataCreateWithData(&valData);
}

bool TWEthereumAbiFunctionGetParamBool(struct TWEthereumAbiFunction *_Nonnull func_in, int idx, bool isOutput) {
    assert(func_in != nullptr);
    auto param = func_in->impl.getParam(idx, isOutput);
    if (!param.has_value() || !param->has_boolean()) {
        return false;
    }
    return param->boolean();
}

TWString *_Nonnull TWEthereumAbiFunctionGetParamString(struct TWEthereumAbiFunction *_Nonnull func_in, int idx, bool isOutput) {
    assert(func_in != nullptr);
    std::string valStr;

    auto param = func_in->impl.getParam(idx, isOutput);
    if (!param.has_value() || !param->has_string_value()) {
        return TWStringCreateWithUTF8Bytes(valStr.c_str());
    }
    valStr = param->string_value();
    return TWStringCreateWithUTF8Bytes(valStr.c_str());
}

TWData *_Nonnull TWEthereumAbiFunctionGetParamAddress(struct TWEthereumAbiFunction *_Nonnull func_in, int idx, bool isOutput) {
    assert(func_in != nullptr);
    Data addressData;

    auto param = func_in->impl.getParam(idx, isOutput);
    if (!param.has_value() || !param->has_address()) {
        return TWDataCreateWithData(&addressData);
    }
    auto addressStr = param->address();
    try {
        addressData = parse_hex(addressStr);
        return TWDataCreateWithData(&addressData);
    } catch (...) {
        return TWDataCreateWithData(&addressData);
    }
}

///// AddInArrayParam

int TWEthereumAbiFunctionAddInArrayParamUInt8(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, uint8_t val) {
    assert(func_in != nullptr);
    auto encodedVal = store(val);
    return func_in->impl.addInArrayUintParam(arrayIdx, 8, encodedVal);
}

int TWEthereumAbiFunctionAddInArrayParamUInt16(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, uint16_t val) {
    assert(func_in != nullptr);
    auto encodedVal = store(val);
    return func_in->impl.addInArrayUintParam(arrayIdx, 16, encodedVal);
}

int TWEthereumAbiFunctionAddInArrayParamUInt32(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, uint32_t val) {
    assert(func_in != nullptr);
    auto encodedVal = store(val);
    return func_in->impl.addInArrayUintParam(arrayIdx, 32, encodedVal);
}

int TWEthereumAbiFunctionAddInArrayParamUInt64(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, uint64_t val) {
    assert(func_in != nullptr);
    auto encodedVal = store(val);
    return func_in->impl.addInArrayUintParam(arrayIdx, 64, encodedVal);
}

int TWEthereumAbiFunctionAddInArrayParamUInt256(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, TWData *_Nonnull val) {
    assert(func_in != nullptr);
    const Data& bytesData = *(reinterpret_cast<const Data*>(val));
    return func_in->impl.addInArrayUintParam(arrayIdx, 256, bytesData);
}

int TWEthereumAbiFunctionAddInArrayParamUIntN(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, int bits, TWData *_Nonnull val) {
    assert(func_in != nullptr);
    const Data& bytesData = *(reinterpret_cast<const Data*>(val));
    return func_in->impl.addInArrayUintParam(arrayIdx, bits, bytesData);
}

int TWEthereumAbiFunctionAddInArrayParamInt8(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, int8_t val) {
    assert(func_in != nullptr);
    auto encodedVal = store(val);
    return func_in->impl.addInArrayIntParam(arrayIdx, 8, encodedVal);
}

int TWEthereumAbiFunctionAddInArrayParamInt16(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, int16_t val) {
    assert(func_in != nullptr);
    auto encodedVal = store(val);
    return func_in->impl.addInArrayIntParam(arrayIdx, 16, encodedVal);
}

int TWEthereumAbiFunctionAddInArrayParamInt32(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, int32_t val) {
    assert(func_in != nullptr);
    auto encodedVal = store(val);
    return func_in->impl.addInArrayIntParam(arrayIdx, 32, encodedVal);
}

int TWEthereumAbiFunctionAddInArrayParamInt64(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, int64_t val) {
    assert(func_in != nullptr);
    auto encodedVal = store(val);
    return func_in->impl.addInArrayIntParam(arrayIdx, 64, encodedVal);
}

int TWEthereumAbiFunctionAddInArrayParamInt256(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, TWData *_Nonnull val) {
    assert(func_in != nullptr);
    const Data& bytesData = *(reinterpret_cast<const Data*>(val));
    return func_in->impl.addInArrayIntParam(arrayIdx, 256, bytesData);
}

int TWEthereumAbiFunctionAddInArrayParamIntN(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, int bits, TWData *_Nonnull val) {
    assert(func_in != nullptr);
    const Data& bytesData = *(reinterpret_cast<const Data*>(val));
    return func_in->impl.addInArrayIntParam(arrayIdx, bits, bytesData);
}

int TWEthereumAbiFunctionAddInArrayParamBool(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, bool val) {
    assert(func_in != nullptr);

    EthereumAbi::Proto::ParamType paramType;
    // Declare the boolean type.
    paramType.mutable_boolean();

    EthereumAbi::Proto::Token token;
    token.set_boolean(val);

    return func_in->impl.addInArrayParam(arrayIdx, std::move(paramType), std::move(token));
}

int TWEthereumAbiFunctionAddInArrayParamString(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, TWString *_Nonnull val) {
    assert(func_in != nullptr);

    EthereumAbi::Proto::ParamType paramType;
    // Declare the boolean type.
    paramType.mutable_string_param();

    EthereumAbi::Proto::Token token;
    token.set_string_value(TWStringUTF8Bytes(val));

    return func_in->impl.addInArrayParam(arrayIdx, std::move(paramType), std::move(token));
}

int TWEthereumAbiFunctionAddInArrayParamAddress(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, TWData *_Nonnull val) {
    assert(func_in != nullptr);

    EthereumAbi::Proto::ParamType paramType;
    // Declare the boolean type.
    paramType.mutable_address();

    EthereumAbi::Proto::Token token;
    const Data& addressData = *(reinterpret_cast<const Data*>(val));
    bool prefixed = true;
    auto addressStr = hex(addressData, prefixed);
    token.set_address(addressStr);

    return func_in->impl.addInArrayParam(arrayIdx, std::move(paramType), std::move(token));
}

int TWEthereumAbiFunctionAddInArrayParamBytes(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, TWData *_Nonnull val) {
    assert(func_in != nullptr);

    EthereumAbi::Proto::ParamType paramType;
    // Declare the boolean type.
    paramType.mutable_byte_array();

    EthereumAbi::Proto::Token token;
    const Data& bytesData = *(reinterpret_cast<const Data*>(val));
    token.set_byte_array(bytesData.data(), bytesData.size());

    return func_in->impl.addInArrayParam(arrayIdx, std::move(paramType), std::move(token));
}

int TWEthereumAbiFunctionAddInArrayParamBytesFix(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, size_t count, TWData *_Nonnull val) {
    assert(func_in != nullptr);

    EthereumAbi::Proto::ParamType paramType;
    // Declare the boolean type.
    paramType.mutable_byte_array_fix()->set_size(static_cast<uint64_t>(count));

    EthereumAbi::Proto::Token token;
    Data bytesData = *(reinterpret_cast<const Data*>(val));
    bytesData.resize(count);
    token.set_byte_array_fix(bytesData.data(), bytesData.size());

    return func_in->impl.addInArrayParam(arrayIdx, std::move(paramType), std::move(token));
}
