// Copyright © 2017-2023 Trust Wallet.
// Author: Clement Doumergue
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "BCS.h"
#include "Data.h"
#include "Move/Address.h"
#include "PublicKey.h"

#include <string>

namespace TW::Aptos {

class Address : public Move::Address<Address, 32, true> {
public:
    using AptosAddress = Move::Address<Address, 32, true>;
    using AptosAddress::size;
    using AptosAddress::bytes;

    /// Initializes an Aptos address with a string representation.
    explicit Address(const std::string& string);

    /// Initializes an Aptos address with a public key.
    explicit Address(const PublicKey& publicKey);

    /// Constructor that allow factory programming;
    Address() noexcept = default;

    Data getDigest(const PublicKey& publicKey);
};

constexpr inline bool operator==(const Address& lhs, const Address& rhs) noexcept {
    return lhs.bytes == rhs.bytes;
}

BCS::Serializer& operator<<(BCS::Serializer& stream, Address) noexcept;

} // namespace TW::Aptos
