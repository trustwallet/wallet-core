// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "AddressChecksum.h"

#include "../HexCoding.h"
#include <cctype>

namespace TW::Ethereum {

std::string checksumed(const Address& address) {
    const auto addressString = hex(address.bytes);
    const auto hash = hex(Hash::keccak256(addressString));

    std::string string = "0x";
    for (auto i = 0ul; i < std::min(addressString.size(), hash.size()); i += 1) {
        const auto a = addressString[i];
        const auto h = hash[i];
        if (a >= '0' && a <= '9') {
            string.push_back(a);
        } else if ((h >= '8' && h <= '9') || (h >= 'a' && h <= 'f')) {
            string.push_back(static_cast<char>(toupper(a)));
        } else {
            string.push_back(static_cast<char>(tolower(a)));
        }
    }

    return string;
}

} // namespace TW::Ethereum
