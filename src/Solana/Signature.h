// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <cstdint>
#include <array>

#include "Base58.h"

namespace TW::Solana {
class Signature {
public:
    static const size_t size = 64;
    /// Signature data
    std::array<uint8_t, size> bytes;

    Signature(const std::string& string) {
        const auto data = Base58::decode(string);
        std::copy(data.begin(), data.end(), this->bytes.begin());
    }
    Signature(const std::array<uint8_t, 64>& bytes) { this->bytes = bytes; }
    Signature(const Data& bytes) { std::copy(bytes.begin(), bytes.end(), this->bytes.begin()); }

    bool operator==(const Signature& v) const;
};
}
