// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../CoinEntry.h"

namespace TW::IOST {

/// Entry point for implementation of IOST coin.
/// Note: do not put the implementation here (no matter how simple), to avoid having coin-specific
/// includes in this file.
class Entry final : public CoinEntry {
  public:
    bool validateAddress(TWCoinType coin, const std::string& address, const PrefixVariant& addressPrefixp) const override;
    std::string deriveAddress(TWCoinType coin, const PublicKey& publicKey, TWDerivation derivation, const PrefixVariant& addressPrefix) const override;
    Data addressToData(TWCoinType coin, const std::string& address) const override;
    void sign(TWCoinType coin, const Data& dataIn, Data& dataOut) const override;

    Data preImageHashes(TWCoinType coin, const Data& txInputData) const override;
    void compile(TWCoinType coin, const Data& txInputData,
                         const std::vector<Data>& signatures,
                         const std::vector<PublicKey>& publicKeys, Data& dataOut) const override;
};

} // namespace TW::IOST
