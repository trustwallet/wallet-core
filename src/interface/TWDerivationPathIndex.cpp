// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
