// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "PublicKey.h"

#include <optional>
#include <string>

namespace TW::Hedera {

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
    std::optional<PublicKey> alias() const {return mAlias;}

private:
    std::size_t mShard{0};
    std::size_t mRealm{0};
    std::size_t mNum;
    std::optional<PublicKey> mAlias{std::nullopt};
};

} // namespace TW::Hedera
