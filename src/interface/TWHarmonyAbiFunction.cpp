// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWHarmonyAbiFunction.h>

#include "Ethereum/ABI.h"
#include "Data.h"
#include "HexCoding.h"
#include "../uint256.h"

#include <string>
#include <vector>
#include <cassert>

using namespace TW;
using namespace TW::Ethereum;
using namespace TW::Ethereum::ABI;

/// Wrapper for C interface.
struct TWHarmonyAbiFunction {
    TW::Ethereum::ABI::Function impl;
};

struct TWHarmonyAbiFunction *_Nullable TWHarmonyAbiFunctionCreateWithString(TWString *_Nonnull name) {
    auto func = Function(TWStringUTF8Bytes(name));
    return new TWHarmonyAbiFunction{ func };
}

void TWHarmonyAbiFunctionDelete(struct TWHarmonyAbiFunction *_Nonnull func_in) {
    assert(func_in != nullptr);
    delete func_in;
}

TWString *_Nonnull TWHarmonyAbiFunctionGetType(struct TWHarmonyAbiFunction *_Nonnull func_in) {
    assert(func_in != nullptr);
    auto function = func_in->impl;
    std::string sign = function.getType();
    return TWStringCreateWithUTF8Bytes(sign.c_str());
}

///// AddParam

int TWHarmonyAbiFunctionAddParamUInt8(struct TWHarmonyAbiFunction *_Nonnull func_in, uint8_t val, bool isOutput) {
    assert(func_in != nullptr);
    Function& function = func_in->impl;

    auto param = std::make_shared<ParamUInt8>(val);
    auto idx = function.addParam(param, isOutput);
    return idx;
}

int TWHarmonyAbiFunctionAddParamUInt16(struct TWHarmonyAbiFunction *_Nonnull func_in, uint16_t val, bool isOutput) {
    assert(func_in != nullptr);
    Function& function = func_in->impl;

    auto param = std::make_shared<ParamUInt16>(val);
    auto idx = function.addParam(param, isOutput);
    return idx;
}

int TWHarmonyAbiFunctionAddParamUInt32(struct TWHarmonyAbiFunction *_Nonnull func_in, uint32_t val, bool isOutput) {
    assert(func_in != nullptr);
    Function& function = func_in->impl;

    auto param = std::make_shared<ParamUInt32>(val);
    auto idx = function.addParam(param, isOutput);
    return idx;
}

int TWHarmonyAbiFunctionAddParamUInt64(struct TWHarmonyAbiFunction *_Nonnull func_in, uint64_t val, bool isOutput) {
    assert(func_in != nullptr);
    Function& function = func_in->impl;

    auto param = std::make_shared<ParamUInt64>(val);
    auto idx = function.addParam(param, isOutput);
    return idx;
}

int TWHarmonyAbiFunctionAddParamUInt256(struct TWHarmonyAbiFunction *_Nonnull func_in, TWData *_Nonnull val, bool isOutput) {
    assert(func_in != nullptr);
    Function& function = func_in->impl;

    uint256_t val2 = load(*static_cast<const Data*>(val));
    auto param = std::make_shared<ParamUInt256>(val2);
    auto idx = function.addParam(param, isOutput);
    return idx;
}

int TWHarmonyAbiFunctionAddParamUIntN(struct TWHarmonyAbiFunction *_Nonnull func_in, int bits, TWData *_Nonnull val, bool isOutput) {
    assert(func_in != nullptr);
    Function& function = func_in->impl;

    uint256_t val2 = load(*static_cast<const Data*>(val));
    auto param = std::make_shared<ParamUIntN>(bits, val2);
    auto idx = function.addParam(param, isOutput);
    return idx;
}

int TWHarmonyAbiFunctionAddParamInt8(struct TWHarmonyAbiFunction *_Nonnull func_in, int8_t val, bool isOutput) {
    assert(func_in != nullptr);
    Function& function = func_in->impl;

    auto param = std::make_shared<ParamInt8>(val);
    auto idx = function.addParam(param, isOutput);
    return idx;
}

