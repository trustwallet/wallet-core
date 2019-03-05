// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWPublicKeySecp256k1.h>

#include "../HexCoding.h"
#include <TrezorCrypto/ecdsa.h>
#include <TrezorCrypto/secp256k1.h>

#include <string.h>

bool TWPublicKeySecp256k1InitWithData(struct TWPublicKeySecp256k1 *_Nonnull pk, TWData *_Nonnull data) {
    if (!TWPublicKeySecp256k1IsValid(data)) {
        return false;
    }

    TWDataCopyBytes(data, 0, TWDataSize(data), pk->bytes);
    return true;
}

bool TWPublicKeySecp256k1IsValid(TWData *_Nonnull data) {
    switch (TWDataGet(data, 0)) {
    case 2:
    case 3:
        return TWDataSize(data) == TWPublicKeySecp256k1CompressedSize;
    case 4:
    case 6:
    case 7:
        return TWDataSize(data) == TWPublicKeySecp256k1UncompressedSize;
    default:
        return false;
    }
}

bool TWPublicKeySecp256k1IsCompressed(struct TWPublicKeySecp256k1 pk) {
    return pk.bytes[0] == 2 || pk.bytes[0] == 3;
}

TWData *TWPublicKeySecp256k1Data(struct TWPublicKeySecp256k1 pk) {
    if (TWPublicKeySecp256k1IsCompressed(pk)) {
        return TWDataCreateWithBytes(pk.bytes, TWPublicKeySecp256k1CompressedSize);
    } else {
        return TWDataCreateWithBytes(pk.bytes, TWPublicKeySecp256k1UncompressedSize);
    }
}

struct TWPublicKeySecp256k1 TWPublicKeySecp256k1Compressed(struct TWPublicKeySecp256k1 pk) {
    if (TWPublicKeySecp256k1IsCompressed(pk)) {
        return pk;
    }

    struct TWPublicKeySecp256k1 result;
    result.bytes[0] = 0x02 | (pk.bytes[64] & 0x01);
    memcpy(result.bytes + 1, pk.bytes + 1, TWPublicKeySecp256k1CompressedSize - 1);
    return result;
}

bool TWPublicKeySecp256k1Verify(struct TWPublicKeySecp256k1 pk, TWData *signature, TWData *message) {
    uint8_t *signatureBytes = TWDataBytes(signature);
    uint8_t *messageBytes = TWDataBytes(message);
    bool success = ecdsa_verify_digest(&curve_secp256k1, pk.bytes, signatureBytes, messageBytes) == 0;
    return success;
}

TWString *_Nonnull TWPublicKeySecp256k1Description(struct TWPublicKeySecp256k1 publicKey) {
    const auto size = TWPublicKeySecp256k1IsCompressed(publicKey) ? TWPublicKeySecp256k1CompressedSize : TWPublicKeySecp256k1UncompressedSize;
    const auto string = TW::hex(publicKey.bytes, publicKey.bytes +  size);
    return TWStringCreateWithUTF8Bytes(string.c_str());
}

struct TWPublicKeySecp256k1 TWPublicKeySecp256k1Recover(TWData *_Nonnull signature, TWData *_Nonnull message) {
    TWPublicKeySecp256k1 pubkey;
    auto signatureBytes = TWDataBytes(signature);
    auto v = signatureBytes[64];
    if (v >= 27) {
        v -= 27;
    }
    if (ecdsa_recover_pub_from_sig(&curve_secp256k1, pubkey.bytes, signatureBytes, TWDataBytes(message), v) != 0) {
        return {0};
    }
    return pubkey;
}
