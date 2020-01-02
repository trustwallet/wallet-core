// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <TrustWalletCore/TWCoinType.h>
#include <functional>

namespace TW::Bitcoin {

using FeeCalculator = std::function<int64_t(size_t, size_t, int64_t)>;
using SingleInputFeeCalculator = std::function<int64_t(int64_t)>;

class UnspentCalculator {
  public:
    static UnspentCalculator getCalculator(TWCoinType coinType);

    FeeCalculator calculate;
    SingleInputFeeCalculator calculateSingleInput;

    UnspentCalculator()
        : calculate(UnspentCalculator::calculateFee)
        , calculateSingleInput(UnspentCalculator::calculateSingleInputFee) {}
    UnspentCalculator(FeeCalculator calculateFee,
                      SingleInputFeeCalculator calculateSingleInputFee)
        : calculate(std::move(calculateFee)), calculateSingleInput(std::move(calculateSingleInputFee)) {}

  private:
    static int64_t calculateFee(size_t inputs, size_t outputs = 2, int64_t byteFee = 1);
    static int64_t calculateSingleInputFee(int64_t byteFee);
};

} // namespace TW::Bitcoin
