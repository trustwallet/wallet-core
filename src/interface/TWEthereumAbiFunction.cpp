// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWEthereumAbiFunction.h>

#include "Ethereum/ABI.h"
#include "Data.h"
#include "HexCoding.h"
#include "../uint256.h"

#include <string>
#include <vector>
#include <cassert>

using namespace TW;
namespace EthAbi = TW::Ethereum::ABI;

struct TWEthereumAbiFunction *_Nonnull TWEthereumAbiFunctionCreateWithString(TWString *_Nonnull name) {
    auto func = EthAbi::Function(TWStringUTF8Bytes(name));
    auto funcV2 = EthAbi::FunctionV2(TWStringUTF8Bytes(name));
    return new TWEthereumAbiFunction{ func, funcV2 };
}

void TWEthereumAbiFunctionDelete(struct TWEthereumAbiFunction *_Nonnull func_in) {
    assert(func_in != nullptr);
    delete func_in;
}

TWString *_Nonnull TWEthereumAbiFunctionGetType(struct TWEthereumAbiFunction *_Nonnull func_in) {
    assert(func_in != nullptr);
    std::string sign = func_in->implV2.getType();
    return TWStringCreateWithUTF8Bytes(sign.c_str());
}

///// AddParam

int TWEthereumAbiFunctionAddParamUInt8(struct TWEthereumAbiFunction *_Nonnull func_in, uint8_t val, bool isOutput) {
    assert(func_in != nullptr);
    uint32_t bits = 8;
    auto encodedValue = store(val);
    return func_in->implV2.addUintParam(bits, encodedValue, isOutput);
}

int TWEthereumAbiFunctionAddParamUInt16(struct TWEthereumAbiFunction *_Nonnull func_in, uint16_t val, bool isOutput) {
    assert(func_in != nullptr);
    uint32_t bits = 16;
    auto encodedValue = store(val);
    return func_in->implV2.addUintParam(bits, encodedValue, isOutput);
}

int TWEthereumAbiFunctionAddParamUInt32(struct TWEthereumAbiFunction *_Nonnull func_in, uint32_t val, bool isOutput) {
    assert(func_in != nullptr);
    uint32_t bits = 32;
    auto encodedValue = store(val);
    return func_in->implV2.addUintParam(bits, encodedValue, isOutput);
}

int TWEthereumAbiFunctionAddParamUInt64(struct TWEthereumAbiFunction *_Nonnull func_in, uint64_t val, bool isOutput) {
    assert(func_in != nullptr);
    uint32_t bits = 64;
    auto encodedValue = store(val);
    return func_in->implV2.addUintParam(bits, encodedValue, isOutput);
}

int TWEthereumAbiFunctionAddParamUInt256(struct TWEthereumAbiFunction *_Nonnull func_in, TWData *_Nonnull val, bool isOutput) {
    assert(func_in != nullptr);
    uint32_t bits = 256;
    const Data& encodedValue = *(reinterpret_cast<const Data*>(val));
    return func_in->implV2.addUintParam(bits, encodedValue, isOutput);
}

int TWEthereumAbiFunctionAddParamUIntN(struct TWEthereumAbiFunction *_Nonnull func_in, int bits, TWData *_Nonnull val, bool isOutput) {
    assert(func_in != nullptr);
    const Data& encodedValue = *(reinterpret_cast<const Data*>(val));
    return func_in->implV2.addUintParam(static_cast<uint32_t>(bits), encodedValue, isOutput);
}

int TWEthereumAbiFunctionAddParamInt8(struct TWEthereumAbiFunction *_Nonnull func_in, int8_t val, bool isOutput) {
    assert(func_in != nullptr);
    uint32_t bits = 8;
    auto encodedValue = store(val);
    return func_in->implV2.addIntParam(bits, encodedValue, isOutput);
}

int TWEthereumAbiFunctionAddParamInt16(struct TWEthereumAbiFunction *_Nonnull func_in, int16_t val, bool isOutput) {
    assert(func_in != nullptr);
    uint32_t bits = 16;
    auto encodedValue = store(val);
    return func_in->implV2.addIntParam(bits, encodedValue, isOutput);
}

int TWEthereumAbiFunctionAddParamInt32(struct TWEthereumAbiFunction *_Nonnull func_in, int32_t val, bool isOutput) {
    assert(func_in != nullptr);
    uint32_t bits = 32;
    auto encodedValue = store(val);
    return func_in->implV2.addIntParam(bits, encodedValue, isOutput);
}

