// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWKeyDerivation.h>

#include <TrezorCrypto/pbkdf2.h>
#include <TrezorCrypto/scrypt.h>

TWData *_Nullable TWKeyDerivationScrypt(TWString *_Nonnull password, TWData *_Nonnull salt, uint64_t n, uint32_t r, uint32_t p, size_t keyLength) {
    uint8_t result[keyLength];
    auto success = scrypt((const uint8_t *) TWStringUTF8Bytes(password), TWStringSize(password), TWDataBytes(salt), TWDataSize(salt), n, r, p, result, keyLength);
    if (success != 0) {
        return nullptr;
    }
    return TWDataCreateWithBytes(result, keyLength);
}

TWData *_Nonnull TWKeyDerivationPBKDF2_256(TWString *_Nonnull password, TWData *_Nonnull salt,  uint32_t iterations, size_t keyLength) {
    uint8_t result[keyLength];
    pbkdf2_hmac_sha256((const uint8_t *) TWStringUTF8Bytes(password), static_cast<int>(TWStringSize(password)), TWDataBytes(salt), static_cast<int>(TWDataSize(salt)), iterations, result, static_cast<int>(keyLength));
    return TWDataCreateWithBytes(result, keyLength);
}

TWData *_Nonnull TWKeyDerivationPBKDF2_512(TWString *_Nonnull password, TWData *_Nonnull salt,  uint32_t iterations, size_t keyLength) {
    uint8_t result[keyLength];
    pbkdf2_hmac_sha512((const uint8_t *) TWStringUTF8Bytes(password), static_cast<int>(TWStringSize(password)), TWDataBytes(salt), static_cast<int>(TWDataSize(salt)), iterations, result, static_cast<int>(keyLength));
    return TWDataCreateWithBytes(result, keyLength);
}
