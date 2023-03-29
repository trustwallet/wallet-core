// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <nlohmann/json.hpp>

#include "Serialization.h"

#include <memory>

namespace TW::EOS {

// An asset class that can be used by Bravo, EOS, steem, et. al.
class Asset {
public:
    int64_t amount;
    uint64_t symbol = 0;

    // Generic constructor
    Asset(int64_t amount, uint8_t decimals, const std::string& symbol);

    // Parses a string in the format "5.001 XYZ"
    // and extracts the amount, decimals and symbol from such string.
    // e.g.: "5.001 XYZ" => (amount = 5001, decimals = 3, symbol = "XYZ")
    // Returns: an Asset object initialized with the values in the string.
    static Asset fromString(std::string assetString);

    std::string getSymbol() const noexcept;
    inline uint8_t getDecimals() const noexcept { return symbol & 0xFF; }
    inline uint64_t getPrecision() const noexcept { 
        int i = getDecimals();
        uint64_t p = 1;
        while(i--) p *= 10;
        return p;
    }

    void serialize(Data& os) const noexcept;
    std::string string() const;
};
} // TW::EOS namespace
