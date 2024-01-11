// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "../CoinEntry.h"

namespace TW::Hedera {

/// Entry point for implementation of Hedera coin.
/// Note: do not put the implementation here (no matter how simple), to avoid having coin-specific includes in this file
class Entry final : public CoinEntry {
public:
    virtual bool validateAddress(TWCoinType coin, const std::string& address, const PrefixVariant& addressPrefix) const;
    virtual std::string deriveAddress(TWCoinType coin, const PublicKey& publicKey, TWDerivation derivation, const PrefixVariant& addressPrefix) const;
    virtual void sign(TWCoinType coin, const Data& dataIn, Data& dataOut) const;
};

} // namespace TW::Hedera
