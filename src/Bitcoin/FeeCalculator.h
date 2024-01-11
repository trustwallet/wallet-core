// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <TrustWalletCore/TWCoinType.h>

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
    [[nodiscard]] virtual int64_t calculate(int64_t inputs, int64_t outputs,
                                            int64_t byteFee) const noexcept = 0;
    [[nodiscard]] virtual int64_t calculateSingleInput(int64_t byteFee) const noexcept = 0;
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

    [[nodiscard]] int64_t calculate(int64_t inputs, int64_t outputs,
                                    int64_t byteFee) const noexcept override;
    [[nodiscard]] int64_t calculateSingleInput(int64_t byteFee) const noexcept override;
};

/// Constant fee calculator
class ConstantFeeCalculator : public FeeCalculator {
public:
    const int64_t fee;
    explicit constexpr ConstantFeeCalculator(int64_t fee) noexcept : fee(fee) {}

    [[nodiscard]] int64_t calculate([[maybe_unused]] int64_t inputs, [[maybe_unused]] int64_t outputs,
                                    [[maybe_unused]] int64_t byteFee) const noexcept final {
        return fee;
    }
    [[nodiscard]] int64_t calculateSingleInput([[maybe_unused]] int64_t byteFee) const noexcept final { return 0; }
};

/// Default Bitcoin transaction fee calculator, non-segwit.
class DefaultFeeCalculator : public LinearFeeCalculator {
private:
    bool disableDustFilter = false;

public:
    constexpr DefaultFeeCalculator(bool disableFilter = false) noexcept
        : LinearFeeCalculator(gDefaultBytesPerInput, gDefaultBytesPerOutput, gDefaultBytesBase)
        , disableDustFilter(disableFilter) {}
    
    [[nodiscard]] int64_t calculateSingleInput(int64_t byteFee) const noexcept override {
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

    [[nodiscard]] int64_t calculateSingleInput(int64_t byteFee) const noexcept override {
        if (disableDustFilter) {
            return 0;
        }
        return LinearFeeCalculator::calculateSingleInput(byteFee);
    }
};

/// Return the fee calculator for the given coin.
const FeeCalculator& getFeeCalculator(TWCoinType coinType, bool disableFilter = false) noexcept;

} // namespace TW::Bitcoin
