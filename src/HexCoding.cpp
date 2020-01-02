// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"

#include <tuple>

std::tuple<uint8_t, bool> TW::value(uint8_t c) {
    if (c >= '0' && c <= '9')
        return std::make_tuple(c - '0', true);
    if (c >= 'a' && c <= 'z')
        return std::make_tuple(c - 'a' + 10, true);
    if (c >= 'A' && c <= 'Z')
        return std::make_tuple(c - 'A' + 10, true);

    // Invalid digit
    return std::make_tuple(0, false);
}
