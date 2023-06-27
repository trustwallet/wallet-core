// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "PublicKey.h"

#include "Everscale/CommonTON/RawAddress.h"
#include <optional>

#include <string>

namespace TW::TheOpenNetwork {

enum AddressTag : uint8_t {
    BOUNCEABLE = 0x11,
    NON_BOUNCEABLE = 0x51,
    TEST_ONLY = 0x80,
};

using AddressData = CommonTON::AddressData;

class Address {
public:
    AddressData addressData;

    /// User-friendly address lens
    static const size_t b64UserFriendlyAddressLen = 48;
    static const size_t userFriendlyAddressLen = 36;

    /// Determines whether the address is user-friendly
    bool isUserFriendly = false;

    /// Determines whether the address is bounceable
    bool isBounceable = false;

    /// Determines whether the address is for tests only
    bool isTestOnly = false;

    /// Determines whether a string makes a valid address.
    [[nodiscard]] static bool isValid(const std::string& string) noexcept;

    /// Initializes an address with a string representation.
    explicit Address(const std::string& string, std::optional<bool> bounceable = std::nullopt);

    /// Initializes an address with its parts
    explicit Address(
        int8_t workchainId, std::array<byte, AddressData::size> hash,
        bool userFriendly = true, bool bounceable = true, bool testOnly = false
    ) : addressData(workchainId, hash),
        isUserFriendly(userFriendly),
        isBounceable(bounceable),
        isTestOnly(testOnly) {}

    /// Returns a string representation of the address.
    [[nodiscard]] std::string string() const;
    [[nodiscard]] std::string string(bool userFriendly, bool bounceable = true, bool testOnly = false) const;
};

} // namespace TW::TheOpenNetwork
