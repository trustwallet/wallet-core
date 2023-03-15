// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWEthereumAbiValue.h>

#include <Ethereum/ABI/ValueEncoder.h>
#include <Ethereum/ABI/ValueDecoder.h>
#include <Data.h>

using namespace TW;

namespace EthAbi = TW::Ethereum::ABI;

TWData* _Nonnull TWEthereumAbiValueEncodeBool(bool value) {
    Data data;
    EthAbi::ValueEncoder::encodeBool(value, data);
    return TWDataCreateWithBytes(data.data(), data.size());
}

TWData* _Nonnull TWEthereumAbiValueEncodeInt32(int32_t value) {
    Data data;
    EthAbi::ValueEncoder::encodeInt32(value, data);
    return TWDataCreateWithBytes(data.data(), data.size());
}

TWData* _Nonnull TWEthereumAbiValueEncodeUInt32(uint32_t value) {
    Data data;
    EthAbi::ValueEncoder::encodeUInt32(value, data);
    return TWDataCreateWithBytes(data.data(), data.size());
}

TWData* _Nonnull TWEthereumAbiValueEncodeInt256(TWData* _Nonnull value) {
    Data data;
    int256_t value256 = static_cast<int256_t>(TW::load(*reinterpret_cast<const Data*>(value)));
    EthAbi::ValueEncoder::encodeInt256(value256, data);
    return TWDataCreateWithBytes(data.data(), data.size());
}

TWData* _Nonnull TWEthereumAbiValueEncodeUInt256(TWData* _Nonnull value) {
    Data data;
    uint256_t value256 = TW::load(*reinterpret_cast<const Data*>(value));
    EthAbi::ValueEncoder::encodeUInt256(value256, data);
    return TWDataCreateWithBytes(data.data(), data.size());
}

TWData* _Nonnull TWEthereumAbiValueEncodeAddress(TWData* _Nonnull value) {
    Data data;
    EthAbi::ValueEncoder::encodeAddress(*reinterpret_cast<const Data*>(value), data);
    return TWDataCreateWithBytes(data.data(), data.size());
}

TWData* _Nonnull TWEthereumAbiValueEncodeString(TWString* _Nonnull value) {
    Data data;
    EthAbi::ValueEncoder::encodeString(TWStringUTF8Bytes(value), data);
    return TWDataCreateWithBytes(data.data(), data.size());
}

TWData* _Nonnull TWEthereumAbiValueEncodeBytes(TWData* _Nonnull value) {
    Data data;
    EthAbi::ValueEncoder::encodeBytes(*reinterpret_cast<const Data*>(value), data);
    return TWDataCreateWithBytes(data.data(), data.size());
}

TWData* _Nonnull TWEthereumAbiValueEncodeBytesDyn(TWData* _Nonnull value) {
    Data data;
    EthAbi::ValueEncoder::encodeBytesDyn(*reinterpret_cast<const Data*>(value), data);
    return TWDataCreateWithBytes(data.data(), data.size());
}

TWString* _Nonnull TWEthereumAbiValueDecodeUInt256(TWData* _Nonnull input) {
    auto data = TW::data(TWDataBytes(input), TWDataSize(input));
    auto decoded = Ethereum::ABI::ValueDecoder::decodeUInt256(data);
    return TWStringCreateWithUTF8Bytes(TW::toString(decoded).c_str());
}

TWString* _Nonnull TWEthereumAbiValueDecodeValue(TWData* _Nonnull input, TWString* _Nonnull type) {
    auto data = TW::data(TWDataBytes(input), TWDataSize(input));
    auto value = Ethereum::ABI::ValueDecoder::decodeValue(data, TWStringUTF8Bytes(type));
    return TWStringCreateWithUTF8Bytes(value.c_str());
}

TWString* _Nonnull TWEthereumAbiValueDecodeArray(TWData* _Nonnull input, TWString* _Nonnull type) {
    auto data = TW::data(TWDataBytes(input), TWDataSize(input));
    auto valueString = Ethereum::ABI::ValueDecoder::decodeValue(data, TWStringUTF8Bytes(type));
    return TWStringCreateWithUTF8Bytes(valueString.c_str());
}
