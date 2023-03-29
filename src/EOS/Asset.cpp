// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Asset.h"

#include <boost/algorithm/string/trim.hpp>
#include <boost/lexical_cast.hpp>
#include <stdexcept>

namespace TW::EOS {

static const int64_t Precision = 1000;
static const uint8_t MaxDecimals = 18;

Asset::Asset(int64_t amount, uint8_t decimals, const std::string& symbol) {
    if (decimals > MaxDecimals) {
        throw std::invalid_argument("Too many decimals!");
    }
    this->symbol |= decimals;

    if (symbol.empty() || symbol.size() > 7) {
        throw std::invalid_argument("Symbol size invalid!");
    }

    for (std::size_t i = 0; i < symbol.size(); i++) {
        uint64_t c = (unsigned char) symbol[i];
        if (c < 'A' || c > 'Z') {
            throw std::invalid_argument("Invalid symbol " + symbol + ".\n Symbol can only have upper case alphabets!");
        }

        this->symbol |= c << (8 * (i + 1));
    }

    this->amount = amount;
}

Asset Asset::fromString(std::string assetString) {
    using namespace std;

    boost::algorithm::trim(assetString);

    // Find space in order to split amount and symbol
    auto spacePosition = assetString.find(' ');
    if (spacePosition == string::npos) {
        throw std::invalid_argument("Asset's amount and symbol should be separated with space");
    }

    auto symbolString = boost::algorithm::trim_copy(assetString.substr(spacePosition + 1));
    auto amountString = assetString.substr(0, spacePosition);

    // Ensure that if decimal point is used (.), decimal fraction is specified
    auto dotPosition = amountString.find('.');
    if (dotPosition != string::npos && dotPosition == amountString.size() - 1) {
        throw std::invalid_argument("Missing decimal fraction after decimal point");
    }

    uint8_t decimals = 0;
    if (dotPosition != string::npos) {
        decimals = static_cast<uint8_t>(amountString.size() - dotPosition - 1);
    }

    auto precision = static_cast<int64_t>(pow(10, static_cast<double>(decimals)));

    // Parse amount
    int64_t intPart, fractPart = 0;
    if (dotPosition != string::npos) {
        intPart = boost::lexical_cast<int64_t>(amountString.data(), dotPosition);
        fractPart = boost::lexical_cast<int64_t>(amountString.data() + dotPosition + 1, decimals);
        if (amountString[0] == '-') {
            fractPart *= -1;
        }
    } else {
        intPart = boost::lexical_cast<int64_t>(amountString);
    }

    int64_t amount = intPart;

    // multiply and check overflow
    amount *= precision;
    if (amount / precision != intPart) {
        throw std::invalid_argument("Amount too large!");
    }

    // add and check overflow
    amount += fractPart;
    if (amountString[0] == '-') {
        if (amount > fractPart) {
            throw std::invalid_argument("Amount too large!");
        }
    } else {
        if (amount < fractPart) {
            throw std::invalid_argument("Amount too large!");
        }
    }

    return Asset(amount, decimals, symbolString);
}

void Asset::serialize(Data& os) const noexcept {
    encode64LE(amount, os);
    encode64LE(symbol, os);
}

std::string Asset::string() const {
    static const int maxBufferSize = 30;
    char buffer[maxBufferSize];

    auto decimals = getDecimals();

    int charsWritten = snprintf(buffer, maxBufferSize, "%.*f %s",
                                decimals,
                                static_cast<double>(amount) / Precision,
                                getSymbol().c_str());

    if (charsWritten < 0 || charsWritten > maxBufferSize) {
        throw std::runtime_error("Failed to create string representation of asset!");
    }

    return std::string(buffer, charsWritten);
}

std::string Asset::getSymbol() const noexcept {
    uint64_t temp = symbol >> 8;
    std::string str;

    while (temp > 0) {
        str += temp & 0xFF;
        temp >>= 8;
    }

    return str;
}

} // namespace TW::EOS