int TWHarmonyAbiFunctionAddParamInt16(struct TWHarmonyAbiFunction *_Nonnull func_in, int16_t val, bool isOutput) {
    assert(func_in != nullptr);
    Function& function = func_in->impl;

    auto param = std::make_shared<ParamInt16>(val);
    auto idx = function.addParam(param, isOutput);
    return idx;
}

int TWHarmonyAbiFunctionAddParamInt32(struct TWHarmonyAbiFunction *_Nonnull func_in, int32_t val, bool isOutput) {
    assert(func_in != nullptr);
    Function& function = func_in->impl;

    auto param = std::make_shared<ParamInt32>(val);
    auto idx = function.addParam(param, isOutput);
    return idx;
}

int TWHarmonyAbiFunctionAddParamInt64(struct TWHarmonyAbiFunction *_Nonnull func_in, int64_t val, bool isOutput) {
    assert(func_in != nullptr);
    Function& function = func_in->impl;

    auto param = std::make_shared<ParamInt64>(val);
    auto idx = function.addParam(param, isOutput);
    return idx;
}

int TWHarmonyAbiFunctionAddParamInt256(struct TWHarmonyAbiFunction *_Nonnull func_in, TWData *_Nonnull val, bool isOutput) {
    assert(func_in != nullptr);
    Function& function = func_in->impl;

    assert(val != nullptr);
    int256_t val2 = ValueEncoder::int256FromUint256(load(*static_cast<const Data*>(val)));
    auto param = std::make_shared<ParamInt256>(val2);
    auto idx = function.addParam(param, isOutput);
    return idx;
}

int TWHarmonyAbiFunctionAddParamIntN(struct TWHarmonyAbiFunction *_Nonnull func_in, int bits, TWData *_Nonnull val, bool isOutput) {
    assert(func_in != nullptr);
    Function& function = func_in->impl;

    assert(val != nullptr);
    int256_t val2 = ValueEncoder::int256FromUint256(load(*static_cast<const Data*>(val)));
    auto param = std::make_shared<ParamIntN>(bits, val2);
    auto idx = function.addParam(param, isOutput);
    return idx;
}

int TWHarmonyAbiFunctionAddParamBool(struct TWHarmonyAbiFunction *_Nonnull func_in, bool val, bool isOutput) {
    assert(func_in != nullptr);
    Function& function = func_in->impl;

    auto param = std::make_shared<ParamBool>(val);
    auto idx = function.addParam(param, isOutput);
    return idx;
}

int TWHarmonyAbiFunctionAddParamString(struct TWHarmonyAbiFunction *_Nonnull func_in, TWString *_Nonnull val, bool isOutput) {
    assert(func_in != nullptr);
    Function& function = func_in->impl;

    assert(val != nullptr);
    auto param = std::make_shared<ParamString>(TWStringUTF8Bytes(val));
    auto idx = function.addParam(param, isOutput);
    return idx;
}

int TWHarmonyAbiFunctionAddParamAddress(struct TWHarmonyAbiFunction *_Nonnull func_in, TWData *_Nonnull val, bool isOutput) {
    assert(func_in != nullptr);
    Function& function = func_in->impl;

    assert(val != nullptr);
    Data data = TW::data(TWDataBytes(val), TWDataSize(val));
    auto param = std::make_shared<ParamAddress>(data);
    auto idx = function.addParam(param, isOutput);
    return idx;
}

int TWHarmonyAbiFunctionAddParamBytes(struct TWHarmonyAbiFunction *_Nonnull func_in, TWData *_Nonnull val, bool isOutput) {
    assert(func_in != nullptr);
    Function& function = func_in->impl;

    Data data = TW::data(TWDataBytes(val), TWDataSize(val));
    auto param = std::make_shared<ParamByteArray>(data);
    auto idx = function.addParam(param, isOutput);
    return idx;    
}

