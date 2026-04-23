// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "FeeCalculator.h"

#include <algorithm>
#include <cmath>

using namespace TW;

namespace TW::Bitcoin {

constexpr double gDecredBytesPerInput{166};
constexpr double gDecredBytesPerOutput{38};
constexpr double gDecredBytesBase{12};

Amount LinearFeeCalculator::calculate(size_t inputs, size_t outputs,
                                       Amount byteFee) const noexcept {
    const auto txsize =
        static_cast<Amount>(std::ceil(bytesPerInput * static_cast<double>(inputs) +
                                       bytesPerOutput * static_cast<double>(outputs) + bytesBase));
    return txsize * byteFee;
}

Amount LinearFeeCalculator::calculateSingleInput(Amount byteFee) const noexcept {
    return static_cast<Amount>(std::ceil(bytesPerInput)) * byteFee; // std::ceil(101.25) = 102
}

class DecredFeeCalculator : public LinearFeeCalculator {
private:
    bool disableDustFilter = false;

public:
    constexpr DecredFeeCalculator(bool disableFilter = false) noexcept
        : LinearFeeCalculator(gDecredBytesPerInput, gDecredBytesPerOutput, gDecredBytesBase)
        , disableDustFilter(disableFilter) {}

    Amount calculateSingleInput(Amount byteFee) const noexcept override {
        if (disableDustFilter) { 
            return 0; 
        }
        return LinearFeeCalculator::calculateSingleInput(byteFee);
    }
};

static constexpr DefaultFeeCalculator defaultFeeCalculator{};
static constexpr DefaultFeeCalculator defaultFeeCalculatorNoDustFilter(true);
static constexpr DecredFeeCalculator decredFeeCalculator{};
static constexpr DecredFeeCalculator decredFeeCalculatorNoDustFilter(true);
static constexpr SegwitFeeCalculator segwitFeeCalculator{};
static constexpr SegwitFeeCalculator segwitFeeCalculatorNoDustFilter(true);
static constexpr Zip0317FeeCalculator zip0317FeeCalculator{};

const FeeCalculator& getFeeCalculator(TWCoinType coinType, bool disableFilter, bool zip0317) noexcept {
    switch (coinType) {
    case TWCoinTypeDecred:
        if (disableFilter) {
            return decredFeeCalculatorNoDustFilter;
        }
        return decredFeeCalculator;

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

    case TWCoinTypeZcash:
    case TWCoinTypeKomodo:
    case TWCoinTypeZelcash:
        if (zip0317) {
            return zip0317FeeCalculator;
        }
        return defaultFeeCalculator;

    default:
        if (disableFilter) {
            return defaultFeeCalculatorNoDustFilter;
        }
        return defaultFeeCalculator;
    }
}

// https://github.com/Zondax/ledger-zcash-tools/blob/5ecf1c04c69d2454b73aa7acea4eadda563dfeff/ledger-zcash-app-builder/src/txbuilder.rs#L342-L363
Amount Zip0317FeeCalculator::calculate(size_t inputs, size_t outputs, [[maybe_unused]] Amount byteFee) const noexcept {
    const auto logicalActions = std::max(inputs, outputs);
    const auto actions = std::max(gGraceActions, logicalActions);
    return gMarginalFee * static_cast<Amount>(actions);
}

} // namespace TW::Bitcoin
