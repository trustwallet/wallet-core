// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <TrustWalletCore/TWHRP.h>

#include "Data.h"
#include "../PublicKey.h"

#include <optional>
#include <stdexcept>
#include <string>

namespace TW::Nervos {

enum HashType {
    Data0 = 0,
    Type1 = 1,
    Data1 = 2,
    Data2 = 4 // since the CKB2023 hardfork
};

/// Maps a hash type to its on-chain JSON/protobuf string.
inline const char* hashTypeString(HashType hashType) {
    switch (hashType) {
    case HashType::Data0:
        return "data";
    case HashType::Type1:
        return "type";
    case HashType::Data1:
        return "data1";
    case HashType::Data2:
        return "data2";
    }
    throw std::invalid_argument("Unknown hash type");
}

inline std::optional<HashType> hashTypeFromString(const std::string& string) {
    for (auto hashType : {HashType::Data0, HashType::Type1, HashType::Data1, HashType::Data2}) {
        if (string == hashTypeString(hashType)) {
            return hashType;
        }
    }
    return std::nullopt;
}

enum AddressType {
    FullVersion = 0,  // full version identifies the hash_type
    HashIdx = 1,      // short version for locks with popular codehash, deprecated
    DataCodeHash = 2, // full version with hash type 'Data', deprecated
    TypeCodeHash = 4, // full version with hash type 'Type', deprecated
};

class Address {
public:
    const char* _hrp;
    AddressType addressType;
    TW::byte codeHashIndex;
    Data codeHash;
    HashType hashType;
    Data args;

    /// Determines whether a string makes a valid address.
    [[nodiscard]] static bool isValid(const std::string& string) noexcept;
    [[nodiscard]] static bool isValid(const std::string& string, const char* hrp) noexcept;

    /// Initializes a Nervos address with a string representation.
    explicit Address(const std::string& string) : Address(string, HRP_NERVOS) {}
    explicit Address(const std::string& string, const char* hrp);

    /// Initializes a Nervos address with a public key.
    explicit Address(const PublicKey& publicKey) : Address(publicKey, HRP_NERVOS) {}
    explicit Address(const PublicKey& publicKey, const char* hrp);

    /// Returns a string representation of the address.
    std::string string() const;

    std::string hashTypeString() const;

private:
    Address() = default;

    // Decodes address from string
    bool decode(const std::string& string, const char* hrp) noexcept;
};

inline bool operator==(const Address& lhs, const Address& rhs) {
    return (lhs.codeHash == rhs.codeHash) && (lhs.hashType == rhs.hashType) &&
           (lhs.args == rhs.args);
}

} // namespace TW::Nervos

/// Wrapper for C interface.
struct TWNervosAddress {
    TW::Nervos::Address impl;
};
