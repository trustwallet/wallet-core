// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Serialization.h"

#include "../Cosmos/Address.h"
#include "../Base64.h"

using namespace TW;
using namespace TW::Cosmos;
using namespace TW::Cosmos::Proto;

using json = nlohmann::json;
using string = std::string;

const string TYPE_PREFIX_MSG_SEND = "cosmos-sdk/MsgSend";
const string TYPE_PREFIX_MSG_DELEGATE = "cosmos-sdk/MsgDelegate";
const string TYPE_PREFIX_MSG_UNDELEGATE = "cosmos-sdk/MsgUndelegate";
const string TYPE_PREFIX_MSG_REDELEGATE = "cosmos-sdk/MsgBeginRedelegate";
const string TYPE_PREFIX_MSG_WITHDRAW_REWARD = "cosmos-sdk/MsgWithdrawDelegationReward";
const string TYPE_PREFIX_MSG_WITHDRAW_REWARDS_ALL = "cosmos-sdk/MsgWithdrawDelegationRewardsAll";
const string TYPE_PREFIX_PUBLIC_KEY = "tendermint/PubKeySecp256k1";

static json broadcastJSON(json& jsonObj) {
    json jsonMsgWrapper;

    jsonMsgWrapper["tx"] = jsonObj;
    jsonMsgWrapper["mode"] = "block";

    return jsonMsgWrapper;
}

static json wrapperJSON(const string& type, json& jsonObj) {
    json jsonMsgWrapper;

    jsonMsgWrapper["type"] = type;
    jsonMsgWrapper["value"] = jsonObj;

    return jsonMsgWrapper;
}

static json amountJSON(const string& amount, const string& denom) {
    json jsonAmount;

    jsonAmount["amount"] = amount;
    jsonAmount["denom"] = denom;

    return jsonAmount;
}

static json feeJSON(const Fee& fee) {
    json jsonAmounts = json::array();

    for (auto& amount : fee.amounts()) {
        jsonAmounts.push_back(
            amountJSON(std::to_string(amount.amount()), amount.denom()));
    }

    json jsonFee;

    jsonFee["amount"] = jsonAmounts;
    jsonFee["gas"] = std::to_string(fee.gas());

    return jsonFee;
}

static json sendCoinsMessage(json& amounts, const string& from_address, const string& to_address, const string& type_prefix) {
    json jsonMsg;

    jsonMsg["amount"] = amounts;
    jsonMsg["from_address"] = from_address;
    jsonMsg["to_address"] = to_address;

    return wrapperJSON(type_prefix, jsonMsg);
}

static json stakeMessage(json& amount, const string& delegator_address, const string& validator_address, const string& type_prefix) {
    json jsonMsg;

    jsonMsg["amount"] = amount;
    jsonMsg["delegator_address"] = delegator_address;
    jsonMsg["validator_address"] = validator_address;

    return wrapperJSON(type_prefix, jsonMsg);
}

static json restakeMessage(json& amount, const string& delegator_address, const string& validator_src_address, const string& validator_dst_address, const string& type_prefix) {
    json jsonMsg;

    jsonMsg["amount"] = amount;
    jsonMsg["delegator_address"] = delegator_address;
    jsonMsg["validator_src_address"] = validator_src_address;
    jsonMsg["validator_dst_address"] = validator_dst_address;

    return wrapperJSON(type_prefix, jsonMsg);
}

static json withdrawStakeRewardMessage(const string& delegator_address, const string& validator_address, const string& type_prefix) {
    json jsonMsg;

    jsonMsg["delegator_address"] = delegator_address;
    jsonMsg["validator_address"] = validator_address;

    return wrapperJSON(type_prefix, jsonMsg);
}

static json withdrawStakeRewardsAllMessage(const string& delegator_address, const string& type_prefix) {
    json jsonMsg;

    jsonMsg["delegator_address"] = delegator_address;

    return wrapperJSON(type_prefix, jsonMsg);
}

static json sendCoinsMessage(const SendCoinsMessage& message) {
    json jsonAmounts = json::array();

    for (auto& amount : message.amounts()) {
        jsonAmounts.push_back(amountJSON(std::to_string(amount.amount()), amount.denom()));
    }

    return sendCoinsMessage(jsonAmounts, message.from_address(), message.to_address(), message.type_prefix());
}


