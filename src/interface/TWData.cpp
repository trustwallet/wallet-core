// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWData.h>
#include <TrustWalletCore/TWString.h>
#include "Data.h"
#include "HexCoding.h"
#include <algorithm>
#include <vector>

using namespace TW;

TWData *_Nonnull TWDataCreateWithBytes(const uint8_t *_Nonnull bytes, size_t size) {
    auto* data = new Data();
    data->reserve(size);
    std::copy(bytes, bytes + size, std::back_inserter(*data));
    return data;
}

TWData *_Nonnull TWDataCreateWithSize(size_t size) {
    auto* data = new Data(size, 0);
    return data;
}

TWData *_Nonnull TWDataCreateWithData(TWData *_Nonnull data) {
    auto* other = reinterpret_cast<const Data*>(data);
    auto* copy = new Data(*other);
    return copy;
}

TWData* TWDataCreateWithHexString(const TWString* hex) {
    if (hex == nullptr) {
        return nullptr;
    }
    Data data = parse_hex(std::string(TWStringUTF8Bytes(hex)));
    return TWDataCreateWithBytes(data.data(), data.size());
}

size_t TWDataSize(TWData *_Nonnull data) {
    auto* v = reinterpret_cast<const Data*>(data);
    return v->size();
}

uint8_t *_Nonnull TWDataBytes(TWData *_Nonnull data) {
    auto* v = const_cast<Data*>(reinterpret_cast<const Data*>(data));
    return v->data();
}

uint8_t TWDataGet(TWData *_Nonnull data, size_t index) {
    auto* v = reinterpret_cast<const Data*>(data);
    return (*v)[index];
}

void TWDataSet(TWData *_Nonnull data, size_t index, uint8_t byte) {
    auto* v = const_cast<Data*>(reinterpret_cast<const Data*>(data));
    (*v)[index] = byte;
}

void TWDataCopyBytes(TWData *_Nonnull data, size_t start, size_t size, uint8_t *_Nonnull output) {
    auto* v = reinterpret_cast<const Data*>(data);
    std::copy(std::begin(*v) + start, std::begin(*v) + start + size, output);
}

void TWDataReplaceBytes(TWData *_Nonnull data, size_t start, size_t size, const uint8_t *_Nonnull bytes) {
    auto* v = const_cast<Data*>(reinterpret_cast<const Data*>(data));
    std::copy(bytes, bytes + size, std::begin(*v) + start);
}

void TWDataAppendBytes(TWData *_Nonnull data, const uint8_t *_Nonnull bytes, size_t size) {
    auto* v = const_cast<Data*>(reinterpret_cast<const Data*>(data));
    for (auto i = 0ul; i < size; i += 1)
        v->push_back(bytes[i]);
}

void TWDataAppendByte(TWData *_Nonnull data, uint8_t byte) {
    auto* v = const_cast<Data*>(reinterpret_cast<const Data*>(data));
    v->push_back(byte);
}

void TWDataAppendData(TWData *_Nonnull data, TWData *_Nonnull append) {
    auto* v = const_cast<Data*>(reinterpret_cast<const Data*>(data));
    auto* av = reinterpret_cast<const Data*>(append);
    std::copy(av->begin(), av->end(), std::back_inserter(*v));
}

void TWDataReverse(TWData *_Nonnull data) {
    auto* v = const_cast<Data*>(reinterpret_cast<const Data*>(data));
    std::reverse(std::begin(*v), std::end(*v));
}

void TWDataReset(TWData *_Nonnull data) {
    auto* v = const_cast<Data*>(reinterpret_cast<const Data*>(data));
    std::fill(std::begin(*v), std::end(*v), 0);
}

void TWDataDelete(TWData *_Nonnull data) {
    auto* v = reinterpret_cast<const Data*>(data);
    delete v;
}

bool TWDataEqual(TWData *_Nonnull lhs, TWData *_Nonnull rhs) {
    auto* lv = reinterpret_cast<const Data*>(lhs);
    auto* rv = reinterpret_cast<const Data*>(rhs);
    return *lv == *rv;
}
