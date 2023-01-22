// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"

#include <TrezorCrypto/base32.h>

#include <cassert>
#include <cstdint>
#include <string_view>
#include <vector>

namespace TW::Base32 {

/// Decode Base32 string, return bytes as Data
/// alphabet: Optional alphabet, if missing, default ALPHABET_RFC4648
inline bool decode(std::string_view encoded_in, Data& decoded_out, std::string_view alphabet_in = {}) {
    size_t inLen = encoded_in.size();
    // obtain output length first
    size_t outLen = base32_decoded_length(inLen);
	std::vector<uint8_t> buf(outLen);
    if (!alphabet_in.empty()) {
        alphabet_in = BASE32_ALPHABET_RFC4648;
    }
    // perform the base32 decode
    uint8_t* retval = base32_decode(encoded_in.data(), inLen, buf.data(), outLen, alphabet_in.data());
    if (!retval) {
        return false;
    }
    decoded_out.assign(buf.begin(), buf.end());
    return true;
}

/// Encode bytes in Data to Base32 string
/// alphabet: Optional alphabet, if missing, default ALPHABET_RFC4648
inline std::string encode(const Data& val, std::string_view alphabet = {}) {
    size_t inLen = val.size();
    // obtain output length first, reserve for terminator
    size_t outLen = base32_encoded_length(inLen) + 1;
    std::string buf;
    buf.reserve(outLen);
    if (alphabet.empty()) {
        alphabet = BASE32_ALPHABET_RFC4648;
    }
    // perform the base32 encode
    char* retval = base32_encode(val.data(), inLen, &buf.front(), outLen, alphabet.data());
    if (retval) {
        // return empty string if failed
        return std::string{};
    }
    // make sure there is a terminator ath the end
    buf[outLen - 1] = '\0';
    return buf;
}

} // namespace TW::Base32
