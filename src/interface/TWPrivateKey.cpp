// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWPrivateKey.h>

#include "../PrivateKey.h"
#include "../PublicKey.h"

#include <TrezorCrypto/ecdsa.h>
#include <TrezorCrypto/rand.h>
#include <TrezorCrypto/secp256k1.h>

#include <string.h>

using namespace TW;

struct TWPrivateKey *TWPrivateKeyCreate() {
    std::array<uint8_t, PrivateKey::size> bytes;
    random_buffer(bytes.data(), PrivateKey::size);
    if (!PrivateKey::isValid(bytes)) {
        abort();
    }

    return new TWPrivateKey{ PrivateKey(std::move(bytes)) };
}

struct TWPrivateKey *_Nullable TWPrivateKeyCreateWithData(TWData *_Nonnull data) {
    if (!TWPrivateKeyIsValid(data)) {
        return nullptr;
    }

    std::array<uint8_t, PrivateKey::size> bytes;
    TWDataCopyBytes(data, 0, TWPrivateKeySize, bytes.data());

   return new TWPrivateKey{ PrivateKey(std::move(bytes)) };
}

struct TWPrivateKey *_Nullable TWPrivateKeyCreateCopy(struct TWPrivateKey *_Nonnull key) {
   return new TWPrivateKey{ PrivateKey(key->impl.bytes) };
}

void TWPrivateKeyDelete(struct TWPrivateKey *_Nonnull pk) {
    if (pk == nullptr)
        return;
    delete pk;
}

bool TWPrivateKeyIsValid(TWData *_Nonnull data) {
    // Check length
    if (TWDataSize(data) != TWPrivateKeySize) {
        return false;
    }

    // Check for zero address
    for (size_t i = 0; i < TWPrivateKeySize; i += 1) {
        if (TWDataGet(data, i) != 0) {
            return true;
        }
    }

    return false;
}

TWData *TWPrivateKeyData(struct TWPrivateKey *_Nonnull pk) {
    return TWDataCreateWithBytes(pk->impl.bytes.data(), TWPrivateKeySize);
}

struct TWPublicKey *_Nonnull TWPrivateKeyGetPublicKeySecp256k1(struct TWPrivateKey *_Nonnull pk, bool compressed) {
    if (compressed)  {
        return new TWPublicKey{ pk->impl.getPublicKey(PublicKeyType::secp256k1) };
     } else {
        return new TWPublicKey{ pk->impl.getPublicKey(PublicKeyType::secp256k1Extended) };
     }
}

struct TWPublicKey *_Nonnull TWPrivateKeyGetPublicKeyEd25519(struct TWPrivateKey *_Nonnull pk) {
    return new TWPublicKey{ pk->impl.getPublicKey(PublicKeyType::ed25519) };
}

TWData *TWPrivateKeySign(struct TWPrivateKey *_Nonnull pk, TWData *_Nonnull digest, enum TWCurve curve) {
    auto& d = *reinterpret_cast<const Data*>(digest);
    auto result = pk->impl.sign(d, curve);
    if (result.empty()) {
        return nullptr;
    } else {
        return TWDataCreateWithBytes(result.data(), result.size());
    }
}

TWData *TWPrivateKeySignAsDER(struct TWPrivateKey *_Nonnull pk, TWData *_Nonnull digest, enum TWCurve curve) {
    auto& d = *reinterpret_cast<const Data*>(digest);
    auto result = pk->impl.signAsDER(d, curve);
    if (result.empty()) {
        return nullptr;
    } else {
        return TWDataCreateWithBytes(result.data(), result.size());
    }
}