int TWHarmonyAbiFunctionAddParamBytesFix(struct TWHarmonyAbiFunction *_Nonnull func_in, size_t count, TWData *_Nonnull val, bool isOutput) {
    assert(func_in != nullptr);
    Function& function = func_in->impl;

    Data data = TW::data(TWDataBytes(val), TWDataSize(val));
    auto param = std::make_shared<ParamByteArrayFix>(count, data);
    auto idx = function.addParam(param, isOutput);
    return idx;    
}

int TWHarmonyAbiFunctionAddParamArray(struct TWHarmonyAbiFunction *_Nonnull func_in, bool isOutput) {
    assert(func_in != nullptr);
    Function& function = func_in->impl;

    auto param = std::make_shared<ParamArray>();
    auto idx = function.addParam(param, isOutput);
    return idx;    
}

///// GetParam

uint8_t TWHarmonyAbiFunctionGetParamUInt8(struct TWHarmonyAbiFunction *_Nonnull func_in, int idx, bool isOutput) {
    assert(func_in != nullptr);
    Function& function = func_in->impl;

    std::shared_ptr<ParamBase> param;
    if (!function.getParam(idx, param, isOutput)) {
        return 0;
    }
    auto param2 = std::dynamic_pointer_cast<ParamUInt8>(param);
    if (param2 == nullptr) {
        return 0;
    }
    return param2->getVal();
}

uint64_t TWHarmonyAbiFunctionGetParamUInt64(struct TWHarmonyAbiFunction *_Nonnull func_in, int idx, bool isOutput) {
    assert(func_in != nullptr);
    Function& function = func_in->impl;

    std::shared_ptr<ParamBase> param;
    if (!function.getParam(idx, param, isOutput)) {
        return 0;
    }
    auto param2 = std::dynamic_pointer_cast<ParamUInt64>(param);
    if (param2 == nullptr) {
        return 0;
    }
    return param2->getVal();
}

TWData *_Nonnull TWHarmonyAbiFunctionGetParamUInt256(struct TWHarmonyAbiFunction *_Nonnull func_in, int idx, bool isOutput) {
    assert(func_in != nullptr);
    Function& function = func_in->impl;

    uint256_t val256 = 0;
    std::shared_ptr<ParamBase> param;
    if (!function.getParam(idx, param, isOutput)) {
        TW::Data valData = TW::store(val256);
        return TWDataCreateWithData(&valData);
    }
    auto param2 = std::dynamic_pointer_cast<ParamUInt256>(param);
    if (param2 == nullptr) {
        TW::Data valData = TW::store(val256);
        return TWDataCreateWithData(&valData);
    }
    val256 = param2->getVal();
    TW::Data valData = TW::store(val256);
    return TWDataCreateWithData(&valData);
}

bool TWHarmonyAbiFunctionGetParamBool(struct TWHarmonyAbiFunction *_Nonnull func_in, int idx, bool isOutput) {
    assert(func_in != nullptr);
    Function& function = func_in->impl;

    std::shared_ptr<ParamBase> param;
    if (!function.getParam(idx, param, isOutput)) {
        return false;
    }
    auto param2 = std::dynamic_pointer_cast<ParamBool>(param);
    if (param2 == nullptr) {
        return false;
    }
    return param2->getVal();
}

TWString *_Nonnull TWHarmonyAbiFunctionGetParamString(struct TWHarmonyAbiFunction *_Nonnull func_in, int idx, bool isOutput) {
    assert(func_in != nullptr);
    Function& function = func_in->impl;

    std::string valStr;
    std::shared_ptr<ParamBase> param;
    if (!function.getParam(idx, param, isOutput)) {
        return TWStringCreateWithUTF8Bytes(valStr.c_str());
    }
    auto param2 = std::dynamic_pointer_cast<ParamString>(param);
    if (param2 == nullptr) {
        return TWStringCreateWithUTF8Bytes(valStr.c_str());
    }
    valStr = param2->getVal();
    return TWStringCreateWithUTF8Bytes(valStr.c_str());
}

