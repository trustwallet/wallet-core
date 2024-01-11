// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "../CoinEntry.h"

namespace TW::NEO {

/// NEO entry dispatcher.
/// Note: do not put the implementation here (no matter how simple), to avoid having coin-specific includes in this file
class Entry final: public CoinEntry {
public:
    bool validateAddress(TWCoinType coin, const std::string& address, const PrefixVariant& addressPrefix) const override;
    std::string deriveAddress(TWCoinType coin, const PublicKey& publicKey, TWDerivation derivation, const PrefixVariant& addressPrefix) const override;
    Data addressToData(TWCoinType coin, const std::string& address) const override;
    void sign(TWCoinType coin, const Data& dataIn, Data& dataOut) const override;
    void plan(TWCoinType coin, const Data& dataIn, Data& dataOut) const override;
    
    Data preImageHashes(TWCoinType coin, const Data& txInputData) const override;
    void compile(TWCoinType coin, const Data& txInputData, const std::vector<Data>& signatures, const std::vector<PublicKey>& publicKeys, Data& dataOut) const override;
};

} // namespace TW::NEO
