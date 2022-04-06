// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#include "Ethereum/ABI/Array.h"
#include "Ethereum/ABI/Bytes.h"
#include "Ethereum/ABI/Function.h"
#include "Ethereum/ABI/ParamAddress.h"
#include "Ethereum/ABI/ParamBase.h"
#include "Ethereum/ABI/ParamFactory.h"
#include "Ethereum/ABI/ParamNumber.h"
#include "Ethereum/ABI/ParamStruct.h"
#include "Ethereum/ABI/Parameters.h"
#include "Ethereum/ABI/Tuple.h"
#include "Ethereum/ABI/ValueDecoder.h"
#include "Ethereum/ABI/ValueEncoder.h"
#include "Ethereum/RLP.h"
#include "TrustWalletCore/TWEthereumChainID.h"
#include <emscripten/bind.h>

using namespace emscripten;
using namespace TW;
using namespace TW::Ethereum::ABI;
using namespace TW::Ethereum;

bool decodeParamBase(ParamBase& param, const Data& encoded) {
    size_t offset = 0;
    return param.decode(encoded, offset);
}

bool decodeParamArray(ParamArray& param, const Data& encoded) {
    size_t offset = 0;
    return param.decode(encoded, offset);
}

bool decodeParamByteArray(ParamByteArray& param, const Data& encoded) {
    size_t offset = 0;
    return param.decode(encoded, offset);
}

bool decodeBytesForParamByteArray(ParamByteArray& param, const Data& encoded, Data& decoded) {
    size_t offset = 0;
    return param.decodeBytes(encoded, decoded, offset);
}

bool decodeParamByteArrayFix(ParamByteArrayFix& param, const Data& encoded) {
    size_t offset = 0;
    return param.decode(encoded, offset);
}

bool decodeBytesFixForParamByteArrayFix(ParamByteArrayFix& param, const Data& encoded, size_t n,
                                        Data& decoded) {
    size_t offset = 0;
    return param.decodeBytesFix(encoded, n, decoded, offset);
}

bool decodeParamString(ParamString& param, const Data& encoded) {
    size_t offset = 0;
    return param.decode(encoded, offset);
}

std::string decodeStringForParamString(ParamString& param, const Data& encoded) {
    size_t offset = 0;
    std::string decoded = "";
    param.decodeString(encoded, decoded, offset);
    return decoded;
}

bool decodeOutputForFunction(Function& func, const Data& encoded) {
    size_t offset = 0;
    return func.decodeOutput(encoded, offset);
}

bool decodeInputForFunction(Function& func, const Data& encoded) {
    size_t offset = 0;
    return func.decodeInput(encoded, offset);
}

bool decodeParamUInt256(ParamUInt256& param, const Data& encoded) {
    size_t offset = 0;
    return param.decode(encoded, offset);
}

bool decodeParamInt256(ParamInt256& param, const Data& encoded) {
    size_t offset = 0;
    return param.decode(encoded, offset);
}

bool decodeParamBool(ParamBool& param, const Data& encoded) {
    size_t offset = 0;
    return param.decode(encoded, offset);
}

bool decodeParamUInt8(ParamUInt8& param, const Data& encoded) {
    size_t offset = 0;
    return param.decode(encoded, offset);
}

bool decodeParamInt8(ParamInt8& param, const Data& encoded) {
    size_t offset = 0;
    return param.decode(encoded, offset);
}

bool decodeParamUInt16(ParamUInt16& param, const Data& encoded) {
    size_t offset = 0;
    return param.decode(encoded, offset);
}

bool decodeParamInt16(ParamInt16& param, const Data& encoded) {
    size_t offset = 0;
    return param.decode(encoded, offset);
}

bool decodeParamUInt32(ParamUInt32& param, const Data& encoded) {
    size_t offset = 0;
    return param.decode(encoded, offset);
}

bool decodeParamInt32(ParamInt32& param, const Data& encoded) {
    size_t offset = 0;
    return param.decode(encoded, offset);
}

bool decodeParamUInt64(ParamUInt64& param, const Data& encoded) {
    size_t offset = 0;
    return param.decode(encoded, offset);
}

bool decodeParamInt64(ParamInt64& param, const Data& encoded) {
    size_t offset = 0;
    return param.decode(encoded, offset);
}

