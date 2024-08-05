// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TrustWalletCore/TWCryptoBoxSecretKey.h"
#include "CryptoBox.h"

using namespace TW;

struct TWCryptoBoxSecretKey* _Nonnull TWCryptoBoxSecretKeyCreate() {
    CryptoBox::SecretKey secretKey;
    return new TWCryptoBoxSecretKey { secretKey };
}

void TWCryptoBoxSecretKeyDelete(struct TWCryptoBoxSecretKey* _Nonnull key) {
    delete key;
}

struct TWCryptoBoxPublicKey* TWCryptoBoxSecretKeyGetPublicKey(struct TWCryptoBoxSecretKey* _Nonnull key) {
    auto publicKey = key->impl.getPublicKey();
    return new TWCryptoBoxPublicKey { publicKey };
}