int TWEthereumAbiFunctionAddParamInt64(struct TWEthereumAbiFunction *_Nonnull func_in, int64_t val, bool isOutput) {
    assert(func_in != nullptr);
    uint32_t bits = 64;
    auto encodedValue = store(val);
    return func_in->implV2.addIntParam(bits, encodedValue, isOutput);
}

int TWEthereumAbiFunctionAddParamInt256(struct TWEthereumAbiFunction *_Nonnull func_in, TWData *_Nonnull val, bool isOutput) {
    assert(func_in != nullptr);
    uint32_t bits = 256;
    const Data& encodedValue = *(reinterpret_cast<const Data*>(val));
    return func_in->implV2.addIntParam(bits, encodedValue, isOutput);
}

int TWEthereumAbiFunctionAddParamIntN(struct TWEthereumAbiFunction *_Nonnull func_in, int bits, TWData *_Nonnull val, bool isOutput) {
    assert(func_in != nullptr);
    const Data& encodedValue = *(reinterpret_cast<const Data*>(val));
    return func_in->implV2.addIntParam(static_cast<uint32_t>(bits), encodedValue, isOutput);
}

int TWEthereumAbiFunctionAddParamBool(struct TWEthereumAbiFunction *_Nonnull func_in, bool val, bool isOutput) {
    assert(func_in != nullptr);

    EthereumAbi::Proto::NamedParamType paramType;
    // Declare the `boolean` type.
    paramType.mutable_param()->mutable_boolean();

    EthereumAbi::Proto::NamedParam paramValue;
    paramValue.mutable_value()->set_boolean(val);

    return func_in->implV2.addParam(paramType, paramValue, isOutput);
}

int TWEthereumAbiFunctionAddParamString(struct TWEthereumAbiFunction *_Nonnull func_in, TWString *_Nonnull val, bool isOutput) {
    assert(func_in != nullptr);

    EthereumAbi::Proto::NamedParamType paramType;
    // Declare the `string` type.
    paramType.mutable_param()->mutable_string_param();

    EthereumAbi::Proto::NamedParam paramValue;
    auto* s = reinterpret_cast<const std::string*>(val);
    paramValue.mutable_value()->set_string_value(*s);

    return func_in->implV2.addParam(paramType, paramValue, isOutput);
}

int TWEthereumAbiFunctionAddParamAddress(struct TWEthereumAbiFunction *_Nonnull func_in, TWData *_Nonnull val, bool isOutput) {
    assert(func_in != nullptr);

    EthereumAbi::Proto::NamedParamType paramType;
    // Declare the `string` type.
    paramType.mutable_param()->mutable_address();

    EthereumAbi::Proto::NamedParam paramValue;
    const Data& addressData = *(reinterpret_cast<const Data*>(val));
    bool prefixed = true;
    auto addressStr = hex(addressData, prefixed);
    paramValue.mutable_value()->set_address(addressStr);

    return func_in->implV2.addParam(paramType, paramValue, isOutput);
}

int TWEthereumAbiFunctionAddParamBytes(struct TWEthereumAbiFunction *_Nonnull func_in, TWData *_Nonnull val, bool isOutput) {
    assert(func_in != nullptr);

    EthereumAbi::Proto::NamedParamType paramType;
    // Declare the `string` type.
    paramType.mutable_param()->mutable_byte_array();

    EthereumAbi::Proto::NamedParam paramValue;
    const Data& bytesData = *(reinterpret_cast<const Data*>(val));
    paramValue.mutable_value()->set_byte_array(bytesData.data(), bytesData.size());

    return func_in->implV2.addParam(paramType, paramValue, isOutput);
}

int TWEthereumAbiFunctionAddParamBytesFix(struct TWEthereumAbiFunction *_Nonnull func_in, size_t count, TWData *_Nonnull val, bool isOutput) {
    assert(func_in != nullptr);

    EthereumAbi::Proto::NamedParamType paramType;
    // Declare the `string` type.
    paramType.mutable_param()->mutable_byte_array_fix()->set_size(static_cast<uint64_t>(count));

    EthereumAbi::Proto::NamedParam paramValue;
    const Data& bytesData = *(reinterpret_cast<const Data*>(val));
    paramValue.mutable_value()->set_byte_array_fix(bytesData.data(), bytesData.size());

    return func_in->implV2.addParam(paramType, paramValue, isOutput);
}

int TWEthereumAbiFunctionAddParamArray(struct TWEthereumAbiFunction *_Nonnull func_in, bool isOutput) {
    assert(func_in != nullptr);

    EthereumAbi::Proto::NamedParamType paramType;
    // Declare the `string` type.
    paramType.mutable_param()->mutable_array();

    EthereumAbi::Proto::NamedParam paramValue;
    // Declare the `array` empty value.
    paramValue.mutable_value()->mutable_array();

    return func_in->implV2.addParam(paramType, paramValue, isOutput);
}

