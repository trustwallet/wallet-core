// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Encrypt.h"
#include "Data.h"
#include <cassert>
#include <cstring>
#include <stdexcept>

#include "rust/Wrapper.h"
#include "TrustWalletCore/Generated/TWCrypto.h"

namespace TW::Encrypt {

Data AESCBCEncrypt(const Data& key, const Data& data, Data& iv, TWAESPaddingMode paddingMode) {
<<<<<<< HEAD
    Rust::TWDataWrapper dataWrapper = data;
    Rust::TWDataWrapper ivWrapper = iv;
    Rust::TWDataWrapper keyWrapper = key;
=======
    if (iv.size() != AES_BLOCK_SIZE) {
        throw std::invalid_argument("Invalid iv size");
    }

    aes_encrypt_ctx ctx;
    if (aes_encrypt_key(key.data(), static_cast<int>(key.size()), &ctx) == EXIT_FAILURE) {
        throw std::invalid_argument("Invalid key");
    }
>>>>>>> master

    Rust::TWDataWrapper res = Rust::tw_aes_encrypt_cbc(
        keyWrapper.get(),
        dataWrapper.get(),
        ivWrapper.get(),
        paddingMode
    );
    auto resData = res.toDataOrDefault();
    if (resData.empty()) {
        throw std::runtime_error("Invalid aes cbc encrypt");
    }
    return resData;
}

Data AESCBCDecrypt(const Data& key, const Data& data, Data& iv, TWAESPaddingMode paddingMode) {
<<<<<<< HEAD
    Rust::TWDataWrapper dataWrapper = data;
    Rust::TWDataWrapper ivWrapper = iv;
    Rust::TWDataWrapper keyWrapper = key;
=======
    if (iv.size() != AES_BLOCK_SIZE) {
        throw std::invalid_argument("Invalid iv size");
    }

    const size_t blockSize = AES_BLOCK_SIZE;
    if (data.size() % blockSize != 0) {
        throw std::invalid_argument("Invalid data size");
    }
    assert((data.size() % blockSize) == 0);
>>>>>>> master

    Rust::TWDataWrapper res = Rust::tw_aes_decrypt_cbc(
        keyWrapper.get(),
        dataWrapper.get(),
        ivWrapper.get(),
        paddingMode
    );
    auto resData = res.toDataOrDefault();
    if (resData.empty()) {
        throw std::runtime_error("Invalid aes cbc decrypt");
    }
    return resData;
}

Data AESCTREncrypt(const Data& key, const Data& data, Data& iv) {
<<<<<<< HEAD
    Rust::TWDataWrapper dataWrapper = data;
    Rust::TWDataWrapper ivWrapper = iv;
    Rust::TWDataWrapper keyWrapper = key;
=======
    if (iv.size() != AES_BLOCK_SIZE) {
        throw std::invalid_argument("Invalid iv size");
    }

    aes_encrypt_ctx ctx;
    if (aes_encrypt_key(key.data(), static_cast<int>(key.size()), &ctx) != EXIT_SUCCESS) {
        throw std::invalid_argument("Invalid key");
    }
>>>>>>> master

    Rust::TWDataWrapper res = Rust::tw_aes_encrypt_ctr(
        keyWrapper.get(),
        dataWrapper.get(),
        ivWrapper.get()
    );
    auto resData = res.toDataOrDefault();
    if (resData.empty()) {
        throw std::runtime_error("Invalid aes ctr encrypt");
    }
    return resData;
}

Data AESCTRDecrypt(const Data& key, const Data& data, Data& iv) {
<<<<<<< HEAD
    Rust::TWDataWrapper dataWrapper = data;
    Rust::TWDataWrapper ivWrapper = iv;
    Rust::TWDataWrapper keyWrapper = key;
=======
    if (iv.size() != AES_BLOCK_SIZE) {
        throw std::invalid_argument("Invalid iv size");
    }

    aes_encrypt_ctx ctx;
    if (aes_encrypt_key(key.data(), static_cast<int>(key.size()), &ctx) != EXIT_SUCCESS) {
        throw std::invalid_argument("Invalid key");
    }
>>>>>>> master

    Rust::TWDataWrapper res = Rust::tw_aes_decrypt_ctr(
        keyWrapper.get(),
        dataWrapper.get(),
        ivWrapper.get()
    );
    auto resData = res.toDataOrDefault();
    if (resData.empty()) {
        throw std::runtime_error("Invalid aes ctr decrypt");
    }
    return resData;
}

} // namespace TW::Encrypt
