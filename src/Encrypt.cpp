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
    Rust::TWDataWrapper dataWrapper = data;
    Rust::TWDataWrapper ivWrapper = iv;
    Rust::TWDataWrapper keyWrapper = key;

    Rust::TWDataWrapper res = Rust::crypto_aes_cbc_encrypt(
        dataWrapper.get(),
        ivWrapper.get(),
        keyWrapper.get(),
        paddingMode
    );
    auto resData = res.toDataOrDefault();
    if (resData.empty()) {
        throw std::runtime_error("Invalid aes cbc encrypt");
    }
    return resData;
}

Data AESCBCDecrypt(const Data& key, const Data& data, Data& iv, TWAESPaddingMode paddingMode) {
    Rust::TWDataWrapper dataWrapper = data;
    Rust::TWDataWrapper ivWrapper = iv;
    Rust::TWDataWrapper keyWrapper = key;

    Rust::TWDataWrapper res = Rust::crypto_aes_cbc_decrypt(
        dataWrapper.get(),
        ivWrapper.get(),
        keyWrapper.get(),
        paddingMode
    );
    auto resData = res.toDataOrDefault();
    if (resData.empty()) {
        throw std::runtime_error("Invalid aes cbc decrypt");
    }
    return resData;
}

Data AESCTREncrypt(const Data& key, const Data& data, Data& iv) {
    Rust::TWDataWrapper dataWrapper = data;
    Rust::TWDataWrapper ivWrapper = iv;
    Rust::TWDataWrapper keyWrapper = key;

    Rust::TWDataWrapper res = Rust::crypto_aes_ctr_encrypt(
        dataWrapper.get(),
        ivWrapper.get(),
        keyWrapper.get()
    );
    auto resData = res.toDataOrDefault();
    if (resData.empty()) {
        throw std::runtime_error("Invalid aes ctr encrypt");
    }
    return resData;
}

Data AESCTRDecrypt(const Data& key, const Data& data, Data& iv) {
    Rust::TWDataWrapper dataWrapper = data;
    Rust::TWDataWrapper ivWrapper = iv;
    Rust::TWDataWrapper keyWrapper = key;

    Rust::TWDataWrapper res = Rust::crypto_aes_ctr_decrypt(
        dataWrapper.get(),
        ivWrapper.get(),
        keyWrapper.get()
    );
    auto resData = res.toDataOrDefault();
    if (resData.empty()) {
        throw std::runtime_error("Invalid aes ctr decrypt");
    }
    return resData;
}

} // namespace TW::Encrypt
