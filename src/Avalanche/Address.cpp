// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../Bech32.h"

#include "Address.h"

#include <TrustWalletCore/TWHRP.h>

using namespace TW::Avalanche;

const std::string Address::hrp = HRP_AVALANCHE;
const size_t Address::hashLen = 20;

bool Address::isValid(const std::string& string) {
    // split into before and after - 
    auto hyphenPos = string.find("-");
    if (hyphenPos == std::string::npos) {
        return false;
    }
    auto chainID = string.substr(hyphenPos - 1, 1);
    // compare before-hyphen with 'X' stringcompare (make it smarter later)
    if (chainID != "X") {
        // implementation is currently X-chain only
        return false;
    }
    auto afterHyphen = string.substr(hyphenPos + 1);
   return Bech32Address::isValid(afterHyphen, hrp);
}

void Address::extractKeyHashFromString (const std::string& string) {
    if (!isValid(string)) {
        throw std::invalid_argument("Invalid address string");
    }
    auto hyphenPos = string.find("-");
    auto dec = Bech32::decode(string.substr(hyphenPos + 1));
    Data conv;
    if (!Bech32::convertBits<5, 8, true>(conv, dec.second)) {
        throw std::invalid_argument("address string caused bech32 converstion failure");
    }
    setKey(conv);
}

std::string Address::string() const {
    return "X-" + Bech32Address::string();
}
