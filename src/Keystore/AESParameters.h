// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"

#include <nlohmann/json.hpp>

namespace TW::Keystore {

enum AESSize: std::int32_t {
    Uninitialized = 0,
    A128 = 16,
    A192 = 24,
    A256 = 32,
};

// AES128/192/256 parameters.
struct AESParameters {
    std::int32_t mBlockSize;

    Data iv;

    /// Initializes `AESParameters` with a random `iv` for AES 128/192/256.
    AESParameters(AESSize blockSize = A128);

    /// Initializes `AESParameters` with a JSON object.
    AESParameters(const nlohmann::json& json, const std::string& cipher);

    /// Saves `this` as a JSON object.
    nlohmann::json json() const;
};

} // namespace TW::Keystore
