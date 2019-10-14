// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <cstdint>
#include <math.h>
#include <stdint.h>
#include <string>
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

class Description {
  public:
    std::string name;
    std::string identity;
    std::string website;
    std::string securityContact;
    std::string details;
    Description(std::string name, std::string identity, std::string website,
                std::string securityContact, std::string details)
        : name(std::move(name))
        , identity(std::move(identity))
        , website(std::move(website))
        , securityContact(std::move(securityContact))
        , details(std::move(details)) {}
};

#define PRECISION 18
class Decimal {
  public:
    uint256_t value;
    Decimal(uint256_t value, uint256_t precision) {
        if (precision > PRECISION) {
            throw std::invalid_argument("too much precision ...");
        }
        int zerosToAdd = (int)(PRECISION - precision);
        uint256_t multiplier = (uint256_t)pow(10, zerosToAdd);
        this->value = value * multiplier;
    }
};

class CommissionRate {
  public:
    Decimal rate;
    Decimal maxRate;
    Decimal maxChangeRate;
    CommissionRate(Decimal rate, Decimal maxRate, Decimal maxChangeRate)
        : rate(std::move(rate))
        , maxRate(std::move(maxRate))
        , maxChangeRate(std::move(maxChangeRate)) {}
};

class NewValidator {
  public:
    Description description;
    CommissionRate commission;
    uint256_t minSelfDelegation;
    Address stakingAddress;
    std::vector<uint8_t> pubKey;
    uint256_t amount;
    NewValidator(Description description, CommissionRate commission, uint256_t minSelfDelegation,
                 Address stakingAddress, Data pubKey, uint256_t amount)
        : description(std::move(description))
        , commission(std::move(commission))
        , minSelfDelegation(std::move(minSelfDelegation))
        , stakingAddress(std::move(stakingAddress))
        , pubKey(std::move(pubKey))
        , amount(std::move(amount)) {}
};

class EditValidator {
  public:
    Description description;
    Address stakingAddress;
    Decimal commissionRate;
    uint256_t minSelfDelegation;
    EditValidator(Description description, Address stakingAddress, Decimal commissionRate,
                  uint256_t minSelfDelegation)
        : description(std::move(description))
        , stakingAddress(std::move(stakingAddress))
        , commissionRate(std::move(commissionRate))
        , minSelfDelegation(std::move(minSelfDelegation)) {}
};

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

class Redelegate {
  public:
    Address delegatorAddress;
    Address validatorSrcAddress;
    Address validatorDstAddress;
    uint256_t amount;
    Redelegate(Address delegatorAddress, Address validatorSrcAddress, Address validatorDstAddress,
               uint256_t amount)
        : delegatorAddress(std::move(delegatorAddress))
        , validatorSrcAddress(std::move(validatorSrcAddress))
        , validatorDstAddress(std::move(validatorDstAddress))
        , amount(std::move(amount)) {}
};

class Undelegate {
  public:
    Address delegatorAddress;
    Address validatorAddress;
    uint256_t amount;
    Undelegate(Address delegatorAddress, Address validatorAddress, uint256_t amount)
        : delegatorAddress(std::move(delegatorAddress))
        , validatorAddress(std::move(validatorAddress))
        , amount(std::move(amount)) {}
};

} // namespace TW::Harmony
