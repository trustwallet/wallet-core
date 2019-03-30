// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWAES.h>

#include <TrezorCrypto/aes.h>

#include <cassert>
#include <cstring>

TWData *_Nullable TWAESCBCEncrypt(TWData *_Nonnull key, TWData *_Nonnull data, TWData *_Nonnull iv) {
    aes_encrypt_ctx ctx;
    if (aes_encrypt_key(TWDataBytes(key), TWDataSize(key), &ctx) == EXIT_FAILURE) {
        return nullptr;
    }

    const auto resultSize = 16 * ((TWDataSize(data) + 15) / 16);
    TWData *result = TWDataCreateWithSize(resultSize);
    size_t i;
    for (i = 0; i <= TWDataSize(data) - 16; i += 16) {
        aes_cbc_encrypt(TWDataBytes(data) + i, TWDataBytes(result) + i, 16, TWDataBytes(iv), &ctx);
    }
    if (i < TWDataSize(data)) {
        uint8_t padded[16] = {0};
        std::memcpy(padded, TWDataBytes(data) + i, TWDataSize(data) - i);
        aes_cbc_encrypt(padded, TWDataBytes(result) + i, 16, TWDataBytes(iv), &ctx);
    }

    return result;
}

TWData *_Nullable TWAESCBCDecrypt(TWData *_Nonnull key, TWData *_Nonnull data, TWData *_Nonnull iv) {
    assert((TWDataSize(data) % 16) == 0);

    aes_decrypt_ctx ctx;
    if (aes_decrypt_key(TWDataBytes(key), TWDataSize(key), &ctx) == EXIT_FAILURE) {
        return nullptr;
    }

    TWData *result = TWDataCreateWithSize(TWDataSize(data));
    for (std::size_t i = 0; i < TWDataSize(data); i += 16) {
        aes_cbc_decrypt(TWDataBytes(data) + i, TWDataBytes(result) + i, 16, TWDataBytes(iv), &ctx);
    }

    return result;
}

TWData *_Nullable TWAESCTREncrypt(TWData *_Nonnull key, TWData *_Nonnull data, TWData *_Nonnull iv) {
	aes_encrypt_ctx ctx;
    if (aes_encrypt_key(TWDataBytes(key), TWDataSize(key), &ctx) == EXIT_FAILURE) {
        return nullptr;
    }

    TWData *result = TWDataCreateWithSize(TWDataSize(data));
    aes_ctr_encrypt(TWDataBytes(data), TWDataBytes(result), TWDataSize(data), TWDataBytes(iv), aes_ctr_cbuf_inc, &ctx);

    return result;
}

TWData *_Nullable TWAESCTRDecrypt(TWData *_Nonnull key, TWData *_Nonnull data, TWData *_Nonnull iv) {
    aes_encrypt_ctx ctx;
    if (aes_encrypt_key(TWDataBytes(key), TWDataSize(key), &ctx) == EXIT_FAILURE) {
        return nullptr;
    }

    TWData *result = TWDataCreateWithSize(TWDataSize(data));
    aes_ctr_decrypt(TWDataBytes(data), TWDataBytes(result), TWDataSize(data), TWDataBytes(iv), aes_ctr_cbuf_inc, &ctx);

    return result;
}
