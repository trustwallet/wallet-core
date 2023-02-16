// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <array>
#include <string>
#include "Base58.h"

namespace TW::Solana {
    class Hash {
    public:
        static const size_t size = 32;
        /// Hash data
        std::array<uint8_t, size> bytes;

        Hash(const std::string& string) {
            const auto data = Base58::bitcoin.decode(string);
            std::copy(data.begin(), data.end(), this->bytes.begin());
        }

        std::string encoded() const { return Base58::bitcoin.encode(bytes); }
    };
}
