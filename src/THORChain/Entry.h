// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../CoinEntry.h"
#include "../Cosmos/Entry.h"

namespace TW::THORChain {

/// Entry point for implementation of THORChain coin.
/// Note: do not put the implementation here (no matter how simple), to avoid having coin-specific includes in this file
class Entry final : public Cosmos::Entry {
public:
     void sign(TWCoinType coin, const Data& dataIn, Data& dataOut) const;
     std::string signJSON(TWCoinType coin, const std::string& json, const Data& key) const;
};

} // namespace TW::THORChain
