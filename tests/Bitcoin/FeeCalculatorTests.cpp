// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Bitcoin/FeeCalculator.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Bitcoin;

TEST(BitcoinFeeCalculator, calculate) {
    FeeCalculator& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    EXPECT_EQ(feeCalculator.calculate(1, 2, 1), 226);
    EXPECT_EQ(feeCalculator.calculate(1, 1, 1), 192);
    EXPECT_EQ(feeCalculator.calculate(0, 2, 1), 78);
    EXPECT_EQ(feeCalculator.calculate(1, 0, 1), 158);
    EXPECT_EQ(feeCalculator.calculate(0, 0, 1), 10);
    EXPECT_EQ(feeCalculator.calculate(1, 2, 10), 2260);
}

TEST(BitcoinDefaultFeeCalculator, calculate) {
    DefaultFeeCalculator defaultFeeCalculator;
    EXPECT_EQ(defaultFeeCalculator.calculate(1, 2, 1), 226);
    EXPECT_EQ(defaultFeeCalculator.calculate(1, 1, 1), 192);
    EXPECT_EQ(defaultFeeCalculator.calculate(0, 2, 1), 78);
    EXPECT_EQ(defaultFeeCalculator.calculate(1, 0, 1), 158);
    EXPECT_EQ(defaultFeeCalculator.calculate(0, 0, 1), 10);
    EXPECT_EQ(defaultFeeCalculator.calculate(1, 2, 10), 2260);
}

TEST(BitcoinDefaultFeeCalculator, calculateSingleInput) {
    DefaultFeeCalculator defaultFeeCalculator;
    EXPECT_EQ(defaultFeeCalculator.calculateSingleInput(1), 148);
    EXPECT_EQ(defaultFeeCalculator.calculateSingleInput(2), 296);
    EXPECT_EQ(defaultFeeCalculator.calculateSingleInput(10), 1480);
}

TEST(ZCashFeeCalculator, calculate) {
    FeeCalculator& feeCalculator = getFeeCalculator(TWCoinTypeZcash);
    EXPECT_EQ(feeCalculator.calculate(1, 2, 1), 10000);
    EXPECT_EQ(feeCalculator.calculate(0, 0, 1), 10000);
    EXPECT_EQ(feeCalculator.calculate(1, 2, 10), 10000);
    EXPECT_EQ(feeCalculator.calculateSingleInput(1), 0);
}

TEST(GroestlcoinFeeCalculator, calculate) {
    FeeCalculator& feeCalculator = getFeeCalculator(TWCoinTypeGroestlcoin);
    EXPECT_EQ(feeCalculator.calculate(1, 2, 1), 20000);
    EXPECT_EQ(feeCalculator.calculate(0, 0, 1), 20000);
    EXPECT_EQ(feeCalculator.calculate(1, 2, 10), 20000);
    EXPECT_EQ(feeCalculator.calculateSingleInput(1), 0);
}

TEST(DecredFeeCalculator, calculate) {
    FeeCalculator& feeCalculator = getFeeCalculator(TWCoinTypeDecred);
    EXPECT_EQ(feeCalculator.calculate(1, 2, 1), 254);
    EXPECT_EQ(feeCalculator.calculate(0, 0, 1), 12);
    EXPECT_EQ(feeCalculator.calculate(1, 2, 10), 2540);
    EXPECT_EQ(feeCalculator.calculateSingleInput(1), 166);
}
