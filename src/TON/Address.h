// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../PublicKey.h"

#include <string>

namespace TW::TON {

using WorkchainId_t = int32_t;

/// Workchain ID, currently supported: masterchain: -1, basic workchain: 0
class Workchain {
  public:
    static constexpr WorkchainId_t MasterChainId = -1;
    static constexpr WorkchainId_t BasicChainId = 0;
    static constexpr WorkchainId_t InvalidChainId = 0x80000000;

    // The default workchain ID
    static WorkchainId_t defaultChain() { return BasicChainId; }
    static bool isValid(WorkchainId_t workchainId_in);
};

/// TON smart contract address, also account address
class Address {
  public:
    WorkchainId_t workchainId;

    static const uint8_t AddressLength = 32;

    // Address: 256 bits (for chains -1 and 0)
    TW::Data addrBytes;

    bool isBounceable{true};
    bool isTestOnly{false};

    /// Initializes a TON address with a string representation, either raw or user friendly
    explicit Address(const std::string& address);

    /// Initializes a TON address with a public key.  WorkchainId is optional, Basic chain by default.
    explicit Address(const PublicKey& publicKey, WorkchainId_t workchain = Workchain::defaultChain());

    /// Determines whether a string makes a valid address, in any format
    static bool isValid(const std::string& address, WorkchainId_t workchain = Workchain::defaultChain());

    /// Returns a string representation of the address (user friendly format)
    std::string string() const;

    /// Returns a string representation of the address, raw format
    std::string stringRaw() const;

  private:
    /// Empty constructor
    Address() = default;

    static bool parseAddress(const std::string& addressStr_in, Address& addr_inout);
    static bool parseRawAddress(const std::string& addressStr_in, Address& addr_inout);
    // Accepts user-friendly base64 format, also accepts Base64Url format
    static bool parseUserAddress(const std::string& addressStr_in, Address& addr_inout);
};

inline bool operator==(const Address& lhs, const Address& rhs) {
    return (lhs.workchainId == rhs.workchainId && lhs.addrBytes == rhs.addrBytes &&
            lhs.isBounceable == rhs.isBounceable && lhs.isTestOnly == rhs.isTestOnly);
}

} // namespace TW::TON
