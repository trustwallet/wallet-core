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
public:
    DecredFeeCalculator(): LinearFeeCalculator(166, 38, 12) {}
};

DefaultFeeCalculator defaultFeeCalculator;
DecredFeeCalculator decredFeeCalculator;
SegwitFeeCalculator segwitFeeCalculator;

FeeCalculator& getFeeCalculator(TWCoinType coinType) {
    switch (coinType) {
    case TWCoinTypeDecred:
        return decredFeeCalculator;

    case TWCoinTypeBitcoin:
    case TWCoinTypeBitcoinGold:
    case TWCoinTypeDigiByte:
    case TWCoinTypeLitecoin:
    case TWCoinTypeViacoin:
    case TWCoinTypeGroestlcoin:
        return segwitFeeCalculator;

    default:
        return defaultFeeCalculator;
    }
}

} // namespace TW::Bitcoin
