// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWPrivateKeySecp256k1.h>

#include "../PrivateKeySecp256k1.h"

#include <TrezorCrypto/ecdsa.h>
#include <TrezorCrypto/rand.h>
#include <TrezorCrypto/secp256k1.h>

#include <string.h>

using namespace TW::secp256k1;

struct TWPrivateKeySecp256k1 *TWPrivateKeySecp256k1Create() {
    std::array<uint8_t, PrivateKey::size> bytes;
    random_buffer(bytes.data(), PrivateKey::size);
    if (!PrivateKey::isValid(bytes)) {
        abort();
    }

    return new TWPrivateKeySecp256k1{ PrivateKey(std::move(bytes)) };
}

struct TWPrivateKeySecp256k1 *_Nullable TWPrivateKeySecp256k1CreateWithData(TWData *_Nonnull data) {
    if (!TWPrivateKeySecp256k1IsValid(data)) {
        return nullptr;
    }

    std::array<uint8_t, PrivateKey::size> bytes;
    TWDataCopyBytes(data, 0, TWPrivateKeySecp256k1Size, bytes.data());

   return new TWPrivateKeySecp256k1{ PrivateKey(std::move(bytes)) };
}

struct TWPrivateKeySecp256k1 *_Nullable TWPrivateKeySecp256k1CreateCopy(struct TWPrivateKeySecp256k1 *_Nonnull key) {
   return new TWPrivateKeySecp256k1{ PrivateKey(key->impl.bytes) };
}

void TWPrivateKeySecp256k1Delete(struct TWPrivateKeySecp256k1 *_Nonnull pk) {
    if (pk == nullptr)
        return;
    delete pk;
}

bool TWPrivateKeySecp256k1IsValid(TWData *_Nonnull data) {
    // Check length
    if (TWDataSize(data) != TWPrivateKeySecp256k1Size) {
        return false;
    }

    // Check for zero address
    for (size_t i = 0; i < TWPrivateKeySecp256k1Size; i += 1) {
        if (TWDataGet(data, i) != 0) {
            return true;
        }
    }

    return false;
}

TWData *TWPrivateKeySecp256k1Data(struct TWPrivateKeySecp256k1 *_Nonnull pk) {
    return TWDataCreateWithBytes(pk->impl.bytes.data(), TWPrivateKeySecp256k1Size);
}

struct TWPublicKeySecp256k1 TWPrivateKeySecp256k1GetPublicKey(struct TWPrivateKeySecp256k1 *_Nonnull pk, bool compressed) {
    struct TWPublicKeySecp256k1 result;
    if (compressed)  {
        ecdsa_get_public_key33(&curve_secp256k1, pk->impl.bytes.data(), result.bytes);
     } else {
        ecdsa_get_public_key65(&curve_secp256k1, pk->impl.bytes.data(), result.bytes);
     }

    return result;
}

TWData *TWPrivateKeySecp256k1Sign(struct TWPrivateKeySecp256k1 *_Nonnull pk, TWData *_Nonnull digest) {
    uint8_t result[65];
    uint8_t *bytes = TWDataBytes(digest);
    bool success = ecdsa_sign_digest(&curve_secp256k1, pk->impl.bytes.data(), bytes, result, result + 64, NULL) == 0;
    if (success) {
        return TWDataCreateWithBytes(result, 65);
    } else {
        return NULL;
    }
}

TWData *TWPrivateKeySecp256k1SignAsDER(struct TWPrivateKeySecp256k1 *_Nonnull pk, TWData *_Nonnull digest) {
    uint8_t sig[64];
    uint8_t *bytes = TWDataBytes(digest);
    bool success = ecdsa_sign_digest(&curve_secp256k1, pk->impl.bytes.data(), bytes, sig, NULL, NULL) == 0;

    if (!success) {
        return NULL;
    }

    uint8_t result[72];
    size_t size = ecdsa_sig_to_der(sig, result);

    return TWDataCreateWithBytes(result, size);
}
