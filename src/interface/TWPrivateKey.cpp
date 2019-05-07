// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../PrivateKey.h"
#include "../PublicKey.h"

#include <TrezorCrypto/ecdsa.h>
#include <TrezorCrypto/rand.h>
#include <TrezorCrypto/secp256k1.h>
#include <TrustWalletCore/TWPrivateKey.h>

#include <exception>

using namespace TW;

struct TWPrivateKey *TWPrivateKeyCreate() {
    std::array<uint8_t, PrivateKey::size> bytes = {0};
    random_buffer(bytes.data(), PrivateKey::size);
    if (!PrivateKey::isValid(bytes)) {
        // Under no circumstance return an invalid private key. We'd rather
        // crash. This also captures cases where the random generator fails
        // since we initialize the array to zeros, which is an invalid private
        // key.
        std::terminate();
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

struct TWPublicKey *_Nonnull TWPrivateKeyGetPublicKeyNist256p1(struct TWPrivateKey *_Nonnull pk) {
    return new TWPublicKey{ pk->impl.getPublicKey(TWPublicKeyTypeNIST256p1) };
}

struct TWPublicKey *_Nonnull TWPrivateKeyGetPublicKeySecp256k1(struct TWPrivateKey *_Nonnull pk, bool compressed) {
    if (compressed)  {
        return new TWPublicKey{ pk->impl.getPublicKey(TWPublicKeyTypeSECP256k1) };
     } else {
        return new TWPublicKey{ pk->impl.getPublicKey(TWPublicKeyTypeSECP256k1Extended) };
     }
}

struct TWPublicKey *_Nonnull TWPrivateKeyGetPublicKeyEd25519(struct TWPrivateKey *_Nonnull pk) {
    return new TWPublicKey{ pk->impl.getPublicKey(TWPublicKeyTypeED25519) };
}

struct TWPublicKey *_Nonnull TWPrivateKeyGetPublicKeyEd25519Blake2b(struct TWPrivateKey *_Nonnull pk) {
    return new TWPublicKey{ pk->impl.getPublicKey(TWPublicKeyTypeED25519Blake2b) };
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

TWData *TWPrivateKeySignSchnorr(struct TWPrivateKey *_Nonnull pk, TWData *_Nonnull message, enum TWCurve curve) {
    auto& msg = *reinterpret_cast<const Data*>(message);
    auto result = pk->impl.signSchnorr(msg, curve);

    if (result.empty()) {
        return nullptr;
    } else {
        return TWDataCreateWithBytes(result.data(), result.size());
    }
}
