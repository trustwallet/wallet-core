// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <TrustWalletCore/TWCoinType.h>
#include "CoinEntry.h"

#include <map>
#include <set>
#include <vector>

namespace TW {

// Collection of coin dispatchers
class CoinDispatchMap {
public: // methods
    // initialize with the list of dispatcher instances
    CoinDispatchMap(const std::vector<CoinEntry*>& dispatchers);
    CoinEntry* coinDispatcher(TWCoinType coinType) const;
    const std::set<TWCoinType>& getCoinTypes() { return coinTypes; }

private: // fields
    // Map with coin entry dispatchers, key is coin type
    std::map<TWCoinType, CoinEntry*> dispatchMap;
    // List of supported coin types
    std::set<TWCoinType> coinTypes;
};

} // namespace TW
