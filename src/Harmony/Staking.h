// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <boost/algorithm/string.hpp>
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
        : directive(move(directive))
        , stakeMsg(move(stakeMsg))
        , nonce(move(nonce))
        , gasPrice(move(gasPrice))
        , gasLimit(move(gasLimit))
        , v(move(v))
        , r(move(r))
        , s(move(s)) {}
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
        : name(move(name))
        , identity(move(identity))
        , website(move(website))
        , securityContact(move(securityContact))
        , details(move(details)) {}
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
        : rate(move(rate)), maxRate(move(maxRate)), maxChangeRate(move(maxChangeRate)) {}
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
        : validatorAddress(move(validatorAddress))
        , description(move(description))
        , commissionRates(move(commissionRates))
        , minSelfDelegation(move(minSelfDelegation))
        , maxTotalDelegation(move(maxTotalDelegation))
        , amount(move(amount))
        , slotPubKeys(move(slotPubKeys))
        , slotKeySigs(move(slotKeySigs)) {}
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
        : validatorAddress(move(validatorAddress))
        , description(move(description))
        , commissionRate(move(commissionRate))
        , minSelfDelegation(move(minSelfDelegation))
        , maxTotalDelegation(move(maxTotalDelegation))
        , slotKeyToRemove(move(slotKeyToRemove))
        , slotKeyToAdd(move(slotKeyToAdd))
        , slotKeyToAddSig(move(slotKeyToAddSig))
        , active(move(active)){}
};

class Delegate {
  public:
    Address delegatorAddress;
    Address validatorAddress;
    uint256_t amount;

    Delegate(Address delegatorAddress, Address validatorAddress, uint256_t amount)
        : delegatorAddress(move(delegatorAddress))
        , validatorAddress(move(validatorAddress))
        , amount(move(amount)) {}
};

class Undelegate {
  public:
    Address delegatorAddress;
    Address validatorAddress;
    uint256_t amount;

    Undelegate(Address delegatorAddress, Address validatorAddress, uint256_t amount)
        : delegatorAddress(move(delegatorAddress))
        , validatorAddress(move(validatorAddress))
        , amount(move(amount)) {}
};

class CollectRewards {
  public:
    Address delegatorAddress;

    CollectRewards(Address delegatorAddress) : delegatorAddress(move(delegatorAddress)) {}
};

} // namespace TW::Harmony
