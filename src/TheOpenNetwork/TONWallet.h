// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "PrivateKey.h"
#include "rust/Wrapper.h"

#include "TrustWalletCore/TWCoinType.h"
#include "TrustWalletCore/TWDerivation.h"

namespace TW::TheOpenNetwork {

class TONWallet;

using TONWalletPtr = std::shared_ptr<Rust::TWTONWallet>;
using MaybePassphrase = std::optional<std::string>;
using MaybeTONWallet = std::optional<TONWallet>;

class TONWallet {
public:
    static bool isValidMnemonic(const std::string& mnemonic, const MaybePassphrase& passphrase);

    static MaybeTONWallet createWithMnemonic(const std::string& mnemonic, const MaybePassphrase& passphrase);

    /// Returns the private key with the given coin and derivation.
    /// \throws std exception if `coin` or `derivation` aren't `TWCoinTypeTON` and `TWDerivationDefault` correspondingly.
    PrivateKey getKey(TWCoinType coin = TWCoinTypeTON, TWDerivation derivation = TWDerivationDefault) const;

    /// Derives the address for the given coin and derivation.
    /// \throws std exception if `coin` or `derivation` aren't `TWCoinTypeTON` and `TWDerivationDefault` correspondingly.
    std::string deriveAddress(TWCoinType coin = TWCoinTypeTON, TWDerivation derivation = TWDerivationDefault) const;

private:
    explicit TONWallet(TONWalletPtr ptr): impl(std::move(ptr)) {
    }

    TONWalletPtr impl;
};

} // namespace TW::TheOpenNetwork

/// Wrapper for C interface.
struct TWTONWallet {
    TW::TheOpenNetwork::TONWallet impl;
};
