// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "DustCalculator.h"

namespace TW::Bitcoin {

FixedDustCalculator::FixedDustCalculator(Amount fixed) noexcept
    : fixedDustAmount(fixed) {
}

Amount FixedDustCalculator::dustAmount([[maybe_unused]] Amount byteFee) noexcept {
    return fixedDustAmount;
}

LegacyDustCalculator::LegacyDustCalculator(TWCoinType coinType) noexcept
    : feeCalculator(getFeeCalculator(coinType, false)) {
}

Amount LegacyDustCalculator::dustAmount([[maybe_unused]] Amount byteFee) noexcept {
    return feeCalculator.calculateSingleInput(byteFee);
}

DustCalculatorShared getDustCalculator(const Proto::SigningInput& input) {
    if (input.disable_dust_filter()) {
        return std::make_shared<FixedDustCalculator>(0);
    }

    if (input.has_fixed_dust_threshold()) {
        return std::make_shared<FixedDustCalculator>(input.fixed_dust_threshold());
    }

    const auto coinType = static_cast<TWCoinType>(input.coin_type());
    return std::make_shared<LegacyDustCalculator>(coinType);
}

} // namespace TW::Bitcoin
