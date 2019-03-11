// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../DerivationPath.h"

#include <nlohmann/json.hpp>
#include <string>

namespace TW {
namespace Keystore {

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
    TWCoinType coin() const {
        return derivationPath.coin();
    }

    Account() = default;
    Account(std::string address, DerivationPath derivationPath, const std::string& extendedPublicKey = "")
        : address(address), derivationPath(derivationPath), extendedPublicKey(extendedPublicKey) {}

    /// Initializes `Account` with a JSON object.
    Account(const nlohmann::json& json);

    /// Saves `this` as a JSON object.
    nlohmann::json json() const;
};

}} // namespace

/// Wrapper for C interface.
struct TWAccount {
    TW::Keystore::Account impl;
};
