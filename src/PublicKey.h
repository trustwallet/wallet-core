// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"

#include <array>
#include <vector>

namespace TW {

enum class PublicKeyType {
    secp256k1,
    secp256k1Extended,
    ed25519
};

class PublicKey {
public:
    /// The number of bytes in a secp256k1 public key.
    static const size_t secp256k1Size = 33;

    /// The number of bytes in a ed25519 public key.
    static const size_t ed25519Size = 33;

    /// The number of bytes in a secp256k1 extended public key.
    static const size_t secp256k1ExtendedSize = 65;

    /// The public key bytes.
    Data bytes;

    /// Type of public key.
    PublicKeyType type() const {
        switch (bytes.at(0)) {
        case 1:
            return PublicKeyType::ed25519;
        case 2:
        case 3:
            return PublicKeyType::secp256k1;
        case 4:
        case 6:
        case 7:
            return PublicKeyType::secp256k1Extended;
        default:
            assert(false && "Invalid public key data");
            return PublicKeyType::secp256k1;
        }
    }

    /// Determines if a collection of bytes makes a valid public key.
    template <typename T>
    static bool isValid(const T& data) {
        const auto size = std::end(data) - std::begin(data);
        if (size == 0) {
            return false;
        }
        switch (data[0]) {
        case 1:
            return size == ed25519Size;
        case 2:
        case 3:
            return size == secp256k1Size;
        case 4:
        case 6:
        case 7:
            return size == secp256k1ExtendedSize;
        default:
            return false;
        }
    }

    /// Initializes a public key with a collection of bytes.
    template<typename T>
    explicit PublicKey(const T& data) {
        assert(isValid(data));
        bytes.reserve(data.size());
        std::copy(std::begin(data), std::end(data), std::back_inserter(bytes));
    }

    /// Determines if this is a compressed public key.
    bool isCompressed() const {
        return bytes[0] >= 1 && bytes[0] <= 3;
    }

    /// Returns a compressed version of this public key.
    PublicKey compressed() const;

    /// Verifies a signature for the provided message.
    bool verify(const std::vector<uint8_t>& signature, const std::vector<uint8_t>& message) const;
};

inline bool operator==(const PublicKey& lhs, const PublicKey& rhs) { return lhs.bytes == rhs.bytes; }
inline bool operator!=(const PublicKey& lhs, const PublicKey& rhs) { return lhs.bytes != rhs.bytes; }

} // namespace

/// Wrapper for C interface.
struct TWPublicKey {
    TW::PublicKey impl;
};
