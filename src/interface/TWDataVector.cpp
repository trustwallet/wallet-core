// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <TrustWalletCore/TWDataVector.h>

#include "Data.h"

#include <vector>
#include <cassert>

using namespace TW;


struct TWDataVector {
    std::vector<Data> impl;
};


struct TWDataVector *_Nonnull TWDataVectorCreate() {
    auto* obj = new struct TWDataVector();
    assert(obj != nullptr);
    return obj;
}

struct TWDataVector *_Nonnull TWDataVectorCreateWithData(TWData *_Nonnull data) {
    auto* obj = new struct TWDataVector();
    assert(obj != nullptr);

    TWDataVectorAdd(obj, data);
    return obj;
}

void TWDataVectorDelete(struct TWDataVector *_Nonnull dataVector) {
    delete dataVector;
}

void TWDataVectorAdd(struct TWDataVector *_Nonnull dataVector, TWData *_Nonnull data) {
    dataVector->impl.push_back(TW::data(TWDataBytes(data), TWDataSize(data)));
}

size_t TWDataVectorSize(const struct TWDataVector *_Nonnull dataVector) {
    return dataVector->impl.size();
}

TWData *_Nullable TWDataVectorGet(const struct TWDataVector *_Nonnull dataVector, size_t index) {
    if (index >= dataVector->impl.size()) {
        return nullptr;
    }
    auto& elem = dataVector->impl[index];
    return TWDataCreateWithBytes(elem.data(), elem.size());
}