bool decodeParamUIntN(ParamUIntN& param, const Data& encoded) {
    size_t offset = 0;
    return param.decode(encoded, offset);
}

bool decodeParamIntN(ParamIntN& param, const Data& encoded) {
    size_t offset = 0;
    return param.decode(encoded, offset);
}

bool decodeParamNamed(ParamNamed& param, const Data& encoded) {
    size_t offset = 0;
    return param.decode(encoded, offset);
}

bool decodeParamSet(ParamSet& param, const Data& encoded) {
    size_t offset = 0;
    return param.decode(encoded, offset);
}

bool decodeParameters(Parameters& param, const Data& encoded) {
    size_t offset = 0;
    return param.decode(encoded, offset);
}

bool decodeParamTuple(ParamTuple& param, const Data& encoded) {
    size_t offset = 0;
    return param.decode(encoded, offset);
}

EMSCRIPTEN_BINDINGS(wallet_ethereum_module) {
    class_<ParamBase>("ParamBase")
        .function("getType", &ParamBase::getType)
        .function("getSize", &ParamBase::getSize)
        .function("isDynamic", &ParamBase::isDynamic)
        .function("encode", &ParamBase::encode)
        .function("setValueJson", &ParamBase::setValueJson)
        .function("hashStruct", &ParamBase::hashStruct)
        .function("getExtraTypes", &ParamBase::getExtraTypes);

    function("decodeParamBase", &decodeParamBase);

    class_<ParamCollection>("ParamCollection").function("getCount", &ParamArray::getCount);

    class_<ParamArray>("ParamArray")
        .constructor<>()
        .constructor<const std::vector<std::shared_ptr<ParamBase>>&>()
        .function("setVal", &ParamArray::setVal)
        .function("getVal", &ParamArray::getVal)
        .function("addParam", &ParamArray::addParam)
        .function("addParams", &ParamArray::addParams)
        .function("getFirstType", &ParamArray::getFirstType)
        .function("getParam", &ParamArray::getFirstType)
        .function("getType", &ParamArray::getType)
        .function("getSize", &ParamArray::getSize)
        .function("isDynamic", &ParamArray::isDynamic)
        .function("getCount", &ParamArray::getCount)
        .function("encode", &ParamArray::encode)
        .function("setValueJson", &ParamArray::setValueJson)
        .function("hashStruct", &ParamArray::hashStruct);
    function("decodeParamArray", &decodeParamArray);

    class_<ParamByteArray>("ParamByteArray")
        .constructor<>()
        .constructor<const Data&>()
        .function("setVal", &ParamByteArray::setVal)
        .function("getVal", &ParamByteArray::getVal)
        .function("getType", &ParamByteArray::getType)
        .function("getSize", &ParamByteArray::getSize)
        .function("isDynamic", &ParamByteArray::isDynamic)
        .function("getCount", &ParamByteArray::getCount)
        .function("encodeBytes", &ParamByteArray::encodeBytes)
        .function("encode", &ParamByteArray::encode)
        .function("setValueJson", &ParamByteArray::setValueJson)
        .function("hashStruct", &ParamByteArray::hashStruct);
    function("decodeParamByteArray", &decodeParamByteArray);
    function("decodeBytesParamByteArray", &decodeBytesForParamByteArray);

    class_<ParamByteArrayFix>("ParamByteArrayFix")
        .constructor<size_t>()
        .constructor<size_t, const Data&>()
        .function("setVal", &ParamByteArrayFix::setVal)
        .function("getVal", &ParamByteArrayFix::getVal)
        .function("getType", &ParamByteArrayFix::getType)
        .function("getSize", &ParamByteArrayFix::getSize)
        .function("isDynamic", &ParamByteArrayFix::isDynamic)
        .function("getCount", &ParamByteArrayFix::getCount)
        .function("encode", &ParamByteArrayFix::encode)
        .function("setValueJson", &ParamByteArrayFix::setValueJson)
        .function("hashStruct", &ParamByteArrayFix::hashStruct);
    function("decodeParamByteArrayFix", &decodeParamByteArrayFix);
    function("decodeBytesFixForParamByteArrayFix", &decodeBytesFixForParamByteArrayFix);

    class_<ParamString>("ParamString")
        .constructor<>()
        .constructor<std::string>()
        .function("setVal", &ParamString::setVal)
        .function("getVal", &ParamString::getVal)
        .function("getType", &ParamString::getType)
        .function("getSize", &ParamString::getSize)
        .function("isDynamic", &ParamString::isDynamic)
        .function("getCount", &ParamString::getCount)
        .function("encodeString", &ParamString::encodeString)
        .function("encode", &ParamString::encode)
        .function("setValueJson", &ParamString::setValueJson)
        .function("hashStruct", &ParamString::hashStruct);
    function("decodeParamString", &decodeParamString);
    function("decodeStringForParamString", &decodeStringForParamString);

    class_<Function>("Function")
        .constructor<std::string>()
        .constructor<std::string, const std::vector<std::shared_ptr<ParamBase>>&>()
        .function("addInParam", &Function::addInParam)
        .function("addOutParam", &Function::addOutParam)
        .function("addParam", &Function::addParam)
        .function("getInParam", &Function::getInParam)
        .function("getOutParam", &Function::getOutParam)
        .function("getParam", &Function::getParam)
        .function("getType", &Function::getType)
        .function("getSignature", &Function::getSignature)
        .function("encode", &Function::encode);
    function("decodeOutputForFunction", &decodeOutputForFunction);
    function("decodeInputForFunction", &decodeInputForFunction);

    class_<ParamAddress>("ParamAddress")
        .constructor<>()
        .constructor<const Data&>()
        .function("getType", &ParamAddress::getType)
        .function("getData", &ParamAddress::getData)
        .function("setValueJson", &ParamAddress::setValueJson);

    class_<ParamFactory>("ParamFactory")
        .class_function("make", &ParamFactory::make)
        .class_function("makeNamed", &ParamFactory::makeNamed)
        .class_function("getValue", &ParamFactory::getValue)
        .class_function("getArrayValue", &ParamFactory::getArrayValue);

    class_<ParamUInt256>("ParamUInt256")
        .constructor<>()
        .constructor<uint256_t>()
        .function("setVal", &ParamUInt256::setVal)
        .function("getType", &ParamUInt256::getType)
        .function("getVal", &ParamUInt256::getVal)
        .function("getSize", &ParamUInt256::getSize)
        .function("isDynamic", &ParamUInt256::isDynamic)
        .function("encode", &ParamUInt256::encode)
        .function("setValueJson", &ParamUInt256::setValueJson)
        .function("setUInt256FromValueJson", &ParamUInt256::setUInt256FromValueJson);
    function("decodeParamUInt256", &decodeParamUInt256);

    class_<ParamInt256>("ParamInt256")
        .constructor<>()
        .constructor<int256_t>()
        .function("setVal", &ParamInt256::setVal)
        .function("getType", &ParamInt256::getType)
        .function("getVal", &ParamInt256::getVal)
        .function("getSize", &ParamInt256::getSize)
        .function("isDynamic", &ParamInt256::isDynamic)
        .function("encode", &ParamInt256::encode)
        .function("setValueJson", &ParamInt256::setValueJson)
        .function("setInt256FromValueJson", &ParamInt256::setInt256FromValueJson);
    function("decodeParamInt256", &decodeParamInt256);

    class_<ParamBool>("ParamBool")
        .constructor<>()
        .constructor<bool>()
        .function("setVal", &ParamBool::setVal)
        .function("getType", &ParamBool::getType)
        .function("getVal", &ParamBool::getVal)
        .function("getSize", &ParamBool::getSize)
        .function("isDynamic", &ParamBool::isDynamic)
        .function("encode", &ParamBool::encode)
        .function("setValueJson", &ParamBool::setValueJson);
    function("decodeParamBool", &decodeParamBool);

    class_<ParamUInt8>("ParamUInt8")
        .constructor<>()
        .constructor<uint8_t>()
        .function("setVal", &ParamUInt8::setVal)
        .function("getType", &ParamUInt8::getType)
        .function("getVal", &ParamUInt8::getVal)
        .function("getSize", &ParamUInt8::getSize)
        .function("isDynamic", &ParamUInt8::isDynamic)
        .function("encode", &ParamUInt8::encode)
        .function("setValueJson", &ParamUInt8::setValueJson);
    function("decodeParamUInt8", &decodeParamUInt8);

    class_<ParamInt8>("ParamInt8")
        .constructor<>()
        .constructor<int8_t>()
        .function("setVal", &ParamInt8::setVal)
        .function("getType", &ParamInt8::getType)
        .function("getVal", &ParamInt8::getVal)
        .function("getSize", &ParamInt8::getSize)
        .function("isDynamic", &ParamInt8::isDynamic)
        .function("encode", &ParamInt8::encode)
        .function("setValueJson", &ParamInt8::setValueJson);
    function("decodeParamInt8", &decodeParamInt8);

    class_<ParamUInt16>("ParamUInt16")
        .constructor<>()
        .constructor<uint16_t>()
        .function("setVal", &ParamUInt16::setVal)
        .function("getType", &ParamUInt16::getType)
        .function("getVal", &ParamUInt16::getVal)
        .function("getSize", &ParamUInt16::getSize)
        .function("isDynamic", &ParamUInt16::isDynamic)
        .function("encode", &ParamUInt16::encode)
        .function("setValueJson", &ParamUInt16::setValueJson);
    function("decodeParamUInt16", &decodeParamUInt16);

    class_<ParamInt16>("ParamInt16")
        .constructor<>()
        .constructor<int16_t>()
        .function("setVal", &ParamInt16::setVal)
        .function("getType", &ParamInt16::getType)
        .function("getVal", &ParamInt16::getVal)
        .function("getSize", &ParamInt16::getSize)
        .function("isDynamic", &ParamInt16::isDynamic)
        .function("encode", &ParamInt16::encode)
        .function("setValueJson", &ParamInt16::setValueJson);
    function("decodeParamInt16", &decodeParamInt16);

    class_<ParamUInt32>("ParamUInt32")
        .constructor<>()
        .constructor<uint32_t>()
        .function("setVal", &ParamUInt32::setVal)
        .function("getType", &ParamUInt32::getType)
        .function("getVal", &ParamUInt32::getVal)
        .function("getSize", &ParamUInt32::getSize)
        .function("isDynamic", &ParamUInt32::isDynamic)
        .function("encode", &ParamUInt32::encode)
        .function("setValueJson", &ParamUInt32::setValueJson);
    function("decodeParamUInt32", &decodeParamUInt32);

    class_<ParamInt32>("ParamInt32")
        .constructor<>()
        .constructor<int32_t>()
        .function("setVal", &ParamInt32::setVal)
        .function("getType", &ParamInt32::getType)
        .function("getVal", &ParamInt32::getVal)
        .function("getSize", &ParamInt32::getSize)
        .function("isDynamic", &ParamInt32::isDynamic)
        .function("encode", &ParamInt32::encode)
        .function("setValueJson", &ParamInt32::setValueJson);
    function("decodeParamInt32", &decodeParamInt32);

    class_<ParamUInt64>("ParamUInt64")
        .constructor<>()
        .constructor<uint64_t>()
        .function("setVal", &ParamUInt64::setVal)
        .function("getType", &ParamUInt64::getType)
        .function("getVal", &ParamUInt64::getVal)
        .function("getSize", &ParamUInt64::getSize)
        .function("isDynamic", &ParamUInt64::isDynamic)
        .function("encode", &ParamUInt64::encode)
        .function("setValueJson", &ParamUInt64::setValueJson);
    function("decodeParamUInt64", &decodeParamUInt64);

    class_<ParamInt64>("ParamInt64")
        .constructor<>()
        .constructor<int64_t>()
        .function("setVal", &ParamInt64::setVal)
        .function("getType", &ParamInt64::getType)
        .function("getVal", &ParamInt64::getVal)
        .function("getSize", &ParamInt64::getSize)
        .function("isDynamic", &ParamInt64::isDynamic)
        .function("encode", &ParamInt64::encode)
        .function("setValueJson", &ParamInt64::setValueJson);
    function("decodeParamInt64", &decodeParamInt64);

    class_<ParamUIntN>("ParamUIntN")
        .constructor<size_t>()
        .constructor<size_t, uint256_t>()
        .function("setVal", &ParamUIntN::setVal)
        .function("getType", &ParamUIntN::getType)
        .function("getVal", &ParamUIntN::getVal)
        .function("getSize", &ParamUIntN::getSize)
        .function("isDynamic", &ParamUIntN::isDynamic)
        .function("encode", &ParamUIntN::encode)
        .function("setValueJson", &ParamUIntN::setValueJson)
        .function("maskForBits", &ParamUIntN::maskForBits);
    function("decodeParamUIntN", &decodeParamUIntN);

    class_<ParamIntN>("ParamIntN")
        .constructor<size_t>()
        .constructor<size_t, int256_t>()
        .function("setVal", &ParamIntN::setVal)
        .function("getType", &ParamIntN::getType)
        .function("getVal", &ParamIntN::getVal)
        .function("getSize", &ParamIntN::getSize)
        .function("isDynamic", &ParamIntN::isDynamic)
        .function("encode", &ParamIntN::encode)
        .function("setValueJson", &ParamIntN::setValueJson);
    function("decodeParamIntN", &decodeParamIntN);

    class_<ParamNamed>("ParamNamed")
        .property("_name", &ParamNamed::_name)
        .property("_param", &ParamNamed::_param)
        .constructor<const std::string&, std::shared_ptr<ParamBase>>()
        .function("getName", &ParamNamed::getName)
        .function("getType", &ParamNamed::getType)
        .function("getSize", &ParamNamed::getSize)
        .function("isDynamic", &ParamNamed::isDynamic)
        .function("encode", &ParamNamed::encode)
        .function("setValueJson", &ParamNamed::setValueJson)
        .function("hashStruct", &ParamNamed::hashStruct)
        .function("getExtraTypes", &ParamNamed::getExtraTypes);
    function("decodeParamNamed", &decodeParamNamed);

    class_<ParamSet>("ParamSet")
        .constructor<>()
        .constructor<const std::vector<std::shared_ptr<ParamBase>>&>()
        .function("addParam", &ParamSet::addParam)
        .function("addParams", &ParamSet::addParams)
        .function("getParam", &ParamSet::getParam)
        .function("getParamUnsafe", &ParamSet::getParamUnsafe)
        .function("getCount", &ParamSet::getCount)
        .function("getParams", &ParamSet::getParams)
        .function("getType", &ParamSet::getType)
        .function("isDynamic", &ParamSet::isDynamic)
        .function("getSize", &ParamSet::getSize)
        .function("encode", &ParamSet::encode)
        .function("encodeHashes", &ParamSet::encodeHashes);
    function("decodeParamSet", &decodeParamSet);

    class_<Parameters>("Parameters")
        .constructor<>()
        .constructor<const std::vector<std::shared_ptr<ParamBase>>&>()
        .function("addParam", &Parameters::addParam)
        .function("addParams", &Parameters::addParams)
        .function("getParam", &Parameters::getParam)
        .function("getCount", &Parameters::getCount)
        .function("getType", &Parameters::getType)
        .function("isDynamic", &Parameters::isDynamic)
        .function("getSize", &Parameters::getSize)
        .function("encode", &Parameters::encode)
        .function("setValueJson", &Parameters::setValueJson)
        .function("hashStruct", &Parameters::hashStruct);
    function("decodeParameters", &decodeParameters);

    class_<ParamTuple>("ParamTuple")
        .constructor<>()
        .constructor<const std::vector<std::shared_ptr<ParamBase>>&>()
        .function("addParam", &ParamTuple::addParam)
        .function("getParam", &ParamTuple::getParam)
        .function("getCount", &ParamTuple::getCount)
        .function("getType", &ParamTuple::getType)
        .function("isDynamic", &ParamTuple::isDynamic)
        .function("getSize", &ParamTuple::getSize)
        .function("encode", &ParamTuple::encode)
        .function("setValueJson", &ParamTuple::setValueJson)
        .function("hashStruct", &ParamTuple::hashStruct);
    function("decodeParamTuple", &decodeParamTuple);

    enum_<TWEthereumChainID>("TWEthereumChainID")
        .value("TWEthereumChainIDEthereum", TWEthereumChainID::TWEthereumChainIDEthereum)
        .value("TWEthereumChainIDGo", TWEthereumChainID::TWEthereumChainIDGo)
        .value("TWEthereumChainIDPOA", TWEthereumChainID::TWEthereumChainIDPOA)
        .value("TWEthereumChainIDCallisto", TWEthereumChainID::TWEthereumChainIDCallisto)
        .value("TWEthereumChainIDEthereumClassic",
               TWEthereumChainID::TWEthereumChainIDEthereumClassic)
        .value("TWEthereumChainIDVeChain", TWEthereumChainID::TWEthereumChainIDVeChain)
        .value("TWEthereumChainIDThunderToken", TWEthereumChainID::TWEthereumChainIDThunderToken)
        .value("TWEthereumChainIDTomoChain", TWEthereumChainID::TWEthereumChainIDTomoChain)
        .value("TWEthereumChainIDBinanceSmartChain",
               TWEthereumChainID::TWEthereumChainIDBinanceSmartChain)
        .value("TWEthereumChainIDPolygon", TWEthereumChainID::TWEthereumChainIDPolygon)
        .value("TWEthereumChainIDWanchain", TWEthereumChainID::TWEthereumChainIDWanchain)
        .value("TWEthereumChainIDOptimism", TWEthereumChainID::TWEthereumChainIDOptimism)
        .value("TWEthereumChainIDArbitrum", TWEthereumChainID::TWEthereumChainIDArbitrum)
        .value("TWEthereumChainIDHeco", TWEthereumChainID::TWEthereumChainIDHeco)
        .value("TWEthereumChainIDAvalanche", TWEthereumChainID::TWEthereumChainIDAvalanche)
        .value("TWEthereumChainIDXDai", TWEthereumChainID::TWEthereumChainIDXDai)
        .value("TWEthereumChainIDFantom", TWEthereumChainID::TWEthereumChainIDFantom)
        .value("TWEthereumChainIDCelo", TWEthereumChainID::TWEthereumChainIDCelo)
        .value("TWEthereumChainIDRonin", TWEthereumChainID::TWEthereumChainIDRonin);

    class_<RLP>("RLP")
        .class_function("encode", select_overload<Data(const std::string&)>(&RLP::encode))
        .class_function("encode", select_overload<Data(uint8_t)>(&RLP::encode))
        .class_function("encode", select_overload<Data(uint16_t)>(&RLP::encode))
        .class_function("encode", select_overload<Data(int32_t)>(&RLP::encode))
        .class_function("encode", select_overload<Data(uint32_t)>(&RLP::encode))
        .class_function("encode", select_overload<Data(int64_t)>(&RLP::encode))
        .class_function("encode", select_overload<Data(uint64_t)>(&RLP::encode))
        .class_function("encode", select_overload<Data(const uint256_t&)>(&RLP::encode))
        .class_function("encode", select_overload<Data(const Data&)>(&RLP::encode))
        .class_function("encodeList", select_overload<Data(const Data&)>(&RLP::encodeList))
        .class_function("encodeHeader", &RLP::encodeHeader)
        .class_function("decodeList", &RLP::decodeList)
        .class_function("decode", &RLP::decode)
        .class_function("putVarInt", &RLP::putVarInt)
        .class_function("parseVarInt", &RLP::parseVarInt);

    class_<RLP::DecodedItem>("DecodedItem")
        .property("decoded", &RLP::DecodedItem::decoded)
        .property("remainder", &RLP::DecodedItem::remainder);

    class_<ValueEncoder>("ValueEncoder")
        .class_function("encodeBool", &ValueEncoder::encodeBool)
        .class_function("encodeInt32", &ValueEncoder::encodeInt32)
        .class_function("encodeUInt32", &ValueEncoder::encodeUInt32)
        .class_function("encodeInt256", &ValueEncoder::encodeInt256)
        .class_function("encodeUInt256", &ValueEncoder::encodeUInt256)
        .class_function("encodeAddress", &ValueEncoder::encodeAddress)
        .class_function("encodeString", &ValueEncoder::encodeString)
        .class_function("encodeBytes", &ValueEncoder::encodeBytes)
        .class_function("encodeBytesDyn", &ValueEncoder::encodeBytesDyn)
        .class_function("int256FromUint256", &ValueEncoder::int256FromUint256)
        .class_function("uint256FromInt256", &ValueEncoder::uint256FromInt256)
        .class_function("paddedTo32", &ValueEncoder::paddedTo32);

    class_<ValueDecoder>("ValueDecoder")
        .class_function("decodeUInt256", &ValueDecoder::decodeUInt256)
        .class_function("decodeValue", &ValueDecoder::decodeValue)
        .class_function("decodeArray", &ValueDecoder::decodeArray);
}
