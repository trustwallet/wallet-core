// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "CryptoBox.h"

namespace TW::CryptoBox {

bool PublicKey::isValid(const Data& bytes) {
    Rust::TWDataWrapper data = bytes;
    return Rust::tw_crypto_box_public_key_is_valid(data.get());
}

std::optional<PublicKey> PublicKey::fromBytes(const Data& bytes) {
    Rust::TWDataWrapper data = bytes;
    if (!Rust::tw_crypto_box_public_key_is_valid(data.get())) {
        return std::nullopt;
    }
    auto* publicKey = Rust::tw_crypto_box_public_key_create_with_data(data.get());
    return PublicKey(PublicKeyPtr(publicKey, Rust::tw_crypto_box_public_key_delete));
}

Data PublicKey::getData() const {
    Rust::TWDataWrapper data = Rust::tw_crypto_box_public_key_data(impl.get());
    return data.toDataOrDefault();
}

SecretKey::SecretKey() {
    auto* secretKey = Rust::tw_crypto_box_secret_key_create();
    impl = SecretKeyPtr(secretKey, Rust::tw_crypto_box_secret_key_delete);
}

bool SecretKey::isValid(const Data& bytes) {
    Rust::TWDataWrapper data = bytes;
    return Rust::tw_crypto_box_secret_key_is_valid(data.get());
}

std::optional<SecretKey> SecretKey::fromBytes(const Data& bytes) {
    Rust::TWDataWrapper data = bytes;
    if (!Rust::tw_crypto_box_secret_key_is_valid(data.get())) {
        return std::nullopt;
    }
    auto* secretKey = Rust::tw_crypto_box_secret_key_create_with_data(data.get());
    return SecretKey(SecretKeyPtr(secretKey, Rust::tw_crypto_box_secret_key_delete));
}

PublicKey SecretKey::getPublicKey() const noexcept {
    auto* publicKey = Rust::tw_crypto_box_secret_key_get_public_key(impl.get());
    return PublicKey(PublicKeyPtr(publicKey, Rust::tw_crypto_box_public_key_delete));
}

Data SecretKey::getData() const {
    Rust::TWDataWrapper data = Rust::tw_crypto_box_secret_key_data(impl.get());
    return data.toDataOrDefault();
}

Data encryptEasy(const SecretKey& mySecret, const PublicKey& otherPubkey, const Data& message) {
    Rust::TWDataWrapper messageData = message;
    Rust::TWDataWrapper encrypted = Rust::tw_crypto_box_encrypt_easy(mySecret.impl.get(), otherPubkey.impl.get(), messageData.get());
    return encrypted.toDataOrDefault();
}

std::optional<Data> decryptEasy(const SecretKey& mySecret, const PublicKey& otherPubkey, const Data& encrypted) {
    Rust::TWDataWrapper encryptedData = encrypted;
    Rust::TWDataWrapper decryptedData = Rust::tw_crypto_box_decrypt_easy(mySecret.impl.get(), otherPubkey.impl.get(), encryptedData.get());
    if (!decryptedData.ptr) {
        return std::nullopt;
    }
    return decryptedData.toDataOrDefault();
}

} // namespace TW::CryptoBox