// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "FeeCalculator.h"

#include <algorithm>
#include <cmath>

using namespace TW;

namespace TW::Bitcoin {

constexpr double gDecredBytesPerInput{166};
constexpr double gDecredBytesPerOutput{38};
constexpr double gDecredBytesBase{12};

constexpr double gZcashMarginalFee{5000};
constexpr double gZcashGraceActions{2};

int64_t LinearFeeCalculator::calculate(int64_t inputs, int64_t outputs,
                                       int64_t byteFee) const noexcept {
    const auto txsize =
        static_cast<int64_t>(std::ceil(bytesPerInput * static_cast<double>(inputs) +
                                       bytesPerOutput * static_cast<double>(outputs) + bytesBase));
    return txsize * byteFee;
}

int64_t LinearFeeCalculator::calculateSingleInput(int64_t byteFee) const noexcept {
    return static_cast<int64_t>(std::ceil(bytesPerInput)) * byteFee; // std::ceil(101.25) = 102
}

class DecredFeeCalculator : public LinearFeeCalculator {
private:
    bool disableDustFilter = false;

public:
    constexpr DecredFeeCalculator(bool disableFilter = false) noexcept
        : LinearFeeCalculator(gDecredBytesPerInput, gDecredBytesPerOutput, gDecredBytesBase)
        , disableDustFilter(disableFilter) {}

    int64_t calculateSingleInput(int64_t byteFee) const noexcept override {
        if (disableDustFilter) { 
            return 0; 
        }
        return LinearFeeCalculator::calculateSingleInput(byteFee);
    }
};

/// Zcash transaction fee calculator
class ZcashFeeCalculator : public FeeCalculator {
private:
    bool disableDustFilter = false;

public:
    constexpr ZcashFeeCalculator(bool disableFilter = false) noexcept
        : disableDustFilter(disableFilter) {}

    [[nodiscard]] int64_t calculate(int64_t inputs, int64_t outputs, int64_t byteFee) const noexcept final {
        // see: https://zips.z.cash/zip-0317
        // Works from v5.5.0 => https://github.com/zcash/zcash/releases/tag/v5.5.0
        // In case of traffic, we use `byteFee` to do scale to improve fee,
        // most of cases, please set it to 1.
        return static_cast<int64_t>(gZcashMarginalFee * std::max(static_cast<int64_t>(gZcashGraceActions), std::max(inputs, outputs))) * byteFee;
    }

    [[nodiscard]] int64_t calculateSingleInput(int64_t byteFee) const noexcept override {
        if (disableDustFilter) { 
            return 0; 
        }
        return static_cast<int64_t>(std::ceil(gZcashMarginalFee * gZcashGraceActions));
    }
};

static constexpr DefaultFeeCalculator defaultFeeCalculator{};
static constexpr DefaultFeeCalculator defaultFeeCalculatorNoDustFilter(true);
static constexpr DecredFeeCalculator decredFeeCalculator{};
static constexpr DecredFeeCalculator decredFeeCalculatorNoDustFilter(true);
static constexpr SegwitFeeCalculator segwitFeeCalculator{};
static constexpr SegwitFeeCalculator segwitFeeCalculatorNoDustFilter(true);
static constexpr ZcashFeeCalculator zcashFeeCalculator{};
static constexpr ZcashFeeCalculator zcashFeeCalculatorNoDustFilter(true);

const FeeCalculator& getFeeCalculator(TWCoinType coinType, bool disableFilter) noexcept {
    switch (coinType) {
    case TWCoinTypeDecred:
        if (disableFilter) {
            return decredFeeCalculatorNoDustFilter;
        }
        return decredFeeCalculator;
    case TWCoinTypeZcash:
        if (disableFilter) {
            return zcashFeeCalculatorNoDustFilter;
        }
        return zcashFeeCalculator;

    case TWCoinTypeBitcoin:
    case TWCoinTypeBitcoinGold:
    case TWCoinTypeDigiByte:
    case TWCoinTypeLitecoin:
    case TWCoinTypeViacoin:
    case TWCoinTypeGroestlcoin:
    case TWCoinTypeSyscoin:
    case TWCoinTypeStratis:
        if (disableFilter) {
            return segwitFeeCalculatorNoDustFilter;
        }
        return segwitFeeCalculator;

    default:
        if (disableFilter) {
            return defaultFeeCalculatorNoDustFilter;
        }
        return defaultFeeCalculator;
    }
}

} // namespace TW::Bitcoin
