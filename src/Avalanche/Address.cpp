// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"

#include "../Bech32.h"
#include "../Hash.h"
#include "../HexCoding.h"

#include <iostream>

using namespace TW::Avalanche;

const std::string Address::hrp =  "avax"; //HRP_AVALANCHE;

bool Address::isValid(const std::string& string) {
    std::cout << "testing " << string << std::endl;
    // split into before and after - 
    auto hyphenPos = string.find("-");
    if (hyphenPos == std::string::npos) {
        std::cout << "hyphenpos" << std::endl;
        return false;
    }
    auto chainID = string.substr(hyphenPos - 1, 1);
    // compare before-hyphen with 'X' stringcompare (make it smarter later)
    if (chainID != "X") {
        std::cout << "chainID was " << chainID << std::endl;
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
    auto decoded = Bech32::decode(string.substr(hyphenPos + 1));
    setKey(decoded.second);
}

std::string Address::string() const {
    return "X-" + Bech32Address::string();
}