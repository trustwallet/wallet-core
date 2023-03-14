// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "../PublicKey.h"

#include <string>

namespace TW::Ripple {

enum class TagFlag: byte { none = 0x00, classic = 0x01};

class XAddress {
  public:
    /// Number of bytes in a X-address.
    static const size_t size = 31;

    /// Public key hash length.
    static const size_t keyHashSize = 20;

    /// Address data consisting of public key hash
    std::array<byte, keyHashSize> bytes;

    /// destination tag
    uint32_t tag;

    /// destination tag flag, none/32/64bit
    TagFlag flag = TagFlag::classic;

    /// Determines whether a string makes a valid  address.
    static bool isValid(const std::string& string);

    /// Initializes a Ripple X-address with a string representation.
    explicit XAddress(const std::string& string);

    /// Initializes a Ripple X-address with a public key.
    explicit XAddress(const PublicKey& publicKey, const uint32_t tag);

    /// Returns a string representation of the address.
    std::string string() const;
};

inline bool operator==(const XAddress& lhs, const XAddress& rhs) {
    return lhs.bytes == rhs.bytes;
}

} // namespace TW::Ripple

/// Wrapper for C interface.
struct TWRippleXAddress {
    TW::Ripple::XAddress impl;
};