///// GetParam

uint8_t TWEthereumAbiFunctionGetParamUInt8(struct TWEthereumAbiFunction *_Nonnull func_in, int idx, bool isOutput) {
    assert(func_in != nullptr);
    return func_in->implV2.getUintParam<uint8_t>(idx, 8, isOutput);
}

uint64_t TWEthereumAbiFunctionGetParamUInt64(struct TWEthereumAbiFunction *_Nonnull func_in, int idx, bool isOutput) {
    assert(func_in != nullptr);
    return func_in->implV2.getUintParam<uint64_t>(idx, 64, isOutput);
}

TWData *_Nonnull TWEthereumAbiFunctionGetParamUInt256(struct TWEthereumAbiFunction *_Nonnull func_in, int idx, bool isOutput) {
    assert(func_in != nullptr);
    auto valData = func_in->implV2.getUintParamData(idx, 256, isOutput);
    return TWDataCreateWithData(&valData);
}

bool TWEthereumAbiFunctionGetParamBool(struct TWEthereumAbiFunction *_Nonnull func_in, int idx, bool isOutput) {
    assert(func_in != nullptr);
    auto param = func_in->implV2.getParam(idx, isOutput);
    if (!param.has_value() || !param->value().has_boolean()) {
        return false;
    }
    return param->value().boolean();
}

TWString *_Nonnull TWEthereumAbiFunctionGetParamString(struct TWEthereumAbiFunction *_Nonnull func_in, int idx, bool isOutput) {
    assert(func_in != nullptr);
    std::string valStr;

    auto param = func_in->implV2.getParam(idx, isOutput);
    if (!param.has_value() || !param->value().has_string_value()) {
        return TWStringCreateWithUTF8Bytes(valStr.c_str());
    }
    valStr = param->value().string_value();
    return TWStringCreateWithUTF8Bytes(valStr.c_str());
}

TWData *_Nonnull TWEthereumAbiFunctionGetParamAddress(struct TWEthereumAbiFunction *_Nonnull func_in, int idx, bool isOutput) {
    assert(func_in != nullptr);
    Data addressData;

    auto param = func_in->implV2.getParam(idx, isOutput);
    if (!param.has_value() || !param->value().has_address()) {
        return TWDataCreateWithData(&addressData);
    }
    auto addressStr = param->value().address();
    try {
        addressData = parse_hex(addressStr);
        return TWDataCreateWithData(&addressData);
    } catch (...) {
        return TWDataCreateWithData(&addressData);
    }
}

///// AddInArrayParam

int addInArrayParam(EthAbi::Function& function, int arrayIdx, const std::shared_ptr<EthAbi::ParamBase>& childParam) {
    std::shared_ptr<EthAbi::ParamBase> param;
    if (!function.getInParam(arrayIdx, param)) {
        return -1;
    }
    std::shared_ptr<EthAbi::ParamArray> paramArr = std::dynamic_pointer_cast<EthAbi::ParamArray>(param);
    if (paramArr == nullptr) {
        return -1; // not an array
    }
    return paramArr->addParam(childParam);
}

int TWEthereumAbiFunctionAddInArrayParamUInt8(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, uint8_t val) {
    assert(func_in != nullptr);
    EthAbi::Function& function = func_in->impl;

    return addInArrayParam(function, arrayIdx, std::make_shared<EthAbi::ParamUInt8>(val));
}

int TWEthereumAbiFunctionAddInArrayParamUInt16(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, uint16_t val) {
    assert(func_in != nullptr);
    EthAbi::Function& function = func_in->impl;

    return addInArrayParam(function, arrayIdx, std::make_shared<EthAbi::ParamUInt16>(val));
}

int TWEthereumAbiFunctionAddInArrayParamUInt32(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, uint32_t val) {
    assert(func_in != nullptr);
    EthAbi::Function& function = func_in->impl;

    return addInArrayParam(function, arrayIdx, std::make_shared<EthAbi::ParamUInt32>(val));
}

int TWEthereumAbiFunctionAddInArrayParamUInt64(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, uint64_t val) {
    assert(func_in != nullptr);
    EthAbi::Function& function = func_in->impl;

    return addInArrayParam(function, arrayIdx, std::make_shared<EthAbi::ParamUInt64>(val));
}

