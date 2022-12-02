// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "PublicKey.h"

#include "Everscale/Address.h"

#include <string>

namespace TW::TheOpenNetwork {

enum AddressTag : uint8_t {
    BOUNCEABLE = 0x11,
    NON_BOUNCEABLE = 0x51,
    TEST_ONLY = 0x80,
};

class Address : public Everscale::Address {
public:
    /// User-friendly address lens
    static const size_t b64UserFriendlyAddressLen = 48;
    static const size_t userFriendlyAddressLen = 36;

    // Determines whether the address is user-friendly
    bool isUserFriendly = false;

    /// Determines whether the address is bounceable
    bool isBounceable = false;

    /// Determines whether the address is for tests only
    bool isTestOnly = false;

    /// Determines whether a string makes a valid address.
    [[nodiscard]] static bool isValid(const std::string& string) noexcept;

    /// Initializes an address with a string representation.
    explicit Address(const std::string& string);

    /// Initializes an address with a public key.
    explicit Address(const PublicKey& publicKey, int8_t workchainId);

    /// Initializes an address with its parts
    explicit Address(int8_t workchainId, std::array<byte, size> hash)
        : Everscale::Address(workchainId, hash) { }

    /// Returns a string representation of the address.
    [[nodiscard]] std::string string() const override;
    [[nodiscard]] std::string string(bool userFriendly, bool bounceable = true, bool testOnly = false) const;

private:
    static Address createFromWallet(const PublicKey& publicKey, int8_t workchainId);
    static Data decodeUserFriendlyAddress(const std::string& string);
};

} // namespace TW::TheOpenNetwork
