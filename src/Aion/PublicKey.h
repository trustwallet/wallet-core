// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <array>
#include <vector>

namespace TW {
namespace Aion {

class PublicKey {
public:
    /// The number of bytes in Aion public key.
    static const size_t size = 32;

    /// The public key bytes.
    std::array<uint8_t, size> bytes;

    /// Determines if a collection of bytes makes a valid public key.
    template <typename T>
    static bool isValid(const T& data) {
        return size == std::end(data) - std::begin(data);
    }
    
    /// Initializes a public key with a collection of bytes.
    template<typename T>
    explicit PublicKey(const T& data) {
        assert(isValid(data));
        std::copy(std::begin(data), std::end(data), std::begin(bytes));
    }

};

inline bool operator==(const PublicKey& lhs, const PublicKey& rhs) { return lhs.bytes == rhs.bytes; }
inline bool operator!=(const PublicKey& lhs, const PublicKey& rhs) { return lhs.bytes != rhs.bytes; }

}} // namespace

/// Wrapper for C interface.
struct TWAionPublicKey {
    TW::Aion::PublicKey impl;
};
