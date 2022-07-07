// Copyright © 2017 Pieter Wuille
// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"

#include <TrustWalletCore/TWHRP.h>
#include <vector>

using namespace TW::Binance;

const std::string Address::hrp = HRP_BINANCE;
const std::string Address::hrpValidator = "bva";
const std::vector<std::string> validHrps = {Address::hrp, Address::hrpValidator, "bnbp", "bvap", "bca", "bcap"};

bool Address::isValid(const std::string& addr) {
    Address addrNotUsed;
    return decode(addr, addrNotUsed);
}

bool Address::decode(const std::string& addr, Address& obj_out) {
    for (const auto& hrp: validHrps) {
        if (Bech32Address::decode(addr, obj_out, hrp)) {
            return true;
        }
    }
    return false;
}

const std::string TAddress::hrp = HRP_TBINANCE;
const std::string TAddress::hrpValidator = "bva";

bool TAddress::isValid(const std::string& addr) {
    std::vector<std::string> hrps = {hrp, hrpValidator, "bnbp", "bvap", "bca", "bcap"};
    bool result = false;
    for (auto& hrp : hrps) {
        result = Bech32Address::isValid(addr, hrp);
        if (result) {
            break;
        }
    }
    return result;
}

