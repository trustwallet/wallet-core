// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <exception>
#include "ImmutableX/Constants.h"
#include "uint256.h"

namespace TW::ImmutableX {
    class PrivateKey {
    public:
        int256_t mNumber;

        explicit PrivateKey(int256_t number) : mNumber(number) {
            if (number > internal::gStarkCurveN || number < 1) {
                throw std::runtime_error("Invalid private key");
            }
        }

        explicit PrivateKey(const std::string& hex) {
            uint256_t out;
            std::stringstream ss;
            ss << std::hex << hex;
            ss >> out;
            *this = PrivateKey(out);
        }
    };
}
