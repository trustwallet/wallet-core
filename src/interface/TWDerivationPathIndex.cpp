// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <TrustWalletCore/TWDerivationPathIndex.h>
#include <TrustWalletCore/TWString.h>

#include "../DerivationPath.h"

using namespace TW;

struct TWDerivationPathIndex* _Nonnull TWDerivationPathIndexCreate(uint32_t value, bool hardened) {
    return new TWDerivationPathIndex{DerivationPathIndex(value, hardened)};
}

void TWDerivationPathIndexDelete(struct TWDerivationPathIndex* _Nonnull index) {
    delete index;
}

uint32_t TWDerivationPathIndexValue(struct TWDerivationPathIndex* _Nonnull index) {
    return index->impl.value;
}

bool TWDerivationPathIndexHardened(struct TWDerivationPathIndex* _Nonnull index) {
    return index->impl.hardened;
}

TWString* _Nonnull TWDerivationPathIndexDescription(struct TWDerivationPathIndex* _Nonnull index) {
    return TWStringCreateWithUTF8Bytes(index->impl.string().c_str());
}
