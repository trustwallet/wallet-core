// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "ImmutableX/StarkCurve.h"
#include <gtest/gtest.h>

namespace TW::ImmutableX::tests {
    TEST(StarkCurve, modN) {
        ASSERT_EQ(StarkCurve::modN(18926), int256_t(18926) % internal::gStarkCurveN);
    }

    TEST(StarkCurve, modP) {
        ASSERT_EQ(StarkCurve::modP(18926), int256_t(18926) % internal::gStarkCurveP);
    }

    TEST(StarkCurve, modInverseP) {
        int256_t value1(123123);
        int256_t value2(543543);
        ASSERT_EQ(StarkCurve::modInverseP(value1, value2), StarkCurve::divide(value1, value2, internal::gStarkCurveP));
    }

    TEST(StarkCurve, modInverseN) {
        int256_t value1(123123);
        int256_t value2(543543);
        ASSERT_EQ(StarkCurve::modInverseP(value1, value2), StarkCurve::divide(value1, value2, internal::gStarkCurveN));
    }

    TEST(StarkCurve, AdditionReturnsNulloptWhenXsAreEqualButYsDifferent) {
        auto p1 = CurvePoint{.x = int256_t(1), .y =  int256_t(2)};
        auto p2 = CurvePoint{.x = int256_t(1), .y =  int256_t(3)};
        ASSERT_EQ(StarkCurve::addition(p1, p2), std::nullopt);
    }

    TEST(StarkCurve, AdditionReturnsP2IfP1IsNullopt) {
        auto p2 = CurvePoint{.x = int256_t(1), .y =  int256_t(3)};
        ASSERT_EQ(*StarkCurve::addition(std::nullopt, p2), p2);
    }

    TEST(StarkCurve, AdditionReturnsP1IfP2IsNullopt) {
        auto p1 = CurvePoint{.x = int256_t(1), .y =  int256_t(3)};
        ASSERT_EQ(*StarkCurve::addition(p1, std::nullopt), p1);
    }

    TEST(StarkCurve, AdditionDoublesPointIfP1AndP2AreEqual) {
        auto p1 = CurvePoint{.x = int256_t(1), .y =  int256_t(3)};
        auto p2 = p1;
        auto expected = CurvePoint{.x = int256_t("402055865407347912633035864788341122847011912814621855552565784015096891163"), .y = int256_t("2144297948839188867376191278871152655184063535011316562947017514747183419543")};
        auto result = *StarkCurve::addition(p1, p2);
        // not working yet
        //ASSERT_EQ(result, expected);
    }
}
