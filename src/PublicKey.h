// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "Hash.h"

#include <TrustWalletCore/TWPublicKeyType.h>

#include <array>
#include <cassert>
#include <stdexcept>
#include <vector>

namespace TW {

class PublicKey {
  public:
    /// The number of bytes in a secp256k1 and nist256p1 public key.
    static const size_t secp256k1Size = 33;

    /// The number of bytes in a ed25519 public key.
    static const size_t ed25519Size = 32;

    /// The number of bytes in a secp256k1 and nist256p1 extended public key.
    static const size_t secp256k1ExtendedSize = 65;

    /// The public key bytes.
    Data bytes;

    /// The of public key.
    ///
    /// This has information about the elliptic curve and other parameters
    /// used when generating the public key.
    enum TWPublicKeyType type;

    /// Determines if a collection of bytes makes a valid public key of the
    /// given type.
    template <typename T>
    static bool isValid(const T& data, enum TWPublicKeyType type) {
        const auto size = std::end(data) - std::begin(data);
        if (size == 0) {
            return false;
        }
        switch (type) {
        case TWPublicKeyTypeED25519:
            return size == ed25519Size || (size == ed25519Size + 1 && data[0] == 0x01);
        case TWPublicKeyTypeED25519Blake2b:
            return size == ed25519Size;
        case TWPublicKeyTypeSECP256k1:
        case TWPublicKeyTypeNIST256p1:
            return size == secp256k1Size && (data[0] == 0x02 || data[0] == 0x03);
        case TWPublicKeyTypeSECP256k1Extended:
        case TWPublicKeyTypeNIST256p1Extended:
            return size == secp256k1ExtendedSize && data[0] == 0x04;
        default:
            return false;
        }
    }

    /// Initializes a public key with a collection of bytes.
    ///
    /// @throws std::invalid_argument if the data is not a valid public key.
    template <typename T>
    explicit PublicKey(const T& data, enum TWPublicKeyType type) : type(type) {
        if (!isValid(data, type)) {
            throw std::invalid_argument("Invalid public key data");
        }
        switch (type) {
        case TWPublicKeyTypeSECP256k1:
        case TWPublicKeyTypeNIST256p1:
        case TWPublicKeyTypeSECP256k1Extended:
        case TWPublicKeyTypeNIST256p1Extended:
            bytes.reserve(data.size());
            std::copy(std::begin(data), std::end(data), std::back_inserter(bytes));
            break;

        case TWPublicKeyTypeED25519:
            bytes.reserve(ed25519Size);
            if (data.size() == ed25519Size + 1) {
                std::copy(std::begin(data) + 1, std::end(data), std::back_inserter(bytes));
            } else {
                std::copy(std::begin(data), std::end(data), std::back_inserter(bytes));
            }
            break;
        case TWPublicKeyTypeED25519Blake2b:
            bytes.reserve(ed25519Size);
            if (data.size() == ed25519Size + 1) {
                std::copy(std::begin(data) + 1, std::end(data), std::back_inserter(bytes));
            } else {
                std::copy(std::begin(data), std::end(data), std::back_inserter(bytes));
            }
            break;
        }
    }

    /// Determines if this is a compressed public key.
    bool isCompressed() const {
        return type != TWPublicKeyTypeSECP256k1Extended && type != TWPublicKeyTypeNIST256p1Extended;
    }

    /// Returns a compressed version of this public key.
    PublicKey compressed() const;

    /// Returns an extended version of this public key.
    PublicKey extended() const;

    /// Verifies a signature for the provided message.
    bool verify(const Data& signature, const Data& message) const;

    /// Verifies a schnorr signature for the provided message.
    bool verifySchnorr(const Data& signature, const Data& message) const;

    /// Computes the public key hash.
    ///
    /// The public key hash is computed by applying the hasher to the public key
    /// bytes and then prepending the prefix.
    Data hash(const Data& prefix, Hash::Hasher hasher = Hash::sha256ripemd, bool skipTypeByte = false) const;
};

inline bool operator==(const PublicKey& lhs, const PublicKey& rhs) {
    return lhs.bytes == rhs.bytes;
}
inline bool operator!=(const PublicKey& lhs, const PublicKey& rhs) {
    return lhs.bytes != rhs.bytes;
}

} // namespace TW

/// Wrapper for C interface.
struct TWPublicKey {
    TW::PublicKey impl;
};
