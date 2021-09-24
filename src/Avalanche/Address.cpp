// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../Bech32.h"

#include "Address.h"

#include <TrustWalletCore/TWHRP.h>

using namespace TW::Avalanche;

const std::string Address::hrp = HRP_AVALANCHEX;
const size_t Address::hashLen = 20;

std::tuple<bool, std::string, std::string> Address::isValidParse(const std::string& string) {
    // split into before and after -
    auto hyphenPos = string.find("-");
    if (hyphenPos == std::string::npos) {
        return std::make_tuple(false, std::string(), std::string());
    }
    if (hyphenPos == 0 || hyphenPos == string.length() - 1) {
        // empty before or after
        return std::make_tuple(false, std::string(), std::string());
    }
    auto chainID = string.substr(hyphenPos - 1, 1);
    if ((chainID != "X") && (chainID != "P")) {
        // implementation is currently X-chain only, but a passed address starting with P- follows
        // the same rules
        return std::make_tuple(false, chainID, std::string());
    }
    auto afterHyphen = string.substr(hyphenPos + 1);
    bool bechValid = Bech32Address::isValid(afterHyphen, hrp);
    return std::make_tuple(bechValid, chainID, afterHyphen);
}

Address::Address(const std::string& string) : Bech32Address(hrp) {
    auto valid = isValidParse(string);
    if (!std::get<0>(valid)) {
        throw std::invalid_argument("Invalid address string");
    }
    Bech32Address::decode(std::get<2>(valid), *this, hrp, true);
}

std::string Address::string() const {
    // for now, we only support x-chain addresses
    return "X-" + Bech32Address::string();
}
