// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrezorCrypto/memzero.h>
#include <TrezorCrypto/pbkdf2.h>
#include <TrustWalletCore/TWPBKDF2.h>

#include "Data.h"

using namespace TW;

TWData* _Nullable TWPBKDF2Sha256Hmac(TWData* _Nonnull password, TWData* _Nonnull salt,
                                     uint32_t iterations, uint32_t dkLen) {

    PBKDF2_HMAC_SHA256_CTX ctx;
    Data key(dkLen);

    pbkdf2_hmac_sha256_Init(&ctx, TWDataBytes(password), static_cast<int>(TWDataSize(password)),
                            TWDataBytes(salt), static_cast<int>(TWDataSize(salt)), 1);

    for (int i = 0; i < iterations; i += 1) {
        pbkdf2_hmac_sha256_Update(&ctx, 1);
    }

    pbkdf2_hmac_sha256_Final(&ctx, key.data());
    memzero(&ctx, sizeof(ctx));

    return TWDataCreateWithData(&key);
}

TWData* _Nullable TWPBKDF2Sha512Hmac(TWData* _Nonnull password, TWData* _Nonnull salt,
                                     uint32_t iterations, uint32_t dkLen) {
    PBKDF2_HMAC_SHA512_CTX ctx;
    Data key(dkLen);

    pbkdf2_hmac_sha512_Init(&ctx, TWDataBytes(password), static_cast<int>(TWDataSize(password)),
                            TWDataBytes(salt), static_cast<int>(TWDataSize(salt)), 1);

    for (int i = 0; i < iterations; i += 1) {
        pbkdf2_hmac_sha512_Update(&ctx, 1);
    }

    pbkdf2_hmac_sha512_Final(&ctx, key.data());
    memzero(&ctx, sizeof(ctx));

    return TWDataCreateWithData(&key);
}
