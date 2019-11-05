#include <TrustWalletCore/TWEthAbiFunction.h>

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


struct TWEthAbiFunction *_Nullable TWEthAbiFunctionCreateWithString(TWString *_Nonnull name) {
    auto func = Function(TWStringUTF8Bytes(name));
    TWStringDelete(name);
    return new TWEthAbiFunction{ func };
}

void TWEthAbiFunctionDelete(struct TWEthAbiFunction *_Nonnull func) {
    assert(func != nullptr);
    delete func;
}

TWString *_Nonnull TWEthAbiFunctionGetType(struct TWEthAbiFunction *_Nonnull func) {
    assert(func != nullptr);
    auto function = func->impl;
    std::string sign = function.getType();
    return TWStringCreateWithUTF8Bytes(sign.c_str());
}

///// AddInParam

int TWEthAbiFunctionAddInParamInt32(struct TWEthAbiFunction *_Nonnull func, uint64_t val) {
    assert(func != nullptr);
    Function& function = func->impl;

    auto param = std::make_shared<ParamInt32>(static_cast<int32_t>(val));
    auto idx = function.addInParam(param);
    return idx;
}

int TWEthAbiFunctionAddInParamUInt256(struct TWEthAbiFunction *_Nonnull func, TWData *_Nonnull val) {
    assert(func != nullptr);
    Function& function = func->impl;

    uint256_t val2 = load(*static_cast<const Data*>(val));
    TWDataDelete(val); // remove original
    auto param = std::make_shared<ParamUInt256>(val2);
    auto idx = function.addInParam(param);
    return idx;
}

int TWEthAbiFunctionAddInParamBool(struct TWEthAbiFunction *_Nonnull func, bool val) {
    assert(func != nullptr);
    Function& function = func->impl;

    auto param = std::make_shared<ParamBool>(val);
    auto idx = function.addInParam(param);
    return idx;
}

int TWEthAbiFunctionAddInParamString(struct TWEthAbiFunction *_Nonnull func, TWString *_Nonnull val) {
    assert(func != nullptr);
    Function& function = func->impl;

    assert(val != nullptr);
    auto param = std::make_shared<ParamString>(TWStringUTF8Bytes(val));
    auto idx = function.addInParam(param);
    return idx;
}

int TWEthAbiFunctionAddInParamByteArray(struct TWEthAbiFunction *_Nonnull func, TWData *_Nonnull val) {
    assert(func != nullptr);
    Function& function = func->impl;

    Data data = TW::data(TWDataBytes(val), TWDataSize(val));
    TWDataDelete(val); // remove original
    auto param = std::make_shared<ParamByteArray>(data);
    auto idx = function.addInParam(param);
    return idx;    
}

int TWEthAbiFunctionAddInParamByteArrayFix(struct TWEthAbiFunction *_Nonnull func, size_t count, TWData *_Nonnull val) {
    assert(func != nullptr);
    Function& function = func->impl;

    Data data = TW::data(TWDataBytes(val), TWDataSize(val));
    TWDataDelete(val); // remove original
    auto param = std::make_shared<ParamByteArrayFix>(count, data);
    auto idx = function.addInParam(param);
    return idx;    
}

int TWEthAbiFunctionAddInParamArray(struct TWEthAbiFunction *_Nonnull func) {
    assert(func != nullptr);
    Function& function = func->impl;

    auto param = std::make_shared<ParamArray>();
    auto idx = function.addInParam(param);
    return idx;    
}

///// AddOutParam

int TWEthAbiFunctionAddOutParamInt32(struct TWEthAbiFunction *_Nonnull func) {
    assert(func != nullptr);
    Function& function = func->impl;

    auto param = std::make_shared<ParamInt32>(0);
    auto idx = function.addOutParam(param);
    return idx;
}

int TWEthAbiFunctionAddOutParamUInt256(struct TWEthAbiFunction *_Nonnull func) {
    assert(func != nullptr);
    Function& function = func->impl;

    auto param = std::make_shared<ParamUInt256>(0);
    auto idx = function.addOutParam(param);
    return idx;
}

///// GetOutParam

uint64_t TWEthAbiFunctionGetOutParamInt32(struct TWEthAbiFunction *_Nonnull func, int idx) {
    assert(func != nullptr);
    Function& function = func->impl;

    std::shared_ptr<ParamBase> param;
    if (!function.getOutParam(idx, param)) {
        return 0;
    }
    uint64_t val = (std::dynamic_pointer_cast<ParamInt32>(param))->getVal();
    return val;
}

TWData *_Nonnull TWEthAbiFunctionGetOutParamUInt256(struct TWEthAbiFunction *_Nonnull func, int idx) {
    assert(func != nullptr);
    Function& function = func->impl;

    TW::Data valDat;
    std::shared_ptr<ParamBase> param;
    if (!function.getOutParam(idx, param)) {
        return TWDataCreateWithData(&valDat);
    }
    uint256_t val256 = (std::dynamic_pointer_cast<ParamUInt256>(param))->getVal();
    valDat = TW::store(val256);
    return TWDataCreateWithData(&valDat);
}

///// AddInArrayParam

int TWEthAbiFunctionAddInArrayParamUInt32(struct TWEthAbiFunction *_Nonnull func, int arrayIdx, uint64_t val) {
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

int TWEthAbiFunctionAddInArrayParamUInt256(struct TWEthAbiFunction *_Nonnull func, int arrayIdx, TWData *_Nonnull val) {
    assert(func != nullptr);
    Function& function = func->impl;

    std::shared_ptr<ParamBase> param;
    if (!function.getInParam(arrayIdx, param)) {
        return -1;
    }
    std::shared_ptr<ParamArray> paramArr = std::dynamic_pointer_cast<ParamArray>(param);
    uint256_t val2 = load(*static_cast<const Data*>(val));
    TWDataDelete(val); // remove original
    auto paramChild = std::make_shared<ParamUInt256>(val2);
    int idxChild = paramArr->addParam(paramChild);
    return idxChild;
}

///// Encode

TWData*_Nonnull TWEthAbiFunctionEncode(struct TWEthAbiFunction *_Nonnull func) {
    assert(func != nullptr);
    Function& function = func->impl;

    Data encoded;
    function.encode(encoded);
    return TWDataCreateWithData(&encoded);
}
