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
    return new TWEthereumAbiFunction{ func };
}

void TWEthereumAbiFunctionDelete(struct TWEthereumAbiFunction *_Nonnull func_in) {
    assert(func_in != nullptr);
    delete func_in;
}

TWString *_Nonnull TWEthereumAbiFunctionGetType(struct TWEthereumAbiFunction *_Nonnull func_in) {
    assert(func_in != nullptr);
    auto function = func_in->impl;
    std::string sign = function.getType();
    return TWStringCreateWithUTF8Bytes(sign.c_str());
}

///// AddParam

int TWEthereumAbiFunctionAddParamUInt8(struct TWEthereumAbiFunction *_Nonnull func_in, uint8_t val, bool isOutput) {
    assert(func_in != nullptr);
    EthAbi::Function& function = func_in->impl;

    auto param = std::make_shared<EthAbi::ParamUInt8>(val);
    auto idx = function.addParam(param, isOutput);
    return idx;
}

int TWEthereumAbiFunctionAddParamUInt16(struct TWEthereumAbiFunction *_Nonnull func_in, uint16_t val, bool isOutput) {
    assert(func_in != nullptr);
    EthAbi::Function& function = func_in->impl;

    auto param = std::make_shared<EthAbi::ParamUInt16>(val);
    auto idx = function.addParam(param, isOutput);
    return idx;
}

int TWEthereumAbiFunctionAddParamUInt32(struct TWEthereumAbiFunction *_Nonnull func_in, uint32_t val, bool isOutput) {
    assert(func_in != nullptr);
    EthAbi::Function& function = func_in->impl;

    auto param = std::make_shared<EthAbi::ParamUInt32>(val);
    auto idx = function.addParam(param, isOutput);
    return idx;
}

int TWEthereumAbiFunctionAddParamUInt64(struct TWEthereumAbiFunction *_Nonnull func_in, uint64_t val, bool isOutput) {
    assert(func_in != nullptr);
    EthAbi::Function& function = func_in->impl;

    auto param = std::make_shared<EthAbi::ParamUInt64>(val);
    auto idx = function.addParam(param, isOutput);
    return idx;
}

int TWEthereumAbiFunctionAddParamUInt256(struct TWEthereumAbiFunction *_Nonnull func_in, TWData *_Nonnull val, bool isOutput) {
    assert(func_in != nullptr);
    EthAbi::Function& function = func_in->impl;

    uint256_t val2 = load(*static_cast<const Data*>(val));
    auto param = std::make_shared<EthAbi::ParamUInt256>(val2);
    auto idx = function.addParam(param, isOutput);
    return idx;
}

int TWEthereumAbiFunctionAddParamUIntN(struct TWEthereumAbiFunction *_Nonnull func_in, int bits, TWData *_Nonnull val, bool isOutput) {
    assert(func_in != nullptr);
    EthAbi::Function& function = func_in->impl;

    uint256_t val2 = load(*static_cast<const Data*>(val));
    auto param = std::make_shared<EthAbi::ParamUIntN>(bits, val2);
    auto idx = function.addParam(param, isOutput);
    return idx;
}

int TWEthereumAbiFunctionAddParamInt8(struct TWEthereumAbiFunction *_Nonnull func_in, int8_t val, bool isOutput) {
    assert(func_in != nullptr);
    EthAbi::Function& function = func_in->impl;

    auto param = std::make_shared<EthAbi::ParamInt8>(val);
    auto idx = function.addParam(param, isOutput);
    return idx;
}

int TWEthereumAbiFunctionAddParamInt16(struct TWEthereumAbiFunction *_Nonnull func_in, int16_t val, bool isOutput) {
    assert(func_in != nullptr);
    EthAbi::Function& function = func_in->impl;

    auto param = std::make_shared<EthAbi::ParamInt16>(val);
    auto idx = function.addParam(param, isOutput);
    return idx;
}

int TWEthereumAbiFunctionAddParamInt32(struct TWEthereumAbiFunction *_Nonnull func_in, int32_t val, bool isOutput) {
    assert(func_in != nullptr);
    EthAbi::Function& function = func_in->impl;

    auto param = std::make_shared<EthAbi::ParamInt32>(val);
    auto idx = function.addParam(param, isOutput);
    return idx;
}

