// Copyright © 2017 Pieter Wuille
// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Address.h"
#include "Coin.h"

#include <TrustWalletCore/TWHRP.h>
#include <vector>

namespace TW::Binance {

const std::string Address::hrpValidator = "bva";

Address::Address(TWCoinType coin):
    Bech32Address(stringForHRP(TW::hrp(coin))) {
}

Address::Address(const Data& keyHash, TWCoinType coin):
    Bech32Address(stringForHRP(TW::hrp(coin)), keyHash) {
}

Address::Address(const PublicKey& publicKey, TWCoinType coin):
    Bech32Address(stringForHRP(TW::hrp(coin)), Hash::HasherSha256ripemd, publicKey) {
}

bool Address::isValid(TWCoinType coin, const std::string& addr) {
    const auto* const hrp = stringForHRP(TW::hrp(coin));
    Address addrNotUsed(hrp);
    return decode(addr, addrNotUsed);
}

bool Address::isValid(const std::string& addr, const std::string& hrp) {
    Address addrNotUsed(hrp);
    return decode(addr, addrNotUsed);
}

bool Address::decode(const std::string& addr, Address& obj_out) {
    std::vector<std::string> validHrps = {obj_out.getHrp(), Address::hrpValidator, "bnbp", "bvap", "bca", "bcap"};
    for (const auto& hrp: validHrps) {
        if (Bech32Address::decode(addr, obj_out, hrp)) {
            return true;
        }
    }
    return false;
}

} // namespace TW::Binance
