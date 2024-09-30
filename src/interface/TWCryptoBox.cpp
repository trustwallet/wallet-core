// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TrustWalletCore/TWCryptoBox.h"
#include "CryptoBox.h"

using namespace TW;

TWData* _Nonnull TWCryptoBoxEncryptEasy(struct TWCryptoBoxSecretKey* _Nonnull mySecret, struct TWCryptoBoxPublicKey* _Nonnull otherPubkey, TWData* _Nonnull message) {
    auto& messageBytes = *reinterpret_cast<const Data*>(message);
    auto encrypted = CryptoBox::encryptEasy(mySecret->impl, otherPubkey->impl, messageBytes);
    return TWDataCreateWithBytes(encrypted.data(), encrypted.size());
}

TWData* _Nullable TWCryptoBoxDecryptEasy(struct TWCryptoBoxSecretKey* _Nonnull mySecret, struct TWCryptoBoxPublicKey* _Nonnull otherPubkey, TWData* _Nonnull encrypted) {
    auto& encryptedBytes = *reinterpret_cast<const Data*>(encrypted);
    auto decrypted = CryptoBox::decryptEasy(mySecret->impl, otherPubkey->impl, encryptedBytes);
    if (!decrypted) {
        return nullptr;
    }
    return TWDataCreateWithBytes(decrypted->data(), decrypted->size());
}
