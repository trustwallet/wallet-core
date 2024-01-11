// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Data.h"
#include "Hash.h"
#include "rust/bindgen/WalletCoreRSBindgen.h"
#include "rust/Wrapper.h"

#include <string>

namespace TW::Base58 {
    /// Decodes a base 58 string into `result`, returns `false` on failure.
    static inline Data decode(const std::string& string, Rust::Base58Alphabet alphabet = Rust::Base58Alphabet::Bitcoin)  {
        if (string.empty()) {
            return {};
        }
        Rust::CByteArrayResultWrapper res = Rust::decode_base58(string.c_str(), alphabet);
        return res.unwrap_or_default().data;
    }

    static inline Data decodeCheck(const std::string& string, Rust::Base58Alphabet alphabet = Rust::Base58Alphabet::Bitcoin, Hash::Hasher hasher = Hash::HasherSha256d) {
        auto result = decode(string, alphabet);
        if (result.size() < 4) {
            return {};
        }

        // re-calculate the checksum, ensure it matches the included 4-byte checksum
        auto hash = Hash::hash(hasher, result.data(), result.size() - 4);
        if (!std::equal(hash.begin(), hash.begin() + 4, result.end() - 4)) {
            return {};
        }

        return Data(result.begin(), result.end() - 4);
    }

    template <typename T>
    static inline std::string encode(const T& data, Rust::Base58Alphabet alphabet = Rust::Base58Alphabet::Bitcoin) {
        auto encoded = encode_base58(data.data(), data.size(), alphabet);
        std::string encoded_str(encoded);
        Rust::free_string(encoded);
        return encoded_str;
    }

    template <typename T>
    static inline std::string encodeCheck(const T& data, Rust::Base58Alphabet alphabet = Rust::Base58Alphabet::Bitcoin, Hash::Hasher hasher = Hash::HasherSha256d) {
        auto hash = Hash::hash(hasher, data);
        Data toBeEncoded(std::begin(data), std::end(data));
        toBeEncoded.insert(toBeEncoded.end(), hash.begin(), hash.begin() + 4);
        return encode(toBeEncoded, alphabet);
    }
}
