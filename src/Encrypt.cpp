// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Encrypt.h"
#include "Data.h"
#include <cassert>
#include <cstring>
#include <stdexcept>

#include "rust/Wrapper.h"

namespace TW::Encrypt {

Data AESCBCEncrypt(const Data& key, const Data& data, Data& iv, TWAESPaddingMode paddingMode) {
    Rust::TWDataWrapper dataWrapper = data;
    Rust::TWDataWrapper ivWrapper = iv;
    Rust::TWDataWrapper keyWrapper = key;

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
    Rust::TWDataWrapper dataWrapper = data;
    Rust::TWDataWrapper ivWrapper = iv;
    Rust::TWDataWrapper keyWrapper = key;

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
    Rust::TWDataWrapper dataWrapper = data;
    Rust::TWDataWrapper ivWrapper = iv;
    Rust::TWDataWrapper keyWrapper = key;

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
    Rust::TWDataWrapper dataWrapper = data;
    Rust::TWDataWrapper ivWrapper = iv;
    Rust::TWDataWrapper keyWrapper = key;

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
