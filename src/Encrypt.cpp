// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Encrypt.h"

#include "Data.h"

#include <TrezorCrypto/aes.h>

#include <cassert>

namespace TW::Encrypt {

Data AESCBCEncrypt(const Data& key, const Data& data, Data& iv) {
    aes_encrypt_ctx ctx;
    if (aes_encrypt_key(key.data(), key.size(), &ctx) == EXIT_FAILURE) {
        throw std::invalid_argument("Invalid key");
    }

    const auto resultSize = 16 * ((data.size() + 15) / 16);
    Data result(resultSize);
    size_t i;
    for (i = 0; i <= data.size() - 16; i += 16) {
        aes_cbc_encrypt(data.data() + i, result.data() + i, 16, iv.data(), &ctx);
    }
    if (i < data.size()) {
        uint8_t padded[16] = {0};
        std::memcpy(padded, data.data() + i, data.size() - i);
        aes_cbc_encrypt(padded, result.data() + i, 16, iv.data(), &ctx);
    }

    return result;
}

Data AESCBCDecrypt(const Data& key, const Data& data, Data& iv) {
    if (data.size() % 16 != 0) {
        throw std::invalid_argument("Invalid data size");
    }
    assert((data.size() % 16) == 0);

    aes_decrypt_ctx ctx;
    if (aes_decrypt_key(key.data(), key.size(), &ctx) != EXIT_SUCCESS) {
        throw std::invalid_argument("Invalid key");
    }

    Data result(data.size());
    for (std::size_t i = 0; i < data.size(); i += 16) {
        aes_cbc_decrypt(data.data() + i, result.data() + i, 16, iv.data(), &ctx);
    }
    return result;
}

Data AESCTREncrypt(const Data& key, const Data& data, Data& iv) {
	aes_encrypt_ctx ctx;
    if (aes_encrypt_key(key.data(), key.size(), &ctx) != EXIT_SUCCESS) {
        throw std::invalid_argument("Invalid key");
    }

    Data result(data.size());
    aes_ctr_encrypt(data.data(), result.data(), data.size(), iv.data(), aes_ctr_cbuf_inc, &ctx);
    return result;
}

Data AESCTRDecrypt(const Data& key, const Data& data, Data& iv) {
    aes_encrypt_ctx ctx;
    if (aes_encrypt_key(key.data(), key.size(), &ctx) != EXIT_SUCCESS) {
        throw std::invalid_argument("Invalid key");
    }

    Data result(data.size());
    aes_ctr_decrypt(data.data(), result.data(), data.size(), iv.data(), aes_ctr_cbuf_inc, &ctx);
    return result;
}

} // namespace TW::Encrypt
