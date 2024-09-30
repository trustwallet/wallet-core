// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Data.h"
#include "PublicKey.h"

#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWData.h>
#include <CoinEntry.h>
#include <string>

namespace TW {

class AnyAddress {
public:
    std::string address;

    enum TWCoinType coin;

    // Create address from string address and optional prefix; also normalizes the address.
    static AnyAddress* createAddress(const std::string& address, enum TWCoinType coin, const PrefixVariant& prefix = std::monostate());
    // Create address from private key, with optional non-standard derivation and prefix
    static AnyAddress* createAddress(const PublicKey& publicKey, enum TWCoinType coin, TWDerivation derivation = TWDerivationDefault, const PrefixVariant& prefix = std::monostate());

    Data getData() const;
};

inline bool operator==(const AnyAddress& lhs, const AnyAddress& rhs) {
    return lhs.address == rhs.address && lhs.coin == rhs.coin;
}

} // namespace TW

/// Wrapper for C interface.
struct TWAnyAddress {
    // Pointer to the underlying implementation
    TW::AnyAddress* impl;
};
