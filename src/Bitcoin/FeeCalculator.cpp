// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "FeeCalculator.h"

#include <cmath>

using namespace TW;

namespace TW::Bitcoin {

int64_t LinearFeeCalculator::calculate(int64_t inputs, int64_t outputs, int64_t byteFee) const {
    const auto txsize = int64_t(std::ceil(bytesPerInput * (double)inputs + bytesPerOutput * (double)outputs + bytesBase));
    return txsize * byteFee;
}

int64_t LinearFeeCalculator::calculateSingleInput(int64_t byteFee) const {
    return int64_t(std::ceil(bytesPerInput)) * byteFee; // std::ceil(101.25) = 102
}

class DecredFeeCalculator : public LinearFeeCalculator {
private:
    bool disableDustFilter = false;

public:
    DecredFeeCalculator(bool disableFilter = false)
        :LinearFeeCalculator(166, 38, 12), disableDustFilter(disableFilter) {}

     virtual int64_t calculateSingleInput(int64_t byteFee) const override {
        if (disableDustFilter) { 
            return 0; 
        }
        return LinearFeeCalculator::calculateSingleInput(byteFee);
    }
};

DefaultFeeCalculator defaultFeeCalculator;
DefaultFeeCalculator defaultFeeCalculatorNoDustFilter(true);
DecredFeeCalculator decredFeeCalculator;
DecredFeeCalculator decredFeeCalculatorNoDustFilter(true);
SegwitFeeCalculator segwitFeeCalculator;
SegwitFeeCalculator segwitFeeCalculatorNoDustFilter(true);

FeeCalculator& getFeeCalculator(TWCoinType coinType, bool disableFilter) {
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
