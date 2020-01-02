// Copyright © 2017 Pieter Wuille
// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <cstdint>
#include <string>
#include <vector>

namespace TW::Bech32 {

/// Encodes a Bech32 string.
///
/// \returns the encoded string, or an empty string in case of failure.
std::string encode(const std::string& hrp, const std::vector<uint8_t>& values);

/// Decodes a Bech32 string.
///
/// \returns a pair with the human-readable part and the data, or a pair or
/// empty collections on failure.
std::pair<std::string, std::vector<uint8_t>> decode(const std::string& str);

/// Converts from one power-of-2 number base to another.
template <int frombits, int tobits, bool pad>
inline bool convertBits(std::vector<uint8_t>& out, const std::vector<uint8_t>& in) {
    int acc = 0;
    int bits = 0;
    const int maxv = (1 << tobits) - 1;
    const int max_acc = (1 << (frombits + tobits - 1)) - 1;
    for (const auto& value : in) {
        acc = ((acc << frombits) | value) & max_acc;
        bits += frombits;
        while (bits >= tobits) {
            bits -= tobits;
            out.push_back((acc >> bits) & maxv);
        }
    }
    if (pad) {
        if (bits)
            out.push_back((acc << (tobits - bits)) & maxv);
    } else if (bits >= frombits || ((acc << (tobits - bits)) & maxv)) {
        return false;
    }
    return true;
}

} // namespace TW::Bech32
