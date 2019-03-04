// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <array>
#include <vector>

namespace TW {
namespace Nimiq {

class EdPrivateKey {
public:
    /// The number of bytes in a private key.
    static const size_t size = 32;

    /// The private key bytes.
    std::array<uint8_t, size> bytes;

    /// Determines if a collection of bytes makes a valid private key.
    template<typename T>
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
    template<typename T>
    explicit EdPrivateKey(const T& data) {
        assert(data.size() == size);
        std::copy(std::begin(data), std::end(data), std::begin(bytes));
    }

    /// Initializes a private key with a static array of bytes.
    EdPrivateKey(std::array<uint8_t, size>&& array) : bytes(array) {}

    ~EdPrivateKey();

    /// Returns the public key data for this private key.
    std::array<uint8_t, 32> getPublicKey() const;

    /// Signs a digest using Ed25519.
    std::array<uint8_t, 64> sign(const std::vector<uint8_t>&  digest) const;
};

inline bool operator==(const EdPrivateKey& lhs, const EdPrivateKey& rhs) { return lhs.bytes == rhs.bytes; }
inline bool operator!=(const EdPrivateKey& lhs, const EdPrivateKey& rhs) { return lhs.bytes != rhs.bytes; }

}} // namespace
