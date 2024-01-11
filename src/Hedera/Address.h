// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Data.h"
#include "PublicKey.h"

#include <optional>
#include <string>

namespace TW::Hedera {

struct Alias {
    explicit Alias(std::optional<PublicKey> alias = std::nullopt) noexcept;
    std::string string() const noexcept;
    std::optional<PublicKey> mPubKey{std::nullopt};
};

class Address {
public:
    /// Determines whether a string makes a valid address.
    static bool isValid(const std::string& string);

    /// Initializes a Hedera address with a string representation.
    explicit Address(const std::string& string);

    /// Initializes a Hedera address with a public key.
    explicit Address(const PublicKey& publicKey);

    /// Initializes a Hedera address with a shard, realm, num and optional alias
    explicit Address(std::size_t shard, std::size_t realm, std::size_t num, std::optional<PublicKey> alias = std::nullopt);

    /// Returns a string representation of the address.
    std::string string() const;

    std::size_t shard() const { return mShard; }
    std::size_t realm() const { return mRealm; }
    std::size_t num() const { return mNum; }
    const Alias& alias() const {return mAlias;}

private:
    std::size_t mShard{0};
    std::size_t mRealm{0};
    std::size_t mNum;
    Alias mAlias;
};

} // namespace TW::Hedera
