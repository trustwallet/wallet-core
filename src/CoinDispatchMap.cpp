// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "CoinDispatchMap.h"

#include <cassert>

using namespace TW;
using namespace std;

CoinDispatchMap::CoinDispatchMap(const std::vector<CoinEntry*>& dispatchers) {
    for (auto d : dispatchers) {
        assert(d != nullptr);
        auto coins = d->coinTypes();
        for (auto c : coins) {
            assert(dispatchMap.find(c) == dispatchMap.end()); // each coin must appear only once
            dispatchMap[c] = d;
            if (coinTypes.emplace(c).second != true) {
                // each coin must appear only once
                abort();
            };
        }
    }
}

CoinEntry* CoinDispatchMap::coinDispatcher(TWCoinType coinType) const {
    // Coin must be present, and not null.  Otherwise that is a fatal code configuration error.
    auto found = dispatchMap.find(coinType);
    assert(found != dispatchMap.end()); // coin must be present
    assert(found->second != nullptr);
    return found->second;
}
