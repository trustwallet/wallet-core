// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TexAddress.h"

namespace TW::Zcash {
bool TexAddress::isValid(const std::string& addr) {
    return isValid(addr, "");
}

bool TexAddress::isValid(const std::string& addr, const std::string& hrp) {
    auto dec = Bech32::decode(addr);
    // check hrp prefix (if given)
    if (hrp.length() > 0 && std::get<0>(dec).compare(0, hrp.length(), hrp) != 0) {
        return false;
    }
    if (std::get<1>(dec).empty()) {
        return false;
    }
    if (std::get<2>(dec) != Bech32::Bech32M) {
        return false;
    }

    Data conv;
    auto success = Bech32::convertBits<5, 8, false>(conv, std::get<1>(dec));
    if (!success || conv.size() != length) {
        return false;
    }

    return true;
}

TexAddress::TexAddress(const std::string& addr) {
    auto dec = Bech32::decode(addr);
    if (std::get<1>(dec).empty()) {
        throw std::invalid_argument("Tex address has invalid payload");
    }
    if (std::get<2>(dec) != Bech32::Bech32M) {
        throw std::invalid_argument("Error decoding Tex address as bech32m");
    }

    Data conv;
    auto success = Bech32::convertBits<5, 8, false>(conv, std::get<1>(dec));
    if (!success || conv.size() != length) {
        throw std::invalid_argument("Error decoding Tex address payload as bech32m");
    }

    hrp = std::get<0>(dec);
    keyHash = conv;
}


} // namespace TW::Zcash
