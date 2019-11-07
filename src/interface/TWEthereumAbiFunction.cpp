// Copyright © 2017-2019 Trust Wallet.
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
using namespace TW::Ethereum;
using namespace TW::Ethereum::ABI;

struct TWEthereumAbiFunction *_Nullable TWEthereumAbiFunctionCreateWithString(TWString *_Nonnull name) {
    auto func = Function(TWStringUTF8Bytes(name));
    return new TWEthereumAbiFunction{ func };
}

void TWEthereumAbiFunctionDelete(struct TWEthereumAbiFunction *_Nonnull func) {
    assert(func != nullptr);
    delete func;
}

TWString *_Nonnull TWEthereumAbiFunctionGetType(struct TWEthereumAbiFunction *_Nonnull func) {
    assert(func != nullptr);
    auto function = func->impl;
    std::string sign = function.getType();
    return TWStringCreateWithUTF8Bytes(sign.c_str());
}

///// AddParam

int TWEthereumAbiFunctionAddParamInt32(struct TWEthereumAbiFunction *_Nonnull func, uint64_t val, bool isOutput) {
    assert(func != nullptr);
    Function& function = func->impl;

    auto param = std::make_shared<ParamInt32>(static_cast<int32_t>(val));
    auto idx = function.addParam(param, isOutput);
    return idx;
}

int TWEthereumAbiFunctionAddParamUInt256(struct TWEthereumAbiFunction *_Nonnull func, TWData *_Nonnull val, bool isOutput) {
    assert(func != nullptr);
    Function& function = func->impl;

    uint256_t val2 = load(*static_cast<const Data*>(val));

    auto param = std::make_shared<ParamUInt256>(val2);
    auto idx = function.addParam(param, isOutput);
    return idx;
}

int TWEthereumAbiFunctionAddParamBool(struct TWEthereumAbiFunction *_Nonnull func, bool val, bool isOutput) {
    assert(func != nullptr);
    Function& function = func->impl;

    auto param = std::make_shared<ParamBool>(val);
    auto idx = function.addParam(param, isOutput);
    return idx;
}

int TWEthereumAbiFunctionAddParamString(struct TWEthereumAbiFunction *_Nonnull func, TWString *_Nonnull val, bool isOutput) {
    assert(func != nullptr);
    Function& function = func->impl;

    assert(val != nullptr);
    auto param = std::make_shared<ParamString>(TWStringUTF8Bytes(val));
    auto idx = function.addParam(param, isOutput);
    return idx;
}

int TWEthereumAbiFunctionAddParamByteArray(struct TWEthereumAbiFunction *_Nonnull func, TWData *_Nonnull val, bool isOutput) {
    assert(func != nullptr);
    Function& function = func->impl;

    Data data = TW::data(TWDataBytes(val), TWDataSize(val));

    auto param = std::make_shared<ParamByteArray>(data);
    auto idx = function.addParam(param, isOutput);
    return idx;    
}

int TWEthereumAbiFunctionAddParamByteArrayFix(struct TWEthereumAbiFunction *_Nonnull func, size_t count, TWData *_Nonnull val, bool isOutput) {
    assert(func != nullptr);
    Function& function = func->impl;

    Data data = TW::data(TWDataBytes(val), TWDataSize(val));

    auto param = std::make_shared<ParamByteArrayFix>(count, data);
    auto idx = function.addParam(param, isOutput);
    return idx;    
}

int TWEthereumAbiFunctionAddParamArray(struct TWEthereumAbiFunction *_Nonnull func, bool isOutput) {
    assert(func != nullptr);
    Function& function = func->impl;

    auto param = std::make_shared<ParamArray>();
    auto idx = function.addParam(param, isOutput);
    return idx;    
}

///// GetParam

uint64_t TWEthereumAbiFunctionGetParamInt32(struct TWEthereumAbiFunction *_Nonnull func, int idx, bool isOutput) {
    assert(func != nullptr);
    Function& function = func->impl;

    std::shared_ptr<ParamBase> param;
    if (!function.getParam(idx, param, isOutput)) {
        return 0;
    }
    uint64_t val = (std::dynamic_pointer_cast<ParamInt32>(param))->getVal();
    return val;
}

TWData *_Nonnull TWEthereumAbiFunctionGetParamUInt256(struct TWEthereumAbiFunction *_Nonnull func, int idx, bool isOutput) {
    assert(func != nullptr);
    Function& function = func->impl;

    TW::Data valDat;
    std::shared_ptr<ParamBase> param;
    if (!function.getParam(idx, param, isOutput)) {
        return TWDataCreateWithData(&valDat);
    }
    uint256_t val256 = (std::dynamic_pointer_cast<ParamUInt256>(param))->getVal();
    valDat = TW::store(val256);
    return TWDataCreateWithData(&valDat);
}

///// AddInArrayParam

int TWEthereumAbiFunctionAddInArrayParamUInt32(struct TWEthereumAbiFunction *_Nonnull func, int arrayIdx, uint64_t val) {
    assert(func != nullptr);
    Function& function = func->impl;

    std::shared_ptr<ParamBase> param;
    if (!function.getInParam(arrayIdx, param)) {
        return -1;
    }
    std::shared_ptr<ParamArray> paramArr = std::dynamic_pointer_cast<ParamArray>(param);
    auto paramChild = std::make_shared<ParamUInt32>(static_cast<uint32_t>(val));
    int idxChild = paramArr->addParam(paramChild);
    return idxChild;
}

int TWEthereumAbiFunctionAddInArrayParamUInt256(struct TWEthereumAbiFunction *_Nonnull func, int arrayIdx, TWData *_Nonnull val) {
    assert(func != nullptr);
    Function& function = func->impl;

    std::shared_ptr<ParamBase> param;
    if (!function.getInParam(arrayIdx, param)) {
        return -1;
    }
    std::shared_ptr<ParamArray> paramArr = std::dynamic_pointer_cast<ParamArray>(param);
    uint256_t val2 = load(*static_cast<const Data*>(val));

    auto paramChild = std::make_shared<ParamUInt256>(val2);
    int idxChild = paramArr->addParam(paramChild);
    return idxChild;
}
