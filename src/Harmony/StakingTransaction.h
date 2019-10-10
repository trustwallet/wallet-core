// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <cstdint>
#include <stdint.h>
#include <vector>

#include "Address.h"
#include "../uint256.h"

namespace TW::Harmony {

template <typename Directive>
class StakingTransaction {
  public:
    uint8_t directive;
    Directive stakeMsg;
    uint256_t nonce;
    uint256_t gasPrice;
    uint256_t gasLimit;

    // Signature values
    uint256_t v = uint256_t();
    uint256_t r = uint256_t();
    uint256_t s = uint256_t();

    StakingTransaction(uint8_t directive, Directive stakeMsg, uint256_t nonce, uint256_t gasPrice,
                       uint256_t gasLimit, uint256_t v, uint256_t r, uint256_t s)
        : directive(std::move(directive))
        , stakeMsg(std::move(stakeMsg))
        , nonce(std::move(nonce))
        , gasPrice(std::move(gasPrice))
        , gasLimit(std::move(gasLimit))
        , v(std::move(v))
        , r(std::move(r))
        , s(std::move(s)) {}
};

enum Directive : uint8_t {
    DirectiveNewValidator,
    DirectiveEditValidator,
    DirectiveDelegate,
    DirectiveRedelegate,
    DirectiveUndelegate
};

class NewValidator {};

class EditValidator {};

class Delegate {
  public:
    Address delegatorAddress;
    Address validatorAddress;
    uint256_t amount;

    Delegate(Address delegatorAddress, Address validatorAddress, uint256_t amount)
        : delegatorAddress(std::move(delegatorAddress))
        , validatorAddress(std::move(validatorAddress))
        , amount(std::move(amount)) {}
};

class Redelegate {};

class Undelegate {};

} // namespace TW::Harmony
