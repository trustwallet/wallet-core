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

/// Constant fee calculator
class ConstantFeeCalculator : public FeeCalculator {
public:
    const int64_t fee;
    ConstantFeeCalculator(int64_t fee) : fee(fee) {}

    virtual int64_t calculate(int64_t inputs, int64_t outputs, int64_t byteFee) const override { return fee; }
    virtual int64_t calculateSingleInput(int64_t byteFee) const override { return 0; }
};

/// Default Bitcoin transaction fee calculator, non-segwit.
class DefaultFeeCalculator : public LinearFeeCalculator {
private:
    bool disableDustFilter = false;

public:
    DefaultFeeCalculator(bool disableFilter = false)
        :LinearFeeCalculator(148, 34, 10), disableDustFilter(disableFilter) {}
    
    virtual int64_t calculateSingleInput(int64_t byteFee) const override {
        if (disableDustFilter) { 
            return 0; 
        }
        return LinearFeeCalculator::calculateSingleInput(byteFee);
    }
};

/// Bitcoin Segwit transaction fee calculator
class SegwitFeeCalculator : public LinearFeeCalculator {
private:
    bool disableDustFilter = false;
    
public:
    SegwitFeeCalculator(bool disableFilter = false)
        :LinearFeeCalculator(101.25, 31, 10), disableDustFilter(disableFilter) {}

    virtual int64_t calculateSingleInput(int64_t byteFee) const override {
        if (disableDustFilter) { 
            return 0; 
        }
        return LinearFeeCalculator::calculateSingleInput(byteFee);
    }
};

/// Return the fee calculator for the given coin.
FeeCalculator& getFeeCalculator(TWCoinType coinType, bool disableFilter = false);

} // namespace TW::Bitcoin
