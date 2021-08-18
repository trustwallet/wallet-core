// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <TrustWalletCore/TWCoinType.h>

namespace TW::Bitcoin {

/// Interface for transaction fee calculator.
class FeeCalculator {
public:
    virtual int64_t calculate(int64_t inputs, int64_t outputs, int64_t byteFee) const = 0;
    virtual int64_t calculateSingleInput(int64_t byteFee) const = 0;
};

/// Generic fee calculator with linear input and output size, and a fix size
class LinearFeeCalculator : public FeeCalculator {
public:
    const double bytesPerInput;
    const double bytesPerOutput;
    const double bytesBase;
    LinearFeeCalculator(double bytesPerInput, double bytesPerOutput, double bytesBase)
        :bytesPerInput(bytesPerInput), bytesPerOutput(bytesPerOutput), bytesBase(bytesBase) {}

    virtual int64_t calculate(int64_t inputs, int64_t outputs, int64_t byteFee) const override;
    virtual int64_t calculateSingleInput(int64_t byteFee) const override;
};

/// Default Bitcoin transaction fee calculator, non-segwit.
class DefaultFeeCalculator : public LinearFeeCalculator {
public:
    DefaultFeeCalculator(): LinearFeeCalculator(148, 34, 10) {}
};

/// Bitcoin Segwit transaction fee calculator
class SegwitFeeCalculator : public LinearFeeCalculator {
public:
    SegwitFeeCalculator(): LinearFeeCalculator(101.25, 31, 10) {}
};

/// Return the fee calculator for the given coin.
FeeCalculator& getFeeCalculator(TWCoinType coinType);

} // namespace TW::Bitcoin
