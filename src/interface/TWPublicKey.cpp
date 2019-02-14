// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWPublicKey.h>

#include "../HexCoding.h"
#include <TrezorCrypto/ecdsa.h>
#include <TrezorCrypto/secp256k1.h>

#include <string.h>

bool TWPublicKeyInitWithData(struct TWPublicKey *_Nonnull pk, TWData *_Nonnull data) {
    if (!TWPublicKeyIsValid(data)) {
        return false;
    }

    TWDataCopyBytes(data, 0, TWDataSize(data), pk->bytes);
    return true;
}

bool TWPublicKeyIsValid(TWData *_Nonnull data) {
    switch (TWDataGet(data, 0)) {
    case 2:
    case 3:
        return TWDataSize(data) == TWPublicKeyCompressedSize;
    case 4:
    case 6:
    case 7:
        return TWDataSize(data) == TWPublicKeyUncompressedSize;
    default:
        return false;
    }
}

bool TWPublicKeyIsCompressed(struct TWPublicKey pk) {
    return pk.bytes[0] == 2 || pk.bytes[0] == 3;
}

TWData *TWPublicKeyData(struct TWPublicKey pk) {
    if (TWPublicKeyIsCompressed(pk)) {
        return TWDataCreateWithBytes(pk.bytes, TWPublicKeyCompressedSize);
    } else {
        return TWDataCreateWithBytes(pk.bytes, TWPublicKeyUncompressedSize);
    }
}

struct TWPublicKey TWPublicKeyCompressed(struct TWPublicKey pk) {
    if (TWPublicKeyIsCompressed(pk)) {
        return pk;
    }

    struct TWPublicKey result;
    result.bytes[0] = 0x02 | (pk.bytes[64] & 0x01);
    memcpy(result.bytes + 1, pk.bytes + 1, TWPublicKeyCompressedSize - 1);
    return result;
}

bool TWPublicKeyVerify(struct TWPublicKey pk, TWData *signature, TWData *message) {
    uint8_t *signatureBytes = TWDataBytes(signature);
    uint8_t *messageBytes = TWDataBytes(message);
    bool success = ecdsa_verify_digest(&secp256k1, pk.bytes, signatureBytes, messageBytes) == 0;
    return success;
}

TWString *_Nonnull TWPublicKeyDescription(struct TWPublicKey publicKey) {
    const auto size = TWPublicKeyIsCompressed(publicKey) ? TWPublicKeyCompressedSize : TWPublicKeyUncompressedSize;
    const auto string = TW::hex(publicKey.bytes, publicKey.bytes +  size);
    return TWStringCreateWithUTF8Bytes(string.c_str());
}

struct TWPublicKey TWPublicKeyRecover(TWData *_Nonnull signature, TWData *_Nonnull message) {
    TWPublicKey pubkey;
    auto signatureBytes = TWDataBytes(signature);
    auto v = signatureBytes[64];
    if (v >= 27) {
        v -= 27;
    }
    if (ecdsa_recover_pub_from_sig(&secp256k1, pubkey.bytes, signatureBytes, TWDataBytes(message), v) != 0) {
        return {0};
    }
    return pubkey;
}
