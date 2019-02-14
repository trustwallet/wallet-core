// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <array>
#include <vector>

namespace TW {

class PublicKey {
public:
    /// The number of bytes in a compressed public key.
    static const size_t compressedSize = 33;

    /// The number of bytes in an uncompressed public key.
    static const size_t uncompressedSize = 65;

    /// The public key bytes.
    std::array<uint8_t, uncompressedSize> bytes;

    /// Determines if a collection of bytes makes a valid public key.
    template <typename T>
    static bool isValid(const T& data) {
        const auto size = std::end(data) - std::begin(data);
        if (size == 0) {
            return false;
        }
        switch (data[0]) {
        case 2:
        case 3:
            return size == compressedSize;
        case 4:
        case 6:
        case 7:
            return size == uncompressedSize;
        default:
            return false;
        }
    }
    
    /// Initializes a public key with a collection of bytes.
    template<typename T>
    explicit PublicKey(const T& data) {
        assert(isValid(data));
        std::copy(std::begin(data), std::end(data), std::begin(bytes));
    }

    /// Determines if this is a compressed public key.
    bool isCompressed() const {
        return bytes[0] == 2 || bytes[0] == 3;
    }

    /// Returns a compressed version of this public key.
    PublicKey compressed() const;

    /// Verifies a signature for the provided message.
    bool verify(const std::vector<uint8_t>& signature, const std::vector<uint8_t>& message) const;
};

inline bool operator==(const PublicKey& lhs, const PublicKey& rhs) { return lhs.bytes == rhs.bytes; }
inline bool operator!=(const PublicKey& lhs, const PublicKey& rhs) { return lhs.bytes != rhs.bytes; }

} // namespace
