// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../CoinEntry.h"
#include "Ethereum/Entry.h"

namespace TW::VeChain {

/// Entry point for VeChain.
/// Note: do not put the implementation here (no matter how simple), to avoid having coin-specific includes in this file
class Entry: public Ethereum::Entry {
public:
    virtual void sign(TWCoinType coin, const Data& dataIn, Data& dataOut) const;
};

} // namespace TW::VeChain
