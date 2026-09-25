// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "rust/Wrapper.h"

namespace TW::CryptoBox {

using PublicKeyPtr = std::shared_ptr<Rust::TWCryptoBoxPublicKey>;
using SecretKeyPtr = std::shared_ptr<Rust::TWCryptoBoxSecretKey>;

/// Public key used in `crypto_box` cryptography.
struct PublicKey {
    explicit PublicKey(PublicKeyPtr ptr): impl(std::move(ptr)) {
    }

    /// Determines if the given public key is valid or not.
    static bool isValid(const Data& bytes);

    /// Create a `crypto_box` public key with the given block of data.
    static std::optional<PublicKey> fromBytes(const Data& bytes);

    /// Returns the raw data of the given public-key.
    Data getData() const;

    PublicKeyPtr impl;
};

/// Secret key used in `crypto_box` cryptography.
class SecretKey {
public:
    /// Create a random secret key.
    SecretKey();

    explicit SecretKey(SecretKeyPtr ptr): impl(std::move(ptr)) {
    }

    /// Determines if the given secret key is valid or not.
    static bool isValid(const Data& bytes);

    /// Create a `crypto_box` secret key with the given block of data.
    static std::optional<SecretKey> fromBytes(const Data& bytes);

    /// Returns the public key associated with the given `key`.
    PublicKey getPublicKey() const noexcept;

    /// Returns the raw data of the given secret-key.
    Data getData() const;

    SecretKeyPtr impl;
};

/// Encrypts message using `my_secret` and `other_pubkey`.
/// The output will have a randomly generated nonce prepended to it.
/// The output will be Overhead + 24 bytes longer than the original.
Data encryptEasy(const SecretKey& mySecret, const PublicKey& otherPubkey, const Data& message);

/// Decrypts box produced by `TWCryptoBoxEncryptEasy`.
/// We assume a 24-byte nonce is prepended to the encrypted text in box.
std::optional<Data> decryptEasy(const SecretKey& mySecret, const PublicKey& otherPubkey, const Data& encrypted);

} // namespace TW::CryptoBox

/// Wrapper for C interface.
struct TWCryptoBoxSecretKey {
    TW::CryptoBox::SecretKey impl;
};

/// Wrapper for C interface.
struct TWCryptoBoxPublicKey {
    TW::CryptoBox::PublicKey impl;
};
