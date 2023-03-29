// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "AddressChecksum.h"

#include "../Hash.h"
#include "../HexCoding.h"
#include "../uint256.h"
#include <cctype>

namespace TW::Zilliqa {

/// \see https://github.com/Zilliqa/Zilliqa/blob/1c53b792c7ae44f7b77366536a7e2f73a3eade6a/src/libServer/AddressChecksum.h
std::string checksum(const Data& bytes) {
    const auto addressString = hex(bytes);
    const auto hash = hex(Hash::sha256(bytes));

    uint256_t temp_1 = 1;
    uint256_t v("0x" + hash);

    std::string string = "";
    for (auto i = 0ul; i < addressString.size(); i += 1) {
        const auto a = addressString[i];
        if (a >= '0' && a <= '9') {
            string.push_back(a);
        } else {
            if ((v & (temp_1 << 255 - 6 * i))) {
                string.push_back(static_cast<char>(toupper(a)));
            } else {
                string.push_back(static_cast<char>(tolower(a)));
            }
        }
    }

    return string;
}

} // namespace TW::Zilliqa
