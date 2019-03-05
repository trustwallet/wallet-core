// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWPublicKeyEd25519.h>

#include "../HexCoding.h"
#include "../PublicKeyEd25519.h"

#include <TrezorCrypto/ed25519.h>

#include <string.h>

bool TWPublicKeyEd25519InitWithData(struct TWPublicKeyEd25519 *_Nonnull pk, TWData *_Nonnull data)
{
     if (!TWPublicKeyEd25519IsValid(data)) {
        return false;
    }

    TWDataCopyBytes(data, 0, TWDataSize(data), pk->bytes);
    return true;
}

bool TWPublicKeyEd25519IsValid(TWData *_Nonnull data)
{
    return TWDataSize(data) == TWPublicKeyEd25519Size;
}

TWData *_Nonnull TWPublicKeyEd25519Data(struct TWPublicKeyEd25519 pk)
{
    return TWDataCreateWithBytes(pk.bytes, TWPublicKeyEd25519Size);
}

bool TWPublicKeyEd25519Verify(struct TWPublicKeyEd25519 pk, TWData *_Nonnull signature, TWData *_Nonnull message)
{
    uint8_t *signatureBytes = TWDataBytes(signature);
    uint8_t *messageBytes = TWDataBytes(message);
    bool success = ed25519_sign_open(messageBytes, TWDataSize(message), pk.bytes, signatureBytes) == 0;
    return success;
}

TWString *_Nonnull TWPublicKeyEd25519Description(struct TWPublicKeyEd25519 pk)
{
    const auto string = TW::hex(pk.bytes, pk.bytes +  TWPublicKeyEd25519Size);
    return TWStringCreateWithUTF8Bytes(string.c_str());
}
