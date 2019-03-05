// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWPrivateKeyEd25519.h>

#include "../PrivateKeyEd25519.h"

#include <TrezorCrypto/rand.h>
#include <TrezorCrypto/ed25519.h>

#include <string.h>

using namespace TW::ed25519;

struct TWPrivateKeyEd25519 *TWPrivateKeyEd25519Create() {
    std::array<uint8_t, PrivateKey::size> bytes;
    random_buffer(bytes.data(), PrivateKey::size);
    if (!PrivateKey::isValid(bytes)) {
        abort();
    }

    return new TWPrivateKeyEd25519{ PrivateKey(std::move(bytes)) };
}

struct TWPrivateKeyEd25519 *_Nullable TWPrivateKeyEd25519CreateWithData(TWData *_Nonnull data) {
    if (!TWPrivateKeyEd25519IsValid(data)) {
        return nullptr;
    }

    std::array<uint8_t, PrivateKey::size> bytes;
    TWDataCopyBytes(data, 0, TWPrivateKeyEd25519Size, bytes.data());

    return new TWPrivateKeyEd25519{ PrivateKey(std::move(bytes)) };
}

struct TWPrivateKeyEd25519 *_Nullable TWPrivateKeyEd25519CreateCopy(struct TWPrivateKeyEd25519 *_Nonnull key) {
    return new TWPrivateKeyEd25519{ PrivateKey(key->impl.bytes) };
}

void TWPrivateKeyEd25519Delete(struct TWPrivateKeyEd25519 *_Nonnull pk) {
    if (pk == nullptr)
        return;
    delete pk;
}

bool TWPrivateKeyEd25519IsValid(TWData *_Nonnull data) {
    // Check length
    if (TWDataSize(data) != TWPrivateKeyEd25519Size) {
        return false;
    }

    // Check for zero address
    for (size_t i = 0; i < TWPrivateKeyEd25519Size; i += 1) {
        if (TWDataGet(data, i) != 0) {
            return true;
        }
    }

    return false;
}

TWData *TWPrivateKeyEd25519Data(struct TWPrivateKeyEd25519 *_Nonnull pk) {
    return TWDataCreateWithBytes(pk->impl.bytes.data(), TWPrivateKeyEd25519Size);
}

struct TWPublicKeyEd25519 TWPrivateKeyEd25519GetPublicKey(struct TWPrivateKeyEd25519 *_Nonnull pk) {
    auto pub_array = pk->impl.getPublicKey();
    TWPublicKeyEd25519 pub;
    std::copy(pub_array.begin(), pub_array.end(), pub.bytes);
    return pub;
}

TWData *TWPrivateKeyEd25519Sign(struct TWPrivateKeyEd25519 *_Nonnull pk, TWData *_Nonnull digest) {
    auto ptr = reinterpret_cast<const uint8_t *>(digest);
    std::vector<uint8_t> digest_vec(&ptr[0], &ptr[32]);
    auto sig = pk->impl.sign(digest_vec);
    return TWDataCreateWithBytes(sig.data(), 64);
}
