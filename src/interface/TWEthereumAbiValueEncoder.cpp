// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWEthereumAbiValueEncoder.h>

#include <Ethereum/ABI/ValueEncoder.h>
#include <Data.h>

#include <iostream>

using namespace TW::Ethereum;
using namespace TW;


TWData* _Nonnull TWEthereumAbiValueEncoderEncodeBool(bool value) {
    Data data;
    ABI::ValueEncoder::encodeBool(value, data);
    return TWDataCreateWithBytes(data.data(), data.size());
}

TWData* _Nonnull TWEthereumAbiValueEncoderEncodeInt32(int32_t value) {
    Data data;
    ABI::ValueEncoder::encodeInt32(value, data);
    return TWDataCreateWithBytes(data.data(), data.size());
}

TWData* _Nonnull TWEthereumAbiValueEncoderEncodeUInt32(uint32_t value) {
    Data data;
    ABI::ValueEncoder::encodeUInt32(value, data);
    return TWDataCreateWithBytes(data.data(), data.size());
}

TWData* _Nonnull TWEthereumAbiValueEncoderEncodeInt256(TWData* _Nonnull value) {
    Data data;
    int256_t value256 = static_cast<int256_t>(TW::load(*reinterpret_cast<const Data*>(value)));
    ABI::ValueEncoder::encodeInt256(value256, data);
    return TWDataCreateWithBytes(data.data(), data.size());
}

TWData* _Nonnull TWEthereumAbiValueEncoderEncodeUInt256(TWData* _Nonnull value) {
    Data data;
    uint256_t value256 = TW::load(*reinterpret_cast<const Data*>(value));
    ABI::ValueEncoder::encodeUInt256(value256, data);
    return TWDataCreateWithBytes(data.data(), data.size());
}

TWData* _Nonnull TWEthereumAbiValueEncoderEncodeAddress(TWData* _Nonnull value) {
    Data data;
    ABI::ValueEncoder::encodeAddress(*reinterpret_cast<const Data*>(value), data);
    return TWDataCreateWithBytes(data.data(), data.size());
}

TWData* _Nonnull TWEthereumAbiValueEncoderEncodeString(TWString* _Nonnull value) {
    Data data;
    ABI::ValueEncoder::encodeString(TWStringUTF8Bytes(value), data);
    return TWDataCreateWithBytes(data.data(), data.size());
}

TWData* _Nonnull TWEthereumAbiValueEncoderEncodeBytes(TWData* _Nonnull value) {
    Data data;
    ABI::ValueEncoder::encodeBytes(*reinterpret_cast<const Data*>(value), data);
    return TWDataCreateWithBytes(data.data(), data.size());
}

TWData* _Nonnull TWEthereumAbiValueEncoderEncodeBytesDyn(TWData* _Nonnull value) {
    Data data;
    ABI::ValueEncoder::encodeBytesDyn(*reinterpret_cast<const Data*>(value), data);
    return TWDataCreateWithBytes(data.data(), data.size());
}
