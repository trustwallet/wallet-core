// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "CoinEntry.h"
#include "Coin.h"
#include <variant>

namespace TW {

const char* getFromPrefixHrpOrDefault(const PrefixVariant &prefix, TWCoinType coin) {
    if (std::holds_alternative<Bech32Prefix>(prefix)) {
        const char* fromPrefix = std::get<Bech32Prefix>(prefix);
        if (fromPrefix != nullptr && *fromPrefix != 0) {
            return fromPrefix;
        }
    }
    // Prefix contains no hrp or empty, return coin-default
    return stringForHRP(TW::hrp(coin));
}

byte getFromPrefixPkhOrDefault(const PrefixVariant &prefix, TWCoinType coin) {
    if (std::holds_alternative<Base58Prefix>(prefix)) {
        return std::get<Base58Prefix>(prefix).p2pkh;
    }
    // Prefix contains no base58 prefixes, return coin-default
    return TW::p2pkhPrefix(coin);
}

} // namespace TW