int TWEthereumAbiFunctionAddParamInt64(struct TWEthereumAbiFunction *_Nonnull func_in, int64_t val, bool isOutput) {
    assert(func_in != nullptr);
    EthAbi::Function& function = func_in->impl;

    auto param = std::make_shared<EthAbi::ParamInt64>(val);
    auto idx = function.addParam(param, isOutput);
    return idx;
}

int TWEthereumAbiFunctionAddParamInt256(struct TWEthereumAbiFunction *_Nonnull func_in, TWData *_Nonnull val, bool isOutput) {
    assert(func_in != nullptr);
    EthAbi::Function& function = func_in->impl;

    assert(val != nullptr);
    int256_t val2 = EthAbi::ValueEncoder::int256FromUint256(load(*static_cast<const Data*>(val)));
    auto param = std::make_shared<EthAbi::ParamInt256>(val2);
    auto idx = function.addParam(param, isOutput);
    return idx;
}

int TWEthereumAbiFunctionAddParamIntN(struct TWEthereumAbiFunction *_Nonnull func_in, int bits, TWData *_Nonnull val, bool isOutput) {
    assert(func_in != nullptr);
    EthAbi::Function& function = func_in->impl;

    assert(val != nullptr);
    int256_t val2 = EthAbi::ValueEncoder::int256FromUint256(load(*static_cast<const Data*>(val)));
    auto param = std::make_shared<EthAbi::ParamIntN>(bits, val2);
    auto idx = function.addParam(param, isOutput);
    return idx;
}

int TWEthereumAbiFunctionAddParamBool(struct TWEthereumAbiFunction *_Nonnull func_in, bool val, bool isOutput) {
    assert(func_in != nullptr);
    EthAbi::Function& function = func_in->impl;

    auto param = std::make_shared<EthAbi::ParamBool>(val);
    auto idx = function.addParam(param, isOutput);
    return idx;
}

int TWEthereumAbiFunctionAddParamString(struct TWEthereumAbiFunction *_Nonnull func_in, TWString *_Nonnull val, bool isOutput) {
    assert(func_in != nullptr);
    EthAbi::Function& function = func_in->impl;

    assert(val != nullptr);
    auto param = std::make_shared<EthAbi::ParamString>(TWStringUTF8Bytes(val));
    auto idx = function.addParam(param, isOutput);
    return idx;
}

int TWEthereumAbiFunctionAddParamAddress(struct TWEthereumAbiFunction *_Nonnull func_in, TWData *_Nonnull val, bool isOutput) {
    assert(func_in != nullptr);
    EthAbi::Function& function = func_in->impl;

    assert(val != nullptr);
    Data data = TW::data(TWDataBytes(val), TWDataSize(val));
    auto param = std::make_shared<EthAbi::ParamAddress>(data);
    auto idx = function.addParam(param, isOutput);
    return idx;
}

int TWEthereumAbiFunctionAddParamBytes(struct TWEthereumAbiFunction *_Nonnull func_in, TWData *_Nonnull val, bool isOutput) {
    assert(func_in != nullptr);
    EthAbi::Function& function = func_in->impl;

    Data data = TW::data(TWDataBytes(val), TWDataSize(val));
    auto param = std::make_shared<EthAbi::ParamByteArray>(data);
    auto idx = function.addParam(param, isOutput);
    return idx;    
}

int TWEthereumAbiFunctionAddParamBytesFix(struct TWEthereumAbiFunction *_Nonnull func_in, size_t count, TWData *_Nonnull val, bool isOutput) {
    assert(func_in != nullptr);
    EthAbi::Function& function = func_in->impl;

    Data data = TW::data(TWDataBytes(val), TWDataSize(val));
    auto param = std::make_shared<EthAbi::ParamByteArrayFix>(count, data);
    auto idx = function.addParam(param, isOutput);
    return idx;    
}

int TWEthereumAbiFunctionAddParamArray(struct TWEthereumAbiFunction *_Nonnull func_in, bool isOutput) {
    assert(func_in != nullptr);
    EthAbi::Function& function = func_in->impl;

    auto param = std::make_shared<EthAbi::ParamArray>();
    auto idx = function.addParam(param, isOutput);
    return idx;    
}