static json stakeMessage(const StakeMessage& message) {
    auto amount = message.amount();
    json jsonAmount = amountJSON(std::to_string(amount.amount()), amount.denom());

    return stakeMessage(jsonAmount, message.delegator_address(), message.validator_address(), message.type_prefix());
}

static json restakeMessage(const ReStakeMessage& message) {
    auto amount = message.amount();
    json jsonAmount = amountJSON(std::to_string(amount.amount()), amount.denom());

    return restakeMessage(jsonAmount, message.delegator_address(), message.validator_src_address(),
            message.validator_dst_address(), message.type_prefix());
}


static json withdrawStakeRewardMessage(const WithdrawStakeRewardMessage& message) {
    return withdrawStakeRewardMessage(message.delegator_address(), message.validator_address(), message.type_prefix());
}

static json withdrawStakeRewardsAllMessage(const WithdrawStakeRewardsAllMessage& message) {
    return withdrawStakeRewardsAllMessage(message.delegator_address(), message.type_prefix());
}

static json messageJSON(const SigningInput& input) {
    if (input.has_send_coins_message()) {
        return sendCoinsMessage(input.send_coins_message());
    } else if (input.has_stake_message()) {
        return stakeMessage(input.stake_message());
    } else if (input.has_unstake_message()) {
        return stakeMessage(input.unstake_message());
    } else if (input.has_restake_message()) {
        return restakeMessage(input.restake_message());
    } else if (input.has_withdraw_stake_reward_message()) {
        return withdrawStakeRewardMessage(input.withdraw_stake_reward_message());
    } else if (input.has_withdraw_stake_rewards_all_message()) {
        return withdrawStakeRewardsAllMessage(input.withdraw_stake_rewards_all_message());
    }

    return nullptr;
}

static json messageJSON(const Transaction& transaction) {
    if (transaction.has_send_coins_message()) {
        return sendCoinsMessage(transaction.send_coins_message());
    } else if (transaction.has_stake_message()) {
        return stakeMessage(transaction.stake_message());
    } else if (transaction.has_unstake_message()) {
        return stakeMessage(transaction.unstake_message());
    } else if (transaction.has_restake_message()) {
        return restakeMessage(transaction.restake_message());
    } else if (transaction.has_withdraw_stake_reward_message()) {
        return withdrawStakeRewardMessage(transaction.withdraw_stake_reward_message());
    } else if (transaction.has_withdraw_stake_rewards_all_message()) {
        return withdrawStakeRewardsAllMessage(transaction.withdraw_stake_rewards_all_message());
    }

    return nullptr;
}

static json signatureJSON(const Signature& signature) {
    json jsonSignature;

    jsonSignature["pub_key"]["type"] = TYPE_PREFIX_PUBLIC_KEY;
    jsonSignature["pub_key"]["value"] = Base64::encode(Data(signature.public_key().begin(), signature.public_key().end()));
    jsonSignature["signature"] = Base64::encode(Data(signature.signature().begin(), signature.signature().end()));

    return jsonSignature;
}

json TW::Cosmos::signaturePreimageJSON(const SigningInput& input) {
    json jsonForSigning;

    jsonForSigning["account_number"] = std::to_string(input.account_number());
    jsonForSigning["chain_id"] = input.chain_id();
    jsonForSigning["fee"] = feeJSON(input.fee());
    jsonForSigning["memo"] = input.memo();
    jsonForSigning["msgs"] = json::array({messageJSON(input)});
    jsonForSigning["sequence"] = std::to_string(input.sequence());

    return jsonForSigning;
}

json TW::Cosmos::transactionJSON(const Transaction& transaction, const string& type_prefix) {
    json jsonTx;

    jsonTx["type"] = type_prefix;
    jsonTx["fee"] = feeJSON(transaction.fee());
    jsonTx["memo"] = transaction.memo();
    jsonTx["msg"] = json::array({messageJSON(transaction)});
    jsonTx["signatures"] = json::array({signatureJSON(transaction.signature())});

    return broadcastJSON(jsonTx);
}
