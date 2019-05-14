// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "PublicKey.h"

#include <TrustWalletCore/TWCurve.h>

#include <array>
#include <vector>

namespace TW {

class PrivateKey {
  public:
    /// The number of bytes in a private key.
    static const size_t size = 32;

    /// The private key bytes.
    std::array<uint8_t, size> bytes;

    /// Determines if a collection of bytes makes a valid private key.
    template <typename T>
    static bool isValid(const T& data) {
        // Check length
        if (data.size() != size) {
            return false;
        }

        // Check for zero address
        for (size_t i = 0; i < size; i += 1) {
            if (data[i] != 0) {
                return true;
            }
        }

        return false;
    }

    /// Initializes a private key with a collection of bytes.
    template <typename T>
    explicit PrivateKey(const T& data) {
        if (!isValid(data)) {
            throw std::invalid_argument("Invalid private key data");
        }
        std::copy(std::begin(data), std::end(data), std::begin(bytes));
    }

    /// Initializes a private key with a static array of bytes.
    PrivateKey(std::array<uint8_t, size>&& array) : bytes(array) {}

    PrivateKey(const PrivateKey& other) = default;
    PrivateKey& operator=(const PrivateKey& other) = default;

    PrivateKey(PrivateKey&& other) = default;
    PrivateKey& operator=(PrivateKey&& other) = default;

    virtual ~PrivateKey();

    /// Returns the public key for this private key.
    PublicKey getPublicKey(enum TWPublicKeyType type) const;

    /// Signs a digest using the given ECDSA curve.
    Data sign(const Data& digest, TWCurve curve) const;

    /// Signs a digest using the given ECDSA curve and prepends the recovery id (a la graphene)
    /// Only a sig that passes canonicalChecker is returned
    Data sign(const Data& digest, TWCurve curve, int(*canonicalChecker)(uint8_t by, uint8_t sig[64])) const;

    /// Signs a digest using the given ECDSA curve. The result is encoded with
    /// DER.
    Data signAsDER(const Data& digest, TWCurve curve) const;

    /// Signs a digest using given ECDSA curve, returns schnorr signature
    Data signSchnorr(const Data& message, TWCurve curve) const;
};

inline bool operator==(const PrivateKey& lhs, const PrivateKey& rhs) {
    return lhs.bytes == rhs.bytes;
}
inline bool operator!=(const PrivateKey& lhs, const PrivateKey& rhs) {
    return lhs.bytes != rhs.bytes;
}

} // namespace TW

/// Wrapper for C interface.
struct TWPrivateKey {
    TW::PrivateKey impl;
};
