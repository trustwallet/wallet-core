// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
#pragma once
#include "../Base58Address.h"
#include "../PrivateKey.h"
#include "../PublicKey.h"

#include <string>

namespace TW::NULS {

class Address : public Base58Address<24> {
  public:
    /// NULS Main Net Chain ID
    static const uint16_t MainNetID = 8964;

    /// Determines whether a string makes a valid address.
    static bool isValid(const std::string& string);

    /// Process NULS private key length is 31 or 33
    static PrivateKey importHexPrivateKey(std::string hexPrivateKey);

    /// Initializes an address from a string representation.
    explicit Address(const std::string& string);

    /// Initializes an address from a public key.
    explicit Address(const PublicKey& publicKey);

    /// Initializes an address with a collection of bytes.
    explicit Address(const std::vector<uint8_t>& data) : TW::Base58Address<24>(data) {}

    /// Determines is a valid address.
    bool isValid() const;

    uint16_t chainID() const;

    uint8_t type() const;

    /// Returns a string representation of the address.
    std::string string() const;
};

static inline bool operator==(const Address& lhs, const Address& rhs) {
    return lhs.bytes == rhs.bytes;
}

} // namespace TW::NULS

/// Wrapper for C interface.
struct TWNULSAddress {
    TW::NULS::Address impl;
};