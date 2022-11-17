// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "uint256.h"

namespace TW::ImmutableX {
    struct CurvePoint {
    bool operator==(const CurvePoint& rhs) const {
        return x == rhs.x &&
               y == rhs.y;
    }
    bool operator!=(const CurvePoint& rhs) const {
        return !(rhs == *this);
    }
    int256_t x;
        int256_t y;
    };
}
