// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "../DerivationPath.h"
#include <TrustWalletCore/TWDerivation.h>

#include <nlohmann/json.hpp>
#include <string>

namespace TW::Keystore {

/// Account for a particular coin within a wallet.
class Account {
  public:
    /// Coin this account is for
    TWCoinType coin;

    /// Account public address
    std::string address;

    /// Account derivation. May be missing or unreliable in Json stored format.
    TWDerivation derivation = TWDerivationDefault;

    /// Account derivation path, only relevant for HD wallets; info only.
    DerivationPath derivationPath;

    /// Account public key in hex format.
    std::string publicKey;

    /// Extended public key, info only.
    std::string extendedPublicKey;

    Account() = default;
    Account(std::string address, TWCoinType coin, TWDerivation derivation, DerivationPath derivationPath, std::string publicKey, std::string extendedPublicKey)
        : coin(coin)
        , address(std::move(address))
        , derivation(derivation)
        , derivationPath(std::move(derivationPath))
        , publicKey(std::move(publicKey))
        , extendedPublicKey(std::move(extendedPublicKey)) {}

    /// Initializes `Account` with a JSON object.
    Account(const nlohmann::json& json);

    /// Saves `this` as a JSON object.
    nlohmann::json json() const;
};

} // namespace TW::Keystore

/// Wrapper for C interface.
struct TWAccount {
    TW::Keystore::Account impl;
};
