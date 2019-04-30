#pragma once

#include <nlohmann/json.hpp>

#include "Serialization.h"

#include <memory>

namespace TW::Bravo {

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

    // Bravo specific constructor
    Asset(int64_t amount, bool isTestNet = false);

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

    // Bravo defaults
    static const uint8_t decimals = 3;
    static const int64_t precision = 1000;
    static const constexpr char * mainNetSymbol = "BRAVO";
    static const constexpr char * testNetSymbol = "TESTS";

    static const uint8_t maxDecimals = 18;

};
} // namespace