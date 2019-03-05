// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWPrivateKey.h>

#include "../PrivateKey.h"

#include <TrezorCrypto/ecdsa.h>
#include <TrezorCrypto/rand.h>
#include <TrezorCrypto/secp256k1.h>

#include <string.h>

struct TWPrivateKey *TWPrivateKeyCreate() {
    std::array<uint8_t, TW::PrivateKey::size> bytes;
    random_buffer(bytes.data(), TW::PrivateKey::size);
    if (!TW::PrivateKey::isValid(bytes)) {
        abort();
    }

    return new TWPrivateKey{ TW::PrivateKey(std::move(bytes)) };
}

struct TWPrivateKey *_Nullable TWPrivateKeyCreateWithData(TWData *_Nonnull data) {
    if (!TWPrivateKeyIsValid(data)) {
        return nullptr;
    }

    std::array<uint8_t, TW::PrivateKey::size> bytes;
    TWDataCopyBytes(data, 0, TWPrivateKeySize, bytes.data());

   return new TWPrivateKey{ TW::PrivateKey(std::move(bytes)) };
}

struct TWPrivateKey *_Nullable TWPrivateKeyCreateCopy(struct TWPrivateKey *_Nonnull key) {
   return new TWPrivateKey{ TW::PrivateKey(key->impl.bytes) };
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

struct TWPublicKey TWPrivateKeyGetPublicKey(struct TWPrivateKey *_Nonnull pk, bool compressed) {
    struct TWPublicKey result;
    if (compressed)  {
        ecdsa_get_public_key33(&secp256k1, pk->impl.bytes.data(), result.bytes);
     } else {
        ecdsa_get_public_key65(&secp256k1, pk->impl.bytes.data(), result.bytes);
     }

    return result;
}

TWData *TWPrivateKeySign(struct TWPrivateKey *_Nonnull pk, TWData *_Nonnull digest) {
    uint8_t result[65];
    uint8_t *bytes = TWDataBytes(digest);
    bool success = ecdsa_sign_digest(&secp256k1, pk->impl.bytes.data(), bytes, result, result + 64, NULL) == 0;
    if (success) {
        return TWDataCreateWithBytes(result, 65);
    } else {
        return NULL;
    }
}

TWData *TWPrivateKeySignAsDER(struct TWPrivateKey *_Nonnull pk, TWData *_Nonnull digest) {
    uint8_t sig[64];
    uint8_t *bytes = TWDataBytes(digest);
    bool success = ecdsa_sign_digest(&secp256k1, pk->impl.bytes.data(), bytes, sig, NULL, NULL) == 0;

    if (!success) {
        return NULL;
    }

    uint8_t result[72];
    size_t size = ecdsa_sig_to_der(sig, result);

    return TWDataCreateWithBytes(result, size);
}
