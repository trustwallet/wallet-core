// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <TrustWalletCore/TWPublicKey.h>

#include "../HexCoding.h"
#include "../PublicKey.h"

#include <TrezorCrypto/ecdsa.h>
#include <TrezorCrypto/secp256k1.h>

using TW::PublicKey;

struct TWPublicKey *_Nullable TWPublicKeyCreateWithData(TWData *_Nonnull data, enum TWPublicKeyType type) {
    auto& d = *reinterpret_cast<const TW::Data *>(data);
    if (!PublicKey::isValid(d, type)) {
        return nullptr;
    }

    return new TWPublicKey{ PublicKey(d, type) };
}

void TWPublicKeyDelete(struct TWPublicKey *_Nonnull pk) {
    delete pk;
}

bool TWPublicKeyIsValid(TWData *_Nonnull data, enum TWPublicKeyType type) {
    auto& d = *reinterpret_cast<const TW::Data *>(data);
    return PublicKey::isValid(d, type);
}

bool TWPublicKeyIsCompressed(struct TWPublicKey *_Nonnull pk) {
    return pk->impl.isCompressed();
}

TWData *TWPublicKeyData(struct TWPublicKey *_Nonnull pk) {
    return TWDataCreateWithBytes(pk->impl.bytes.data(), pk->impl.bytes.size());
}

struct TWPublicKey *_Nonnull TWPublicKeyCompressed(struct TWPublicKey *_Nonnull pk) {
    return new TWPublicKey{ pk->impl.compressed() };
}

struct TWPublicKey *_Nonnull TWPublicKeyUncompressed(struct TWPublicKey *_Nonnull pk) {
    return new TWPublicKey{ pk->impl.extended() };
}

bool TWPublicKeyVerify(struct TWPublicKey *_Nonnull pk, TWData *_Nonnull signature, TWData *message) {
    const auto& s = *reinterpret_cast<const TW::Data *>(signature);
    const auto& m = *reinterpret_cast<const TW::Data *>(message);
    return pk->impl.verify(s, m);
}

bool TWPublicKeyVerifyAsDER(struct TWPublicKey *_Nonnull pk, TWData *_Nonnull signature, TWData *message) {
    const auto& s = *reinterpret_cast<const TW::Data *>(signature);
    const auto& m = *reinterpret_cast<const TW::Data *>(message);
    return pk->impl.verifyAsDER(s, m);
}

bool TWPublicKeyVerifyZilliqaSchnorr(struct TWPublicKey *_Nonnull pk, TWData *_Nonnull signature, TWData *_Nonnull message) {
    const auto& s = *reinterpret_cast<const TW::Data *>(signature);
    const auto& m = *reinterpret_cast<const TW::Data *>(message);
    return pk->impl.verifyZilliqa(s, m);
}

enum TWPublicKeyType TWPublicKeyKeyType(struct TWPublicKey *_Nonnull publicKey) {
    return publicKey->impl.type;
}

TWString *_Nonnull TWPublicKeyDescription(struct TWPublicKey *_Nonnull publicKey) {
    const auto string = TW::hex(publicKey->impl.bytes);
    return TWStringCreateWithUTF8Bytes(string.c_str());
}

struct TWPublicKey *_Nullable TWPublicKeyRecover(TWData *_Nonnull signature, TWData *_Nonnull message) {
    try {
        const PublicKey publicKey = PublicKey::recover(*((TW::Data*)signature), *((TW::Data*)message));
        return new TWPublicKey{ publicKey };
    } catch (...) {
        return nullptr;
    }
}
