// Copyright © 2017-2023 Trust Wallet.
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
#include <TrustWalletCore/TWCoinType.h>

#include <exception>

using namespace TW;

struct TWPrivateKey *TWPrivateKeyCreate() {
    Data bytes(PrivateKey::_size);
    random_buffer(bytes.data(), PrivateKey::_size);
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
    auto dataSize = TWDataSize(data);
    Data bytes(dataSize);
    TWDataCopyBytes(data, 0, dataSize, bytes.data());
    if (!PrivateKey::isValid(bytes)) {
        return nullptr;
    }
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

bool TWPrivateKeyIsValid(TWData *_Nonnull data, enum TWCurve curve) {
    auto dataSize = TWDataSize(data);
    std::vector<uint8_t> bytes(dataSize);
    TWDataCopyBytes(data, 0, dataSize, bytes.data());
    return PrivateKey::isValid(bytes, curve);
}

TWData *TWPrivateKeyData(struct TWPrivateKey *_Nonnull pk) {
    return TWDataCreateWithBytes(pk->impl.bytes.data(), pk->impl.bytes.size());
}

struct TWPublicKey *_Nonnull TWPrivateKeyGetPublicKeyNist256p1(struct TWPrivateKey *_Nonnull pk) {
    return TWPrivateKeyGetPublicKeyByType(pk, TWPublicKeyTypeNIST256p1);
}

struct TWPublicKey *_Nonnull TWPrivateKeyGetPublicKeySecp256k1(struct TWPrivateKey *_Nonnull pk, bool compressed) {
    if (compressed)  {
        return TWPrivateKeyGetPublicKeyByType(pk, TWPublicKeyTypeSECP256k1);
     } else {
         return TWPrivateKeyGetPublicKeyByType(pk, TWPublicKeyTypeSECP256k1Extended);
     }
}

struct TWPublicKey *_Nonnull TWPrivateKeyGetPublicKeyEd25519(struct TWPrivateKey *_Nonnull pk) {
    return TWPrivateKeyGetPublicKeyByType(pk, TWPublicKeyTypeED25519);
}

struct TWPublicKey *_Nonnull TWPrivateKeyGetPublicKeyEd25519Blake2b(struct TWPrivateKey *_Nonnull pk) {
    return TWPrivateKeyGetPublicKeyByType(pk, TWPublicKeyTypeED25519Blake2b);
}

struct TWPublicKey *_Nonnull TWPrivateKeyGetPublicKeyEd25519Cardano(struct TWPrivateKey *_Nonnull pk) {
    return TWPrivateKeyGetPublicKeyByType(pk, TWPublicKeyTypeED25519Cardano);
}

struct TWPublicKey *_Nonnull TWPrivateKeyGetPublicKeyCurve25519(struct TWPrivateKey *_Nonnull pk) {
    return TWPrivateKeyGetPublicKeyByType(pk, TWPublicKeyTypeCURVE25519);
}

TWData *_Nullable TWPrivateKeyGetSharedKey(const struct TWPrivateKey *_Nonnull pk, const struct TWPublicKey *_Nonnull publicKey, enum TWCurve curve) {
    auto result = pk->impl.getSharedKey(publicKey->impl, curve);
    if (result.empty()) {
        return nullptr;
    } else {
        return TWDataCreateWithBytes(result.data(), result.size());
    }
}

TWData *TWPrivateKeySign(struct TWPrivateKey *_Nonnull pk, TWData *_Nonnull digest, enum TWCurve curve) {
    const auto& d = *reinterpret_cast<const Data*>(digest);
    auto result = pk->impl.sign(d, curve);
    if (result.empty()) {
        return nullptr;
    } else {
        return TWDataCreateWithBytes(result.data(), result.size());
    }
}

TWData* TWPrivateKeySignAsDER(struct TWPrivateKey* pk, TWData* digest) {
    auto& d = *reinterpret_cast<const Data*>(digest);
    auto result = pk->impl.signAsDER(d);
    if (result.empty()) {
        return nullptr;
    } else {
        return TWDataCreateWithBytes(result.data(), result.size());
    }
}

TWData *TWPrivateKeySignZilliqaSchnorr(struct TWPrivateKey *_Nonnull pk, TWData *_Nonnull message) {
    const auto& msg = *reinterpret_cast<const Data*>(message);
    auto result = pk->impl.signZilliqa(msg);

    if (result.empty()) {
        return nullptr;
    } else {
        return TWDataCreateWithBytes(result.data(), result.size());
    }
}

struct TWPublicKey* TWPrivateKeyGetPublicKey(struct TWPrivateKey* pk, enum TWCoinType coinType) {
    return TWPrivateKeyGetPublicKeyByType(pk, TWCoinTypePublicKeyType(coinType));
}

struct TWPublicKey* TWPrivateKeyGetPublicKeyByType(struct TWPrivateKey* pk, enum TWPublicKeyType pubkeyType) {
    return new TWPublicKey{ pk->impl.getPublicKey(pubkeyType) };
}
