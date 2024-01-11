// Copyright © 2017 Pieter Wuille
// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Bech32.h"
#include "Data.h"

#include <array>

// Bech32 address encoding
// Bech32M variant also supported (BIP350)
// Max length of 90 constraint is extended here to 120 for other usages


using namespace TW;

namespace TW::Bech32 {

namespace {

/** The Bech32 character set for encoding. */
const char* charset = "qpzry9x8gf2tvdw0s3jn54khce6mua7l";

/** The Bech32 character set for decoding. */
constexpr std::array<int8_t, 128> charset_rev = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, 15, -1, 10, 17, 21, 20, 26, 30, 7, 5, -1, -1, -1, -1, -1, -1, -1, 29,
    -1, 24, 13, 25, 9, 8, 23, -1, 18, 22, 31, 27, 19, -1, 1, 0, 3, 16, 11, 28, 12, 14,
    6, 4, 2, -1, -1, -1, -1, -1, -1, 29, -1, 24, 13, 25, 9, 8, 23, -1, 18, 22, 31, 27,
    19, -1, 1, 0, 3, 16, 11, 28, 12, 14, 6, 4, 2, -1, -1, -1, -1, -1};

const uint32_t BECH32_XOR_CONST = 0x01;
const uint32_t BECH32M_XOR_CONST = 0x2bc830a3;

/** Find the polynomial with value coefficients mod the generator as 30-bit. */
uint32_t polymod(const Data& values) {
    uint32_t chk = 1;
    for (const auto& value : values) {
        uint8_t top = chk >> 25;
        chk = (chk & 0x1ffffff) << 5 ^ value ^ (-((top >> 0) & 1) & 0x3b6a57b2UL) ^
              (-((top >> 1) & 1) & 0x26508e6dUL) ^ (-((top >> 2) & 1) & 0x1ea119faUL) ^
              (-((top >> 3) & 1) & 0x3d4233ddUL) ^ (-((top >> 4) & 1) & 0x2a1462b3UL);
    }
    return chk;
}

/** Convert to lower case. */
unsigned char lc(unsigned char c) {
    return (c >= 'A' && c <= 'Z') ? (c - 'A') + 'a' : c;
}

/** Expand a HRP for use in checksum computation. */
Data expand_hrp(const std::string& hrp) {
    Data ret;
    ret.resize(hrp.size() * 2 + 1);
    for (size_t i = 0; i < hrp.size(); ++i) {
        unsigned char c = hrp[i];
        ret[i] = c >> 5;
        ret[i + hrp.size() + 1] = c & 0x1f;
    }
    ret[hrp.size()] = 0;
    return ret;
}

inline uint32_t xorConstant(ChecksumVariant variant) {
    if (variant == ChecksumVariant::Bech32) {
        return BECH32_XOR_CONST;
    }
    // Bech32M
    return BECH32M_XOR_CONST;
}

/** Verify a checksum. */
ChecksumVariant verify_checksum(const std::string& hrp, const Data& values) {
    Data enc = expand_hrp(hrp);
    append(enc, values);
    auto poly = polymod(enc);
    if (poly == BECH32_XOR_CONST) {
        return ChecksumVariant::Bech32;
    }
    if (poly == BECH32M_XOR_CONST) {
        return ChecksumVariant::Bech32M;
    }
    return ChecksumVariant::None;
}

/** Create a checksum. */
Data create_checksum(const std::string& hrp, const Data& values, ChecksumVariant variant) {
    Data enc = expand_hrp(hrp);
    append(enc, values);
    enc.resize(enc.size() + 6);
    auto xorConst = xorConstant(variant);
    uint32_t mod = polymod(enc) ^ xorConst;
    Data ret;
    ret.resize(6);
    for (size_t i = 0; i < 6; ++i) {
        ret[i] = (mod >> (5 * (5 - i))) & 31;
    }
    return ret;
}

} // namespace

/** Encode a Bech32 string. */
std::string encode(const std::string& hrp, const Data& values, ChecksumVariant variant) {
    Data checksum = create_checksum(hrp, values, variant);
    Data combined = values;
    append(combined, checksum);
    std::string ret = hrp + '1';
    ret.reserve(ret.size() + combined.size());
    for (const auto& value : combined) {
        ret += charset[value];
    }
    return ret;
}

/** Decode a Bech32 string. */
std::tuple<std::string, Data, ChecksumVariant> decode(const std::string& str) {
    if (str.length() > 120 || str.length() < 2) {
        // too long or too short
        return std::make_tuple(std::string(), Data(), None);
    }
    bool lower = false, upper = false;
    bool ok = true;
    for (size_t i = 0; ok && i < str.size(); ++i) {
        unsigned char c = str[i];
        if (c < 33 || c > 126) {
            ok = false;
        }
        if (c >= 'a' && c <= 'z') {
            lower = true;
        }
        if (c >= 'A' && c <= 'Z') {
            upper = true;
        }
    }
    if (lower && upper) {
        ok = false;
    }
    size_t pos = str.rfind('1');
    if (ok && pos != std::string::npos && pos >= 1 && pos + 7 <= str.size()) {
        Data values;
        values.resize(str.size() - 1 - pos);
        for (size_t i = 0; i < str.size() - 1 - pos; ++i) {
            unsigned char c = str[i + pos + 1];
            if (charset_rev[c] == -1) {
                ok = false;
            }
            values[i] = charset_rev[c];
        }
        if (ok) {
            std::string hrp;
            for (size_t i = 0; i < pos; ++i) {
                hrp += lc(str[i]);
            }
            auto variant = verify_checksum(hrp, values);
            if (variant != None) {
                return std::make_tuple(hrp, Data(values.begin(), values.end() - 6), variant);
            }
        }
    }
    return std::make_tuple(std::string(), Data(), None);
}

} // namespace TW::Bech32