TWData *_Nonnull TWHarmonyAbiFunctionGetParamAddress(struct TWHarmonyAbiFunction *_Nonnull func_in, int idx, bool isOutput) {
    assert(func_in != nullptr);
    Function& function = func_in->impl;

    Data valData;
    std::shared_ptr<ParamBase> param;
    if (!function.getParam(idx, param, isOutput)) {
        return TWDataCreateWithData(&valData);
    }
    auto param2 = std::dynamic_pointer_cast<ParamAddress>(param);
    if (param2 == nullptr) {
        return TWDataCreateWithData(&valData);
    }
    valData = param2->getData();
    return TWDataCreateWithData(&valData);
}

///// AddInArrayParam

static int addInArrayParam(Function& function, int arrayIdx, const std::shared_ptr<ParamBase>& childParam) {
    std::shared_ptr<ParamBase> param;
    if (!function.getInParam(arrayIdx, param)) {
        return -1;
    }
    std::shared_ptr<ParamArray> paramArr = std::dynamic_pointer_cast<ParamArray>(param);
    if (paramArr == nullptr) {
        return -1; // not an array
    }
    return paramArr->addParam(childParam);
}

int TWHarmonyAbiFunctionAddInArrayParamUInt8(struct TWHarmonyAbiFunction *_Nonnull func_in, int arrayIdx, uint8_t val) {
    assert(func_in != nullptr);
    Function& function = func_in->impl;

    return addInArrayParam(function, arrayIdx, std::make_shared<ParamUInt8>(val));
}

int TWHarmonyAbiFunctionAddInArrayParamUInt16(struct TWHarmonyAbiFunction *_Nonnull func_in, int arrayIdx, uint16_t val) {
    assert(func_in != nullptr);
    Function& function = func_in->impl;

    return addInArrayParam(function, arrayIdx, std::make_shared<ParamUInt16>(val));
}

int TWHarmonyAbiFunctionAddInArrayParamUInt32(struct TWHarmonyAbiFunction *_Nonnull func_in, int arrayIdx, uint32_t val) {
    assert(func_in != nullptr);
    Function& function = func_in->impl;

    return addInArrayParam(function, arrayIdx, std::make_shared<ParamUInt32>(val));
}

int TWHarmonyAbiFunctionAddInArrayParamUInt64(struct TWHarmonyAbiFunction *_Nonnull func_in, int arrayIdx, uint64_t val) {
    assert(func_in != nullptr);
    Function& function = func_in->impl;

    return addInArrayParam(function, arrayIdx, std::make_shared<ParamUInt64>(val));
}

int TWHarmonyAbiFunctionAddInArrayParamUInt256(struct TWHarmonyAbiFunction *_Nonnull func_in, int arrayIdx, TWData *_Nonnull val) {
    assert(func_in != nullptr);
    Function& function = func_in->impl;

    uint256_t val2 = load(*static_cast<const Data*>(val));
    return addInArrayParam(function, arrayIdx, std::make_shared<ParamUInt256>(val2));
}

int TWHarmonyAbiFunctionAddInArrayParamUIntN(struct TWHarmonyAbiFunction *_Nonnull func_in, int arrayIdx, int bits, TWData *_Nonnull val) {
    assert(func_in != nullptr);
    Function& function = func_in->impl;

    uint256_t val2 = load(*static_cast<const Data*>(val));
    return addInArrayParam(function, arrayIdx, std::make_shared<ParamUIntN>(bits, val2));
}

int TWHarmonyAbiFunctionAddInArrayParamInt8(struct TWHarmonyAbiFunction *_Nonnull func_in, int arrayIdx, int8_t val) {
    assert(func_in != nullptr);
    Function& function = func_in->impl;

    return addInArrayParam(function, arrayIdx, std::make_shared<ParamInt8>(val));
}

int TWHarmonyAbiFunctionAddInArrayParamInt16(struct TWHarmonyAbiFunction *_Nonnull func_in, int arrayIdx, int16_t val) {
    assert(func_in != nullptr);
    Function& function = func_in->impl;

    return addInArrayParam(function, arrayIdx, std::make_shared<ParamInt16>(val));
}

