// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Bitcoin/FeeCalculator.h"

#include <gtest/gtest.h>

namespace TW::Bitcoin {

TEST(BitcoinFeeCalculator, ConstantFeeCalculator) {
    const auto feeCalculator = ConstantFeeCalculator(33);
    EXPECT_EQ(feeCalculator.calculate(1, 2, 1), 33);
    EXPECT_EQ(feeCalculator.calculate(0, 2, 1), 33);
    EXPECT_EQ(feeCalculator.calculate(1, 2, 10), 33);
    EXPECT_EQ(feeCalculator.calculateSingleInput(10), 0);
}

TEST(BitcoinFeeCalculator, LinearFeeCalculator) {
    const auto feeCalculator = LinearFeeCalculator(10, 20, 50);
    EXPECT_EQ(feeCalculator.calculate(1, 2, 1), 100);
    EXPECT_EQ(feeCalculator.calculate(1, 1, 1), 80);
    EXPECT_EQ(feeCalculator.calculate(0, 2, 1), 90);
    EXPECT_EQ(feeCalculator.calculate(1, 0, 1), 60);
    EXPECT_EQ(feeCalculator.calculate(0, 0, 1), 50);
    EXPECT_EQ(feeCalculator.calculate(1, 2, 10), 1000);
    EXPECT_EQ(feeCalculator.calculateSingleInput(10), 100);
}

TEST(BitcoinFeeCalculator, BitcoinCalculate) {
    const FeeCalculator& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    EXPECT_EQ(feeCalculator.calculate(1, 2, 1), 174);
    EXPECT_EQ(feeCalculator.calculate(1, 1, 1), 143);
    EXPECT_EQ(feeCalculator.calculate(0, 2, 1), 72);
    EXPECT_EQ(feeCalculator.calculate(1, 0, 1), 112);
    EXPECT_EQ(feeCalculator.calculate(0, 0, 1), 10);
    EXPECT_EQ(feeCalculator.calculate(1, 2, 10), 1740);
}

TEST(BitcoinFeeCalculator, SegwitCalculate) {
    const FeeCalculator& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    EXPECT_EQ(feeCalculator.calculate(1, 2, 1), 174);
    EXPECT_EQ(feeCalculator.calculate(1, 1, 1), 143);
    EXPECT_EQ(feeCalculator.calculate(0, 2, 1), 72);
    EXPECT_EQ(feeCalculator.calculate(1, 0, 1), 112);
    EXPECT_EQ(feeCalculator.calculate(0, 0, 1), 10);
    EXPECT_EQ(feeCalculator.calculate(1, 2, 10), 1740);
}

TEST(BitcoinFeeCalculator, BitcoinCalculateNoDustFilter) {
    const FeeCalculator& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin, true);
    EXPECT_EQ(feeCalculator.calculate(1, 2, 1), 174);
    EXPECT_EQ(feeCalculator.calculate(1, 1, 1), 143);
    EXPECT_EQ(feeCalculator.calculate(0, 2, 1), 72);
    EXPECT_EQ(feeCalculator.calculate(1, 0, 1), 112);
    EXPECT_EQ(feeCalculator.calculate(0, 0, 1), 10);
    EXPECT_EQ(feeCalculator.calculate(1, 2, 10), 1740);
    EXPECT_EQ(feeCalculator.calculateSingleInput(1), 0);
}

TEST(BitcoinFeeCalculator, DefaultCalculate) {
    DefaultFeeCalculator defaultFeeCalculator;
    EXPECT_EQ(defaultFeeCalculator.calculate(1, 2, 1), 226);
    EXPECT_EQ(defaultFeeCalculator.calculate(1, 1, 1), 192);
    EXPECT_EQ(defaultFeeCalculator.calculate(0, 2, 1), 78);
    EXPECT_EQ(defaultFeeCalculator.calculate(1, 0, 1), 158);
    EXPECT_EQ(defaultFeeCalculator.calculate(0, 0, 1), 10);
    EXPECT_EQ(defaultFeeCalculator.calculate(1, 2, 10), 2260);
    EXPECT_EQ(defaultFeeCalculator.calculateSingleInput(1), 148);
    EXPECT_EQ(defaultFeeCalculator.calculateSingleInput(2), 296);
    EXPECT_EQ(defaultFeeCalculator.calculateSingleInput(10), 1480);
}

TEST(BitcoinFeeCalculator, DefaultCalculateNoDustFilter) {
    DefaultFeeCalculator defaultFeeCalculator(true);
    EXPECT_EQ(defaultFeeCalculator.calculate(1, 2, 1), 226);
    EXPECT_EQ(defaultFeeCalculator.calculate(1, 1, 1), 192);
    EXPECT_EQ(defaultFeeCalculator.calculate(0, 2, 1), 78);
    EXPECT_EQ(defaultFeeCalculator.calculate(1, 0, 1), 158);
    EXPECT_EQ(defaultFeeCalculator.calculate(0, 0, 1), 10);
    EXPECT_EQ(defaultFeeCalculator.calculate(1, 2, 10), 2260);
    EXPECT_EQ(defaultFeeCalculator.calculateSingleInput(1), 0);
}

TEST(BitcoinFeeCalculator, DecredCalculate) {
    const FeeCalculator& feeCalculator = getFeeCalculator(TWCoinTypeDecred);
    EXPECT_EQ(feeCalculator.calculate(1, 2, 1), 254);
    EXPECT_EQ(feeCalculator.calculate(0, 0, 1), 12);
    EXPECT_EQ(feeCalculator.calculate(1, 2, 10), 2540);
    EXPECT_EQ(feeCalculator.calculateSingleInput(1), 166);
}

TEST(BitcoinFeeCalculator, DecredCalculateNoDustFilter) {
    const FeeCalculator& feeCalculator = getFeeCalculator(TWCoinTypeDecred, true);
    EXPECT_EQ(feeCalculator.calculate(1, 2, 1), 254);
    EXPECT_EQ(feeCalculator.calculate(0, 0, 1), 12);
    EXPECT_EQ(feeCalculator.calculate(1, 2, 10), 2540);
    EXPECT_EQ(feeCalculator.calculateSingleInput(1), 0);
}

} // namespace TW::Bitcoin
