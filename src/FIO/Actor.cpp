// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Actor.h"

#include <regex>

using namespace TW::FIO;
using namespace std;

static const auto pattern = regex(R"(\b([a-z1-5]{3,})[.@]?\b)");
string Actor::actor(const Address& addr)
{
    uint64_t shortenedKey = shortenKey(addr.bytes);
    string name13 = name(shortenedKey);
    // trim to 12 chracters
    return name13.substr(0, 12);
}

bool Actor::validate(const std::string& addr) {
    smatch match;
    return regex_search(addr, match, pattern);
}

uint64_t Actor::shortenKey(const array<byte, Address::size>& addrKey)
{
    uint64_t res = 0;
    int i = 1; // Ignore key head
    int len = 0;
    while (len <= 12) {
        assert(i < 33); // Means the key has > 20 bytes with trailing zeroes...
        
        auto trimmed_char = uint64_t(addrKey[i] & (len == 12 ? 0x0f : 0x1f));
        if (trimmed_char == 0) { i++; continue; }  // Skip a zero and move to next

        auto shuffle = len == 12 ? 0 : 5 * (12 - len) - 1;
        res |= trimmed_char << shuffle;

        len++; i++;
    }
    return res;
}

string Actor::name(uint64_t shortKey) {
    static const char* charmap = ".12345abcdefghijklmnopqrstuvwxyz";

    string str(13,'.'); //We are forcing the string to be 13 characters

    uint64_t tmp = shortKey;
    for(uint32_t i = 0; i <= 12; i++ ) {
        char c = charmap[tmp & (i == 0 ? 0x0f : 0x1f)];
        str[12 - i] = c;
        tmp >>= (i == 0 ? 4 : 5);
    }

    return str;
}
