// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWEthereumAbiValueEncoder.h>

#include <Ethereum/ABI/ValueEncoder.h>
#include <Data.h>

using namespace TW::Ethereum;
using namespace TW;

/// Helper, fill exisitng TWData with Data provided.  TWData has to be large enough.
void fillTWData(const Data& value, TWData* _Nonnull data) {
    const Data* dataAsData = reinterpret_cast<const Data*>(data);
    if (dataAsData->size() >= value.size()) {
        std::copy(value.begin(), value.end(), (const_cast<Data*>(dataAsData))->begin());
    }
}

void TWEthereumAbiValueEncodeBool(bool value, TWData* _Nonnull data_inout) {
    Data data;
    ABI::ValueEncoder::encodeBool(value, data);
    fillTWData(data, data_inout);
}

void TWEthereumAbiValueEncodeInt32(int32_t value, TWData* _Nonnull data_inout) {
    Data data;
    ABI::ValueEncoder::encodeInt32(value, data);
    fillTWData(data, data_inout);
}

void TWEthereumAbiValueEncodeUInt32(uint32_t value, TWData* _Nonnull data_inout) {
    Data data;
    ABI::ValueEncoder::encodeUInt32(value, data);
    fillTWData(data, data_inout);
}

void TWEthereumAbiValueEncodeInt256(TWData* _Nonnull value, TWData* _Nonnull data_inout) {
    Data data;
    int256_t value256 = static_cast<int256_t>(TW::load(*reinterpret_cast<const Data*>(value)));
    ABI::ValueEncoder::encodeInt256(value256, data);
    fillTWData(data, data_inout);
}

void TWEthereumAbiValueEncodeUInt256(TWData* _Nonnull value, TWData* _Nonnull data_inout) {
    Data data;
    uint256_t value256 = TW::load(*reinterpret_cast<const Data*>(value));
    ABI::ValueEncoder::encodeUInt256(value256, data);
    fillTWData(data, data_inout);
}

void TWEthereumAbiValueEncodeAddress(TWData* _Nonnull value, TWData* _Nonnull data_inout) {
    Data data;
    ABI::ValueEncoder::encodeAddress(*reinterpret_cast<const Data*>(value), data);
    fillTWData(data, data_inout);
}

void TWEthereumAbiValueEncodeString(TWString* _Nonnull value, TWData* _Nonnull data_inout) {
    Data data;
    ABI::ValueEncoder::encodeString(TWStringUTF8Bytes(value), data);
    fillTWData(data, data_inout);
}

void TWEthereumAbiValueEncodeBytes(TWData* _Nonnull value, TWData* _Nonnull data_inout) {
    Data data;
    ABI::ValueEncoder::encodeBytes(*reinterpret_cast<const Data*>(value), data);
    fillTWData(data, data_inout);
}

void TWEthereumAbiValueEncodeBytesDyn(TWData* _Nonnull value, TWData* _Nonnull data_inout) {
    Data data;
    ABI::ValueEncoder::encodeBytesDyn(*reinterpret_cast<const Data*>(value), data);
    fillTWData(data, data_inout);
}
