// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <boost/integer/mod_inverse.hpp>
#include <optional>

#include "ImmutableX/Constants.h"

namespace TW::ImmutableX {
struct StarkCurve {
    static int256_t modN(int256_t number) {
        return number % internal::gStarkCurveN;
    }

    static int256_t modP(int256_t number) {
        return number % internal::gStarkCurveP;
    }

    static int256_t modInverseN(int256_t dividend, int256_t divisor) {
        return divide(dividend, divisor, internal::gStarkCurveN);
    }

    static int256_t modInverseP(int256_t dividend, int256_t divisor) {
        return divide(dividend, divisor, internal::gStarkCurveP);
    }

    static int256_t divide(int256_t dividend, int256_t divisor, int256_t mod) {
        auto d1 = dividend > 0 ? dividend : dividend + mod;
        auto d2 = divisor > 0 ? divisor : divisor + mod;
        return (boost::integer::mod_inverse(d2, mod) * d1) % mod;
    }

    static CurvePoint addPoint(const CurvePoint& p1, const CurvePoint& p2) {
        auto mipResult = modInverseP(p2.y - p1.y, p2.x - p1.x);
        auto x3 = modP(mipResult * mipResult - p1.x - p2.x);
        auto y3 = modP(mipResult * (p1.x - x3) - p1.y);
        return CurvePoint{.x = x3, .y = y3};
    }

    static CurvePoint doublePoint(const CurvePoint& p) {
        auto mipResult = modInverseP(3 * (p.x * p.x) + internal::gStarkCurveA, 2 * p.y);
        auto x3 = modP(mipResult * mipResult - 2 * p.x);
        auto y3 = modP(mipResult * (p.x - x3) - p.y);
        return CurvePoint{.x = x3, .y = y3};
    }

    static std::optional<CurvePoint> addition(std::optional<CurvePoint> p1, std::optional<CurvePoint> p2) {
        if (!p1.has_value()) {
            return p2;
        }
        if (!p2.has_value()) {
            return p1;
        }
        auto p1_val = p1.value();
        auto p2_val = p2.value();
        if (p1_val.x == p2_val.x && p1_val.y != p2_val.y) {
            return std::nullopt;
        }
        if (p1_val == p2_val) {
            return doublePoint(p1_val);
        } else {
            return addPoint(p1_val, p2_val);
        }
    }
};
} // namespace TW::ImmutableX