int TWEthereumAbiFunctionAddInArrayParamUInt256(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, TWData *_Nonnull val) {
    assert(func_in != nullptr);
    EthAbi::Function& function = func_in->impl;

    uint256_t val2 = load(*static_cast<const Data*>(val));
    return addInArrayParam(function, arrayIdx, std::make_shared<EthAbi::ParamUInt256>(val2));
}

int TWEthereumAbiFunctionAddInArrayParamUIntN(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, int bits, TWData *_Nonnull val) {
    assert(func_in != nullptr);
    EthAbi::Function& function = func_in->impl;

    uint256_t val2 = load(*static_cast<const Data*>(val));
    return addInArrayParam(function, arrayIdx, std::make_shared<EthAbi::ParamUIntN>(bits, val2));
}

int TWEthereumAbiFunctionAddInArrayParamInt8(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, int8_t val) {
    assert(func_in != nullptr);
    EthAbi::Function& function = func_in->impl;

    return addInArrayParam(function, arrayIdx, std::make_shared<EthAbi::ParamInt8>(val));
}

int TWEthereumAbiFunctionAddInArrayParamInt16(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, int16_t val) {
    assert(func_in != nullptr);
    EthAbi::Function& function = func_in->impl;

    return addInArrayParam(function, arrayIdx, std::make_shared<EthAbi::ParamInt16>(val));
}

int TWEthereumAbiFunctionAddInArrayParamInt32(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, int32_t val) {
    assert(func_in != nullptr);
    EthAbi::Function& function = func_in->impl;

    return addInArrayParam(function, arrayIdx, std::make_shared<EthAbi::ParamInt32>(val));
}

int TWEthereumAbiFunctionAddInArrayParamInt64(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, int64_t val) {
    assert(func_in != nullptr);
    EthAbi::Function& function = func_in->impl;

    return addInArrayParam(function, arrayIdx, std::make_shared<EthAbi::ParamInt64>(val));
}

int TWEthereumAbiFunctionAddInArrayParamInt256(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, TWData *_Nonnull val) {
    assert(func_in != nullptr);
    EthAbi::Function& function = func_in->impl;

    assert(val != nullptr);
    int256_t val2 = EthAbi::ValueEncoder::int256FromUint256(load(*static_cast<const Data*>(val)));
    return addInArrayParam(function, arrayIdx, std::make_shared<EthAbi::ParamInt256>(val2));
}

int TWEthereumAbiFunctionAddInArrayParamIntN(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, int bits, TWData *_Nonnull val) {
    assert(func_in != nullptr);
    EthAbi::Function& function = func_in->impl;

    assert(val != nullptr);
    int256_t val2 = EthAbi::ValueEncoder::int256FromUint256(load(*static_cast<const Data*>(val)));
    return addInArrayParam(function, arrayIdx, std::make_shared<EthAbi::ParamIntN>(bits, val2));
}

int TWEthereumAbiFunctionAddInArrayParamBool(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, bool val) {
    assert(func_in != nullptr);
    EthAbi::Function& function = func_in->impl;

    return addInArrayParam(function, arrayIdx, std::make_shared<EthAbi::ParamBool>(val));
}

int TWEthereumAbiFunctionAddInArrayParamString(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, TWString *_Nonnull val) {
    assert(func_in != nullptr);
    EthAbi::Function& function = func_in->impl;

    assert(val != nullptr);
    return addInArrayParam(function, arrayIdx, std::make_shared<EthAbi::ParamString>(TWStringUTF8Bytes(val)));
}

int TWEthereumAbiFunctionAddInArrayParamAddress(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, TWData *_Nonnull val) {
    assert(func_in != nullptr);
    EthAbi::Function& function = func_in->impl;

    assert(val != nullptr);
    Data data = TW::data(TWDataBytes(val), TWDataSize(val));
    return addInArrayParam(function, arrayIdx, std::make_shared<EthAbi::ParamAddress>(data));
}

int TWEthereumAbiFunctionAddInArrayParamBytes(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, TWData *_Nonnull val) {
    assert(func_in != nullptr);
    EthAbi::Function& function = func_in->impl;

    Data data = TW::data(TWDataBytes(val), TWDataSize(val));
    return addInArrayParam(function, arrayIdx, std::make_shared<EthAbi::ParamByteArray>(data));
}

int TWEthereumAbiFunctionAddInArrayParamBytesFix(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, size_t count, TWData *_Nonnull val) {
    assert(func_in != nullptr);
    EthAbi::Function& function = func_in->impl;

    Data data = TW::data(TWDataBytes(val), TWDataSize(val));
    return addInArrayParam(function, arrayIdx, std::make_shared<EthAbi::ParamByteArrayFix>(count, data));
}
