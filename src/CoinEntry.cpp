// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "CoinEntry.h"
#include "Coin.h"
#include "rust/bindgen/WalletCoreRSBindgen.h"
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

void signRust(const Data& dataIn, TWCoinType coin, Data& dataOut)  {
    auto* data = Rust::tw_data_create_with_bytes(dataIn.data(), dataIn.size());
    if (!data) {
        return;
    }
    auto *output = Rust::tw_any_signer_sign(data,static_cast<uint32_t>(coin));
    if (!output) {
        return;
    }
    auto* output_data = Rust::tw_data_bytes(output);
    auto output_size = Rust::tw_data_size(output);
    dataOut.assign(output_data, output_data + output_size);
}

} // namespace TW
