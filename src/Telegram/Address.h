// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../PublicKey.h"

#include <string>

namespace TW::Telegram {

using WorkchainId_t = int32_t;

/// Workchain ID, currently supported: masterchain: -1, basic workchain: 0
class Workchain {
public:
    static const WorkchainId_t Workchain_Master = -1;
    static const WorkchainId_t Workchain_Basic = 0;

    static bool isValid(WorkchainId_t workchainId_in);
};

/// TON smart contract address, also account address
class Address {
public:
    WorkchainId_t workchainId;

    static const uint8_t AddressLength = 32;

    // Address: 256 bits (for chains -1 and 0)
    std::array<byte, AddressLength> addrBytes;

    /// Determines whether a string makes a valid address, in any format
    static bool isValid(const std::string& address);

    /*
    /// Initializes a TON address with a string representation, either raw or user friendly
    explicit Address(const std::string& address);

    /// Initializes a Nano address with a public key.
    explicit Address(const PublicKey& publicKey);

    /// Returns a string representation of the address, raw format
    std::string stringRaw() const;

    /// Returns a string representation of the address, user friendly format
    std::string string() const;
    */

private:
    static bool parseRawAddress(const std::string& addressStr_in, Address& addr_inout);

    static bool parseUserAddress(const std::string& addressStr_in, Address& addr_inout);
};

/*
inline bool operator==(const Address& lhs, const Address& rhs) {
    return lhs.bytes == rhs.bytes;
}
*/

} // namespace TW::Telegram

/*
/// Wrapper for C interface.
struct TWTelegramAddress {
    TW::Telegram::Address impl;
};
*/
