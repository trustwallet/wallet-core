// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../BinaryCoding.h"
#include "Name.h"

#include <boost/algorithm/string/trim.hpp>
#include <stdexcept>

namespace TW::EOS {

Name::Name(const std::string& str) {
    if (str.size() > 13) {
        throw std::invalid_argument(str + ": size too long!");
    }

    std::size_t i = 0;
    while (i < std::min(size_t(12), str.size())) {
        value |= (toSymbol(str[i]) & 0x1f) << (64 - (5 * (i + 1)));
        i++;
    }

    if (i == 12)
        value |= (toSymbol(str[i]) & 0x0f);
}

uint64_t Name::toSymbol(char c) noexcept {
    if (c >= 'a' && c <= 'z')
        return c - 'a' + 6;

    if (c >= '1' && c <= '5')
        return c - '1' + 1;

    return 0;
}

std::string Name::string() const noexcept {
    static const char* charMap = ".12345abcdefghijklmnopqrstuvwxyz";

    std::string str(13, '.');

    uint64_t tmp = value;
    str[12] = charMap[tmp & 0x0f];
    tmp >>= 4;

    for (uint32_t i = 1; i <= 12; ++i) {
        char c = charMap[tmp & 0x1f];
        str[12 - i] = c;
        tmp >>= 5;
    }

    boost::algorithm::trim_right_if(str, [](char c) { return c == '.'; });
    return str;
}

void Name::serialize(Data& o) const noexcept {
    encode64LE(value, o);
}

} // namespace TW::EOS
