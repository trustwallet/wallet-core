// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <TrustWalletCore/TWCoinType.h>
#include "Amount.h"

namespace TW::Bitcoin {

inline constexpr double gDefaultBytesPerInput{148};
inline constexpr double gDefaultBytesPerOutput{34};
inline constexpr double gDefaultBytesBase{10};
inline constexpr double gSegwitBytesPerInput{101.25};
inline constexpr double gSegwitBytesPerOutput{31};
inline constexpr double gSegwitBytesBase{gDefaultBytesBase};

/// Interface for transaction fee calculator.
class FeeCalculator {
public:
    virtual ~FeeCalculator() noexcept = default;
    [[nodiscard]] virtual Amount calculate(size_t inputs, size_t outputs,
                                            Amount byteFee) const noexcept = 0;
    [[nodiscard]] virtual Amount calculateSingleInput(Amount byteFee) const noexcept = 0;
};

/// Generic fee calculator with linear input and output size, and a fix size
class LinearFeeCalculator : public FeeCalculator {
public:
    const double bytesPerInput;
    const double bytesPerOutput;
    const double bytesBase;
    explicit constexpr LinearFeeCalculator(double bytesPerInput, double bytesPerOutput,
                                           double bytesBase) noexcept
        : bytesPerInput(bytesPerInput), bytesPerOutput(bytesPerOutput), bytesBase(bytesBase) {}

    [[nodiscard]] Amount calculate(size_t inputs, size_t outputs,
                                    Amount byteFee) const noexcept override;
    [[nodiscard]] Amount calculateSingleInput(Amount byteFee) const noexcept override;
};

/// Constant fee calculator
class ConstantFeeCalculator : public FeeCalculator {
public:
    const Amount fee;
    explicit constexpr ConstantFeeCalculator(Amount fee) noexcept : fee(fee) {}

    [[nodiscard]] Amount calculate([[maybe_unused]] size_t inputs, [[maybe_unused]] size_t outputs,
                                    [[maybe_unused]] Amount byteFee) const noexcept final {
        return fee;
    }
    [[nodiscard]] Amount calculateSingleInput([[maybe_unused]] Amount byteFee) const noexcept final { return 0; }
};

/// Default Bitcoin transaction fee calculator, non-segwit.
class DefaultFeeCalculator : public LinearFeeCalculator {
private:
    bool disableDustFilter = false;

public:
    constexpr DefaultFeeCalculator(bool disableFilter = false) noexcept
        : LinearFeeCalculator(gDefaultBytesPerInput, gDefaultBytesPerOutput, gDefaultBytesBase)
        , disableDustFilter(disableFilter) {}
    
    [[nodiscard]] Amount calculateSingleInput(Amount byteFee) const noexcept override {
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
    constexpr SegwitFeeCalculator(bool disableFilter = false) noexcept
        : LinearFeeCalculator(gSegwitBytesPerInput, gSegwitBytesPerOutput, gSegwitBytesBase)
        , disableDustFilter(disableFilter) {}

    [[nodiscard]] Amount calculateSingleInput(Amount byteFee) const noexcept override {
        if (disableDustFilter) {
            return 0;
        }
        return LinearFeeCalculator::calculateSingleInput(byteFee);
    }
};

class Zip0317FeeCalculator: public FeeCalculator {
public:
    static constexpr Amount gMarginalFee = 5000ul;
    static constexpr size_t gGraceActions = 2ul;

    Zip0317FeeCalculator() noexcept = default;

    [[nodiscard]] Amount calculate(size_t inputs, size_t outputs, Amount byteFee) const noexcept final;
    [[nodiscard]] Amount calculateSingleInput([[maybe_unused]] Amount byteFee) const noexcept final {
        return gMarginalFee;
    }
};

/// Return the fee calculator for the given coin.
const FeeCalculator& getFeeCalculator(TWCoinType coinType, bool disableFilter = false, bool zip0317 = false) noexcept;

} // namespace TW::Bitcoin
