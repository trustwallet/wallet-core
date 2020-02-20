// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWEthereumEIP712.h>

#include <Ethereum/EIP712/Encoder.h>
#include <Data.h>

using namespace TW::Ethereum;
using namespace TW;

/// Helper
void fillTWData(const Data& value, TWData* _Nonnull data) {
    const Data* dataAsData = reinterpret_cast<const Data*>(data);
    if (dataAsData->size() >= value.size()) {
        std::copy(value.begin(), value.end(), (const_cast<Data*>(dataAsData))->begin());
    }
}

void TWEthereumEIP712EncodeBool(bool value, TWData* _Nonnull data_inout) {
    fillTWData(EIP712::Encoder::encodeBool(value), data_inout);
}

void TWEthereumEIP712EncodeInt32(int32_t value, TWData* _Nonnull data_inout) {
    fillTWData(EIP712::Encoder::encodeInt32(value), data_inout);
}

void TWEthereumEIP712EncodeUInt32(uint32_t value, TWData* _Nonnull data_inout) {
    fillTWData(EIP712::Encoder::encodeUInt32(value), data_inout);
}

void TWEthereumEIP712EncodeInt256(TWData* _Nonnull value, TWData* _Nonnull data_inout) {
    int256_t value256 = static_cast<int256_t>(TW::load(*reinterpret_cast<const Data*>(value)));
    fillTWData(EIP712::Encoder::encodeInt256(value256), data_inout);
}

void TWEthereumEIP712EncodeUInt256(TWData* _Nonnull value, TWData* _Nonnull data_inout) {
    uint256_t value256 = TW::load(*reinterpret_cast<const Data*>(value));
    fillTWData(EIP712::Encoder::encodeUInt256(value256), data_inout);
}

void TWEthereumEIP712EncodeAddress(TWData* _Nonnull value, TWData* _Nonnull data_inout) {
    fillTWData(EIP712::Encoder::encodeAddress(*reinterpret_cast<const Data*>(value)), data_inout);
}

void TWEthereumEIP712EncodeString(TWString* _Nonnull value, TWData* _Nonnull data_inout) {
    fillTWData(EIP712::Encoder::encodeString(TWStringUTF8Bytes(value)), data_inout);
}

void TWEthereumEIP712EncodeBytes(TWData* _Nonnull value, TWData* _Nonnull data_inout) {
    fillTWData(EIP712::Encoder::encodeBytes(*reinterpret_cast<const Data*>(value)), data_inout);
}

void TWEthereumEIP712EncodeBytesDyn(TWData* _Nonnull value, TWData* _Nonnull data_inout) {
    fillTWData(EIP712::Encoder::encodeBytesDyn(*reinterpret_cast<const Data*>(value)), data_inout);
}
