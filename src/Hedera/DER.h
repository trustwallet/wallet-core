// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <string>

namespace TW::Hedera {
    // Hedera DER prefix: https://github.com/hashgraph/hedera-sdk-js/blob/e0cd39c84ab189d59a6bcedcf16e4102d7bb8beb/packages/cryptography/src/Ed25519PrivateKey.js#L8
    inline constexpr const char* const gHederaDerPrefixPrivate = "302e020100300506032b657004220420";
    // Hedera DER prefix: https://github.com/hashgraph/hedera-sdk-js/blob/e0cd39c84ab189d59a6bcedcf16e4102d7bb8beb/packages/cryptography/src/Ed25519PublicKey.js#L7
    inline constexpr const char* const gHederaDerPrefixPublic = "302a300506032b6570032100";

    bool hasDerPrefix(const std::string& input) noexcept;
}
