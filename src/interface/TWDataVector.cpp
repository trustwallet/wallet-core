// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
    const auto dataData = static_cast<const Data*>(data);
    assert(dataData != nullptr);
    dataVector->impl.push_back(*dataData);
}

size_t TWDataVectorSize(struct TWDataVector *_Nonnull dataVector) {
    return dataVector->impl.size();
}

TWData *_Nullable TWDataVectorGet(struct TWDataVector *_Nonnull dataVector, size_t index) {
    if (index >= dataVector->impl.size()) {
        return nullptr;
    }
    auto& elem = dataVector->impl[index];
    return TWDataCreateWithBytes(elem.data(), elem.size());
}
