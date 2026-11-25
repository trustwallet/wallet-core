// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <TrustWalletCore/TWStoredKeySignResult.h>

#include "../Keystore/SealedSigner.h"

void TWStoredKeySignResultDelete(struct TWStoredKeySignResult* _Nonnull result) {
    delete result;
}

enum TWStoredKeySignError TWStoredKeySignResultError(struct TWStoredKeySignResult* _Nonnull result) {
    return result->impl.error;
}

TWData* _Nullable TWStoredKeySignResultData(struct TWStoredKeySignResult* _Nonnull result) {
    if (!result->impl.ok()) {
        return nullptr;
    }
    return TWDataCreateWithBytes(result->impl.data.data(), result->impl.data.size());
}

TWString* _Nonnull TWStoredKeySignResultErrorMessage(struct TWStoredKeySignResult* _Nonnull result) {
    return TWStringCreateWithUTF8Bytes(result->impl.message);
}
