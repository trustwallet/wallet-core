// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Bitcoin/FeeCalculator.h"

#include <gtest/gtest.h>

namespace TW::Bitcoin {

TEST(BitcoinFeeCalculator, ConstantFeeCalculator) {
    const auto feeCalculator = ConstantFeeCalculator(33);
    EXPECT_EQ(feeCalculator.calculate(1ull, 2ull, 1ull), 33ull);
    EXPECT_EQ(feeCalculator.calculate(0ull, 2ull, 1ull), 33ull);
    EXPECT_EQ(feeCalculator.calculate(1ull, 2ull, 10ull), 33ull);
    EXPECT_EQ(feeCalculator.calculateSingleInput(10ull), 0ull);
}

TEST(BitcoinFeeCalculator, LinearFeeCalculator) {
    const auto feeCalculator = LinearFeeCalculator(10, 20, 50);
    EXPECT_EQ(feeCalculator.calculate(1ull, 2ull, 1ull), 100ull);
    EXPECT_EQ(feeCalculator.calculate(1ull, 1ull, 1ull), 80ull);
    EXPECT_EQ(feeCalculator.calculate(0ull, 2ull, 1ull), 90ull);
    EXPECT_EQ(feeCalculator.calculate(1ull, 0ull, 1ull), 60ull);
    EXPECT_EQ(feeCalculator.calculate(0ull, 0ull, 1ull), 50ull);
    EXPECT_EQ(feeCalculator.calculate(1ull, 2ull, 10ull), 1000ull);
    EXPECT_EQ(feeCalculator.calculateSingleInput(10ull), 100ull);
}

TEST(BitcoinFeeCalculator, BitcoinCalculate) {
    const FeeCalculator& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    EXPECT_EQ(feeCalculator.calculate(1ull, 2ull, 1ull), 174ull);
    EXPECT_EQ(feeCalculator.calculate(1ull, 1ull, 1ull), 143ull);
    EXPECT_EQ(feeCalculator.calculate(0ull, 2ull, 1ull), 72ull);
    EXPECT_EQ(feeCalculator.calculate(1ull, 0ull, 1ull), 112ull);
    EXPECT_EQ(feeCalculator.calculate(0ull, 0ull, 1ull), 10ull);
    EXPECT_EQ(feeCalculator.calculate(1ull, 2ull, 10ull), 1740ull);
}

TEST(BitcoinFeeCalculator, SegwitCalculate) {
    const FeeCalculator& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    EXPECT_EQ(feeCalculator.calculate(1ull, 2ull, 1ull), 174ull);
    EXPECT_EQ(feeCalculator.calculate(1ull, 1ull, 1ull), 143ull);
    EXPECT_EQ(feeCalculator.calculate(0ull, 2ull, 1ull), 72ull);
    EXPECT_EQ(feeCalculator.calculate(1ull, 0ull, 1ull), 112ull);
    EXPECT_EQ(feeCalculator.calculate(0ull, 0ull, 1ull), 10ull);
    EXPECT_EQ(feeCalculator.calculate(1ull, 2ull, 10ull), 1740ull);
}

TEST(BitcoinFeeCalculator, BitcoinCalculateNoDustFilter) {
    const FeeCalculator& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin, true);
    EXPECT_EQ(feeCalculator.calculate(1ull, 2ull, 1ull), 174ull);
    EXPECT_EQ(feeCalculator.calculate(1ull, 1ull, 1ull), 143ull);
    EXPECT_EQ(feeCalculator.calculate(0ull, 2ull, 1ull), 72ull);
    EXPECT_EQ(feeCalculator.calculate(1ull, 0ull, 1ull), 112ull);
    EXPECT_EQ(feeCalculator.calculate(0ull, 0ull, 1ull), 10ull);
    EXPECT_EQ(feeCalculator.calculate(1ull, 2ull, 10ull), 1740ull);
    EXPECT_EQ(feeCalculator.calculateSingleInput(1ull), 0ull);
}

TEST(BitcoinFeeCalculator, DefaultCalculate) {
    DefaultFeeCalculator defaultFeeCalculator;
    EXPECT_EQ(defaultFeeCalculator.calculate(1ull, 2ull, 1ull), 226ull);
    EXPECT_EQ(defaultFeeCalculator.calculate(1ull, 1ull, 1ull), 192ull);
    EXPECT_EQ(defaultFeeCalculator.calculate(0ull, 2ull, 1ull), 78ull);
    EXPECT_EQ(defaultFeeCalculator.calculate(1ull, 0ull, 1ull), 158ull);
    EXPECT_EQ(defaultFeeCalculator.calculate(0ull, 0ull, 1ull), 10ull);
    EXPECT_EQ(defaultFeeCalculator.calculate(1ull, 2ull, 10ull), 2260ull);
    EXPECT_EQ(defaultFeeCalculator.calculateSingleInput(1ull), 148ull);
    EXPECT_EQ(defaultFeeCalculator.calculateSingleInput(2ull), 296ull);
    EXPECT_EQ(defaultFeeCalculator.calculateSingleInput(10ull), 1480ull);
}

TEST(BitcoinFeeCalculator, DefaultCalculateNoDustFilter) {
    DefaultFeeCalculator defaultFeeCalculator(true);
    EXPECT_EQ(defaultFeeCalculator.calculate(1ull, 2ull, 1ull), 226ull);
    EXPECT_EQ(defaultFeeCalculator.calculate(1ull, 1ull, 1ull), 192ull);
    EXPECT_EQ(defaultFeeCalculator.calculate(0ull, 2ull, 1ull), 78ull);
    EXPECT_EQ(defaultFeeCalculator.calculate(1ull, 0ull, 1ull), 158ull);
    EXPECT_EQ(defaultFeeCalculator.calculate(0ull, 0ull, 1ull), 10ull);
    EXPECT_EQ(defaultFeeCalculator.calculate(1ull, 2ull, 10ull), 2260ull);
    EXPECT_EQ(defaultFeeCalculator.calculateSingleInput(1ull), 0ull);
}

TEST(BitcoinFeeCalculator, DecredCalculate) {
    const FeeCalculator& feeCalculator = getFeeCalculator(TWCoinTypeDecred);
    EXPECT_EQ(feeCalculator.calculate(1ull, 2ull, 1ull), 254ull);
    EXPECT_EQ(feeCalculator.calculate(0ull, 0ull, 1ull), 12ull);
    EXPECT_EQ(feeCalculator.calculate(1ull, 2ull, 10ull), 2540ull);
    EXPECT_EQ(feeCalculator.calculateSingleInput(1ull), 166ull);
}

TEST(BitcoinFeeCalculator, DecredCalculateNoDustFilter) {
    const FeeCalculator& feeCalculator = getFeeCalculator(TWCoinTypeDecred, true);
    EXPECT_EQ(feeCalculator.calculate(1ull, 2ull, 1ull), 254ull);
    EXPECT_EQ(feeCalculator.calculate(0ull, 0ull, 1ull), 12ull);
    EXPECT_EQ(feeCalculator.calculate(1ull, 2ull, 10ull), 2540ull);
    EXPECT_EQ(feeCalculator.calculateSingleInput(1ull), 0ull);
}

} // namespace TW::Bitcoin