int TWHarmonyAbiFunctionAddInArrayParamInt32(struct TWHarmonyAbiFunction *_Nonnull func_in, int arrayIdx, int32_t val) {
    assert(func_in != nullptr);
    Function& function = func_in->impl;

    return addInArrayParam(function, arrayIdx, std::make_shared<ParamInt32>(val));
}

int TWHarmonyAbiFunctionAddInArrayParamInt64(struct TWHarmonyAbiFunction *_Nonnull func_in, int arrayIdx, int64_t val) {
    assert(func_in != nullptr);
    Function& function = func_in->impl;

    return addInArrayParam(function, arrayIdx, std::make_shared<ParamInt64>(val));
}

int TWHarmonyAbiFunctionAddInArrayParamInt256(struct TWHarmonyAbiFunction *_Nonnull func_in, int arrayIdx, TWData *_Nonnull val) {
    assert(func_in != nullptr);
    Function& function = func_in->impl;

    assert(val != nullptr);
    int256_t val2 = ValueEncoder::int256FromUint256(load(*static_cast<const Data*>(val)));
    return addInArrayParam(function, arrayIdx, std::make_shared<ParamInt256>(val2));
}

int TWHarmonyAbiFunctionAddInArrayParamIntN(struct TWHarmonyAbiFunction *_Nonnull func_in, int arrayIdx, int bits, TWData *_Nonnull val) {
    assert(func_in != nullptr);
    Function& function = func_in->impl;

    assert(val != nullptr);
    int256_t val2 = ValueEncoder::int256FromUint256(load(*static_cast<const Data*>(val)));
    return addInArrayParam(function, arrayIdx, std::make_shared<ParamIntN>(bits, val2));
}

int TWHarmonyAbiFunctionAddInArrayParamBool(struct TWHarmonyAbiFunction *_Nonnull func_in, int arrayIdx, bool val) {
    assert(func_in != nullptr);
    Function& function = func_in->impl;

    return addInArrayParam(function, arrayIdx, std::make_shared<ParamBool>(val));
}

int TWHarmonyAbiFunctionAddInArrayParamString(struct TWHarmonyAbiFunction *_Nonnull func_in, int arrayIdx, TWString *_Nonnull val) {
    assert(func_in != nullptr);
    Function& function = func_in->impl;

    assert(val != nullptr);
    return addInArrayParam(function, arrayIdx, std::make_shared<ParamString>(TWStringUTF8Bytes(val)));
}

int TWHarmonyAbiFunctionAddInArrayParamAddress(struct TWHarmonyAbiFunction *_Nonnull func_in, int arrayIdx, TWData *_Nonnull val) {
    assert(func_in != nullptr);
    Function& function = func_in->impl;

    assert(val != nullptr);
    Data data = TW::data(TWDataBytes(val), TWDataSize(val));
    return addInArrayParam(function, arrayIdx, std::make_shared<ParamAddress>(data));
}

int TWHarmonyAbiFunctionAddInArrayParamBytes(struct TWHarmonyAbiFunction *_Nonnull func_in, int arrayIdx, TWData *_Nonnull val) {
    assert(func_in != nullptr);
    Function& function = func_in->impl;

    Data data = TW::data(TWDataBytes(val), TWDataSize(val));
    return addInArrayParam(function, arrayIdx, std::make_shared<ParamByteArray>(data));
}

int TWHarmonyAbiFunctionAddInArrayParamBytesFix(struct TWHarmonyAbiFunction *_Nonnull func_in, int arrayIdx, size_t count, TWData *_Nonnull val) {
    assert(func_in != nullptr);
    Function& function = func_in->impl;

    Data data = TW::data(TWDataBytes(val), TWDataSize(val));
    return addInArrayParam(function, arrayIdx, std::make_shared<ParamByteArrayFix>(count, data));
}
