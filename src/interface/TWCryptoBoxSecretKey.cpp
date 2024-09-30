// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TrustWalletCore/TWCryptoBoxSecretKey.h"
#include "CryptoBox.h"

using namespace TW;

bool TWCryptoBoxSecretKeyIsValid(TWData* _Nonnull data) {
    auto& bytes = *reinterpret_cast<const Data*>(data);
    return CryptoBox::SecretKey::isValid(bytes);
}

struct TWCryptoBoxSecretKey* _Nullable TWCryptoBoxSecretKeyCreateWithData(TWData* _Nonnull data) {
    auto& bytes = *reinterpret_cast<const Data*>(data);
    auto secretKey = CryptoBox::SecretKey::fromBytes(bytes);
    if (!secretKey) {
        return nullptr;
    }
    return new TWCryptoBoxSecretKey { secretKey.value() };
}

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

TWData* _Nonnull TWCryptoBoxSecretKeyData(struct TWCryptoBoxSecretKey* _Nonnull secretKey) {
    auto bytes = secretKey->impl.getData();
    return TWDataCreateWithBytes(bytes.data(), bytes.size());
}
