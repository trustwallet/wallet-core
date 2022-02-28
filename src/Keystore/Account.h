// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../DerivationPath.h"

#include <nlohmann/json.hpp>
#include <string>

namespace TW::Keystore {

/// Account for a particular coin within a wallet.
class Account {
  public:
    /// Account public address
    std::string address;

    /// Account derivation path, only relevant for HD wallets.
    DerivationPath derivationPath;

    /// Extended public key.
    std::string extendedPublicKey;

    /// Coin this account is for.
    TWCoinType coin;

    Account() = default;
    Account(std::string address, TWCoinType coin, DerivationPath derivationPath, std::string extendedPublicKey = "")
        : address(std::move(address))
        , derivationPath(std::move(derivationPath))
        , extendedPublicKey(std::move(extendedPublicKey))
        , coin(coin) {}

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
