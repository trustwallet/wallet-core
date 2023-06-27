// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Ethereum/Entry.h"
#include "../CoinEntry.h"
#include "Ethereum/Entry.h"

namespace TW::Theta {

/// Entry point for Theta.
/// Note: do not put the implementation here (no matter how simple), to avoid having coin-specific includes in this file
class Entry final : public Ethereum::Entry {
public:
    void sign(TWCoinType coin, const Data& dataIn, Data& dataOut) const;
    Data preImageHashes(TWCoinType coin, const Data& txInputData) const;
    void compile(TWCoinType coin, const Data& txInputData, const std::vector<Data>& signatures, const std::vector<PublicKey>& publicKeys, Data& dataOut) const;
};

} // namespace TW::Theta