///// GetParam

uint8_t TWEthereumAbiFunctionGetParamUInt8(struct TWEthereumAbiFunction *_Nonnull func_in, int idx, bool isOutput) {
    assert(func_in != nullptr);
    EthAbi::Function& function = func_in->impl;

    std::shared_ptr<EthAbi::ParamBase> param;
    if (!function.getParam(idx, param, isOutput)) {
        return 0;
    }
    auto param2 = std::dynamic_pointer_cast<EthAbi::ParamUInt8>(param);
    if (param2 == nullptr) {
        return 0;
    }
    return param2->getVal();
}

uint64_t TWEthereumAbiFunctionGetParamUInt64(struct TWEthereumAbiFunction *_Nonnull func_in, int idx, bool isOutput) {
    assert(func_in != nullptr);
    EthAbi::Function& function = func_in->impl;

    std::shared_ptr<EthAbi::ParamBase> param;
    if (!function.getParam(idx, param, isOutput)) {
        return 0;
    }
    auto param2 = std::dynamic_pointer_cast<EthAbi::ParamUInt64>(param);
    if (param2 == nullptr) {
        return 0;
    }
    return param2->getVal();
}

TWData *_Nonnull TWEthereumAbiFunctionGetParamUInt256(struct TWEthereumAbiFunction *_Nonnull func_in, int idx, bool isOutput) {
    assert(func_in != nullptr);
    EthAbi::Function& function = func_in->impl;

    uint256_t val256 = 0;
    std::shared_ptr<EthAbi::ParamBase> param;
    if (!function.getParam(idx, param, isOutput)) {
        TW::Data valData = TW::store(val256);
        return TWDataCreateWithData(&valData);
    }
    auto param2 = std::dynamic_pointer_cast<EthAbi::ParamUInt256>(param);
    if (param2 == nullptr) {
        TW::Data valData = TW::store(val256);
        return TWDataCreateWithData(&valData);
    }
    val256 = param2->getVal();
    TW::Data valData = TW::store(val256);
    return TWDataCreateWithData(&valData);
}

bool TWEthereumAbiFunctionGetParamBool(struct TWEthereumAbiFunction *_Nonnull func_in, int idx, bool isOutput) {
    assert(func_in != nullptr);
    EthAbi::Function& function = func_in->impl;

    std::shared_ptr<EthAbi::ParamBase> param;
    if (!function.getParam(idx, param, isOutput)) {
        return false;
    }
    auto param2 = std::dynamic_pointer_cast<EthAbi::ParamBool>(param);
    if (param2 == nullptr) {
        return false;
    }
    return param2->getVal();
}

TWString *_Nonnull TWEthereumAbiFunctionGetParamString(struct TWEthereumAbiFunction *_Nonnull func_in, int idx, bool isOutput) {
    assert(func_in != nullptr);
    EthAbi::Function& function = func_in->impl;

    std::string valStr;
    std::shared_ptr<EthAbi::ParamBase> param;
    if (!function.getParam(idx, param, isOutput)) {
        return TWStringCreateWithUTF8Bytes(valStr.c_str());
    }
    auto param2 = std::dynamic_pointer_cast<EthAbi::ParamString>(param);
    if (param2 == nullptr) {
        return TWStringCreateWithUTF8Bytes(valStr.c_str());
    }
    valStr = param2->getVal();
    return TWStringCreateWithUTF8Bytes(valStr.c_str());
}

TWData *_Nonnull TWEthereumAbiFunctionGetParamAddress(struct TWEthereumAbiFunction *_Nonnull func_in, int idx, bool isOutput) {
    assert(func_in != nullptr);
    EthAbi::Function& function = func_in->impl;

    Data valData;
    std::shared_ptr<EthAbi::ParamBase> param;
    if (!function.getParam(idx, param, isOutput)) {
        return TWDataCreateWithData(&valData);
    }
    auto param2 = std::dynamic_pointer_cast<EthAbi::ParamAddress>(param);
    if (param2 == nullptr) {
        return TWDataCreateWithData(&valData);
    }
    valData = param2->getData();
    return TWDataCreateWithData(&valData);
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
