// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TrustWalletCore/TWCryptoBoxPublicKey.h"
#include "CryptoBox.h"

using namespace TW;

bool TWCryptoBoxPublicKeyIsValid(TWData* _Nonnull data) {
    auto& bytes = *reinterpret_cast<const Data*>(data);
    return CryptoBox::PublicKey::isValid(bytes);
}

struct TWCryptoBoxPublicKey* _Nullable TWCryptoBoxPublicKeyCreateWithData(TWData* _Nonnull data) {
    auto& bytes = *reinterpret_cast<const Data*>(data);
    auto publicKey = CryptoBox::PublicKey::fromBytes(bytes);
    if (!publicKey) {
        return nullptr;
    }
    return new TWCryptoBoxPublicKey { publicKey.value() };
}

void TWCryptoBoxPublicKeyDelete(struct TWCryptoBoxPublicKey* _Nonnull publicKey) {
    delete publicKey;
}

TWData* _Nonnull TWCryptoBoxPublicKeyData(struct TWCryptoBoxPublicKey* _Nonnull publicKey) {
    auto bytes = publicKey->impl.getData();
    return TWDataCreateWithBytes(bytes.data(), bytes.size());
}
