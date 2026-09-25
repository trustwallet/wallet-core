// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <cstdint>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <optional>

#include "Address.h"
#include "../uint256.h"
using namespace std;

namespace TW::Harmony {

template <typename Directive>
class Staking {
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

    Staking(uint8_t directive, Directive stakeMsg, uint256_t nonce, uint256_t gasPrice,
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
    DirectiveCreateValidator,
    DirectiveEditValidator,
    DirectiveDelegate,
    DirectiveUndelegate,
    DirectiveCollectRewards
};

class Description {
  public:
    string name;
    string identity;
    string website;
    string securityContact;
    string details;

    Description(string name, string identity, string website, string securityContact,
                string details)
        : name(std::move(name))
        , identity(std::move(identity))
        , website(std::move(website))
        , securityContact(std::move(securityContact))
        , details(std::move(details)) {}
};

const uint256_t MAX_PRECISION = 18;

class Decimal {
  public:
    uint256_t value;

    Decimal(uint256_t value, uint256_t precision) {
        if (precision < 1 || precision > MAX_PRECISION) {
            throw std::invalid_argument("invalid precision, must be between 1 and 17");
        }
        int zerosToAdd = static_cast<int>(MAX_PRECISION - precision);
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
        : rate(std::move(rate)), maxRate(std::move(maxRate)), maxChangeRate(std::move(maxChangeRate)) {}
};

class CreateValidator {
  public:
    Address validatorAddress;
    Description description;
    CommissionRate commissionRates;
    uint256_t minSelfDelegation;
    uint256_t maxTotalDelegation;
    uint256_t amount;
    vector<vector<uint8_t>> slotPubKeys;
    vector<vector<uint8_t>> slotKeySigs;

    CreateValidator(Address validatorAddress, Description description,
                    CommissionRate commissionRates, uint256_t minSelfDelegation,
                    uint256_t maxTotalDelegation, vector<vector<uint8_t>> slotPubKeys,
                    vector<vector<uint8_t>> slotKeySigs,uint256_t amount)
        : validatorAddress(std::move(validatorAddress))
        , description(std::move(description))
        , commissionRates(std::move(commissionRates))
        , minSelfDelegation(std::move(minSelfDelegation))
        , maxTotalDelegation(std::move(maxTotalDelegation))
        , amount(std::move(amount))
        , slotPubKeys(std::move(slotPubKeys))
        , slotKeySigs(std::move(slotKeySigs)) {}
};

class EditValidator {
  public:
    Address validatorAddress;
    Description description;
    std::optional<Decimal> commissionRate;
    uint256_t minSelfDelegation;
    uint256_t maxTotalDelegation;
    vector<uint8_t> slotKeyToRemove;
    vector<uint8_t> slotKeyToAdd;
    vector<uint8_t> slotKeyToAddSig;
    uint256_t        active;

    EditValidator(Address validatorAddress, Description description, std::optional<Decimal>& commissionRate,
                  uint256_t minSelfDelegation, uint256_t maxTotalDelegation,
                  vector<uint8_t> slotKeyToRemove, vector<uint8_t> slotKeyToAdd,
                  vector<uint8_t> slotKeyToAddSig, uint256_t  active)
        : validatorAddress(std::move(validatorAddress))
        , description(std::move(description))
        , commissionRate(std::move(commissionRate))
        , minSelfDelegation(std::move(minSelfDelegation))
        , maxTotalDelegation(std::move(maxTotalDelegation))
        , slotKeyToRemove(std::move(slotKeyToRemove))
        , slotKeyToAdd(std::move(slotKeyToAdd))
        , slotKeyToAddSig(std::move(slotKeyToAddSig))
        , active(std::move(active)){}
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

class CollectRewards {
  public:
    Address delegatorAddress;

    CollectRewards(Address delegatorAddress) : delegatorAddress(std::move(delegatorAddress)) {}
};

} // namespace TW::Harmony
