// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "PublicKey.h"
#include "Move/Address.h"

#include <string>

namespace TW::Sui {

class Address : public Move::Address<Address, 32> {
public:
    using SuiAddress = Move::Address<Address, 32>;
    using SuiAddress::size;
    using SuiAddress::bytes;

    /// Initializes an Sui address with a string representation.
    explicit Address(const std::string& string);

    /// Initializes an Sui address with a public key.
    explicit Address(const PublicKey& publicKey);

    /// Constructor that allow factory programming;
    Address() noexcept = default;

    Data getDigest(const PublicKey& publicKey);
};

constexpr inline bool operator==(const Address& lhs, const Address& rhs) noexcept {
    return lhs.bytes == rhs.bytes;
}

} // namespace TW::Sui
