// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "../uint256.h"

namespace TW::Theta {

class Coins {
  public:
    uint256_t thetaWei;
    uint256_t tfuelWei;

    Coins() = default;
    Coins(uint256_t thetaWei, uint256_t tfuelWei)
        : thetaWei(std::move(thetaWei)), tfuelWei(std::move(tfuelWei)) {}
};

} // namespace TW::Theta
