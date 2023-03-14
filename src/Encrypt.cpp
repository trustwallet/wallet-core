// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Encrypt.h"
#include "Data.h"
#include <TrezorCrypto/aes.h>
#include <cassert>
#include <cstring>
#include <stdexcept>

namespace TW::Encrypt {

size_t paddingSize(size_t origSize, size_t blockSize, TWAESPaddingMode paddingMode) {
    if (origSize % blockSize == 0) {
        // even blocks
        if (paddingMode == TWAESPaddingModePKCS7) {
            return blockSize;
        }
        return 0;
    }
    // non-even
    return blockSize - origSize % blockSize;
}

Data AESCBCEncrypt(const Data& key, const Data& data, Data& iv, TWAESPaddingMode paddingMode) {
    aes_encrypt_ctx ctx;
    if (aes_encrypt_key(key.data(), static_cast<int>(key.size()), &ctx) == EXIT_FAILURE) {
        throw std::invalid_argument("Invalid key");
    }

    // Message is padded to round block size, or by a full padding block if even
    const size_t blockSize = AES_BLOCK_SIZE;
    const auto padding = paddingSize(data.size(), blockSize, paddingMode);
    const auto resultSize = data.size() + padding;
    Data result(resultSize);
    size_t idx;
    for (idx = 0; idx < resultSize - blockSize; idx += blockSize) {
        aes_cbc_encrypt(data.data() + idx, result.data() + idx, blockSize, iv.data(), &ctx);
    }
    // last block
    if (idx < resultSize) {
        uint8_t padded[blockSize] = {0};
        if (paddingMode == TWAESPaddingModePKCS7) {
            std::memset(padded, static_cast<int>(padding), blockSize);
        }
        std::memcpy(padded, data.data() + idx, data.size() - idx);
        aes_cbc_encrypt(padded, result.data() + idx, blockSize, iv.data(), &ctx);
    }

    return result;
}

Data AESCBCDecrypt(const Data& key, const Data& data, Data& iv, TWAESPaddingMode paddingMode) {
    const size_t blockSize = AES_BLOCK_SIZE;
    if (data.size() % blockSize != 0) {
        throw std::invalid_argument("Invalid data size");
    }
    assert((data.size() % blockSize) == 0);

    aes_decrypt_ctx ctx;
    if (aes_decrypt_key(key.data(), static_cast<int>(key.size()), &ctx) != EXIT_SUCCESS) {
        throw std::invalid_argument("Invalid key");
    }

    Data result(data.size());
    for (std::size_t i = 0; i < data.size(); i += blockSize) {
        aes_cbc_decrypt(data.data() + i, result.data() + i, blockSize, iv.data(), &ctx);
    }

    if (paddingMode == TWAESPaddingModePKCS7 && result.size() > 0) {
        // need to remove padding
        assert(result.size() > 0);
        const byte paddingSize = result[result.size() - 1];
        if (paddingSize <= result.size()) {
            // remove last paddingSize number of bytes
            const size_t unpaddedSize = result.size() - paddingSize;
            Data resultUnpadded = TW::data(result.data(), unpaddedSize);
            return resultUnpadded;
        }
    }
    return result;
}

Data AESCTREncrypt(const Data& key, const Data& data, Data& iv) {
	aes_encrypt_ctx ctx;
    if (aes_encrypt_key(key.data(), static_cast<int>(key.size()), &ctx) != EXIT_SUCCESS) {
        throw std::invalid_argument("Invalid key");
    }

    Data result(data.size());
    aes_ctr_encrypt(data.data(), result.data(), static_cast<int>(data.size()), iv.data(), aes_ctr_cbuf_inc, &ctx);
    return result;
}

Data AESCTRDecrypt(const Data& key, const Data& data, Data& iv) {
    aes_encrypt_ctx ctx;
    if (aes_encrypt_key(key.data(), static_cast<int>(key.size()), &ctx) != EXIT_SUCCESS) {
        throw std::invalid_argument("Invalid key");
    }

    Data result(data.size());
    aes_ctr_decrypt(data.data(), result.data(), static_cast<int>(data.size()), iv.data(), aes_ctr_cbuf_inc, &ctx);
    return result;
}

} // namespace TW::Encrypt
