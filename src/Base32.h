// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"

#include "rust/bindgen/WalletCoreRSBindgen.h"
#include <TrezorCrypto/base32.h>

#include <cassert>

namespace TW::Base32 {

/// Decode Base32 string, return bytes as Data
/// alphabet: Optional alphabet, if missing, default ALPHABET_RFC4648
inline bool decode(const std::string& encoded_in, Data& decoded_out, const char* alphabet_in = nullptr) {
    size_t inLen = encoded_in.size();
    // obtain output length first
    size_t outLen = base32_decoded_length(inLen);
    uint8_t buf[outLen];
    if (alphabet_in == nullptr) {
        alphabet_in = BASE32_ALPHABET_RFC4648;
    }
    // perform the base32 decode
    uint8_t* retval = base32_decode(encoded_in.data(), inLen, buf, outLen, alphabet_in);
    if (retval == nullptr) {
        return false;
    }
    decoded_out.assign(buf, buf + outLen);
    return true;
}


/// Encode bytes in Data to Base32 string
/// alphabet: Optional alphabet, if missing, default ALPHABET_RFC4648
inline std::string encode(const Data& val, const char* alphabet = nullptr, bool padding = false) {
    auto* encoded = encode_base32(val.data(), val.size(), alphabet, padding);
    std::string encoded_str(encoded);
    free_string(encoded);
    return encoded_str;
}

} // namespace TW::Base32
