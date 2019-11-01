// Copyright © 2017-2019 Trust.
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

#include "Address.h"
#include "../uint256.h"
using namespace std;

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

#define PRECISION 18
class Decimal {
  public:
    uint256_t value;

    Decimal(string value) {
        if (value.length() == 0) {
            throw invalid_argument("decimal string is empty");
        }
        string value1 = value;
        if (value[0] == '-') {
            // ignore the negative
            value1 = value.substr(1);
        }
        if (value1.length() == 0) {
            throw invalid_argument("decimal string is empty");
        }
        vector<string> strs;
        boost::split(strs, value1, boost::is_any_of("."));
        int lenDecs = 0;
        string combinedStr = strs[0];
        if (strs.size() == 2) {
            lenDecs = (int)strs[1].length();
            if (lenDecs == 0 || combinedStr.length() == 0) {
                throw invalid_argument("bad decimal length");
            }
            combinedStr += strs[1];
        } else if (strs.size() > 2) {
            throw invalid_argument("too many periods to be a decimal string");
        }

        if (lenDecs > PRECISION) {
            throw invalid_argument("too much precision");
        }

        int zerosToAdd = (int)(PRECISION - lenDecs);
        char zeros[PRECISION];
        sprintf(zeros, "%0*d", zerosToAdd, 0);
        combinedStr += zeros;
        combinedStr.erase(0, combinedStr.find_first_not_of('0'));
        this->value = uint256_t(combinedStr);
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
    vector<vector<uint8_t>> slotPubKeys;
    uint256_t amount;
    CreateValidator(Address validatorAddress, Description description,
                    CommissionRate commissionRates, uint256_t minSelfDelegation,
                    uint256_t maxTotalDelegation, vector<Data> slotPubKeys, uint256_t amount)
        : validatorAddress(move(validatorAddress))
        , description(move(description))
        , commissionRates(move(commissionRates))
        , minSelfDelegation(move(minSelfDelegation))
        , maxTotalDelegation(move(maxTotalDelegation))
        , slotPubKeys(move(slotPubKeys))
        , amount(move(amount)) {}
};

class EditValidator {
  public:
    Address validatorAddress;
    Description description;
    Decimal commissionRate;
    uint256_t minSelfDelegation;
    uint256_t maxTotalDelegation;
    vector<uint8_t> slotKeyToRemove;
    vector<uint8_t> slotKeyToAdd;
    EditValidator(Address validatorAddress, Description description, Decimal commissionRate,
                  uint256_t minSelfDelegation, uint256_t maxTotalDelegation, Data slotKeyToRemove,
                  Data slotKeyToAdd)
        : validatorAddress(move(validatorAddress))
        , description(move(description))
        , commissionRate(move(commissionRate))
        , minSelfDelegation(move(minSelfDelegation))
        , maxTotalDelegation(move(maxTotalDelegation))
        , slotKeyToRemove(move(slotKeyToRemove))
        , slotKeyToAdd(move(slotKeyToAdd)) {}
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
