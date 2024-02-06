// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "CoinEntry.h"
#include "Coin.h"
#include "HexCoding.h"
#include "rust/Wrapper.h"
#include <variant>
#include <google/protobuf/util/json_util.h>

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
