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

const std::string AMINO_PREFIX_SEND_COIN_MESSAGE = "cosmos-sdk/MsgSend";
const std::string AMINO_PREFIX_STAKE_MESSAGE = "cosmos-sdk/MsgDelegate";
const std::string AMINO_PREFIX_UNSTAKE_MESSAGE = "cosmos-sdk/MsgUndelegate";
const std::string AMINO_PREFIX_WITHDRAW_STAKE_MESSAGE = "cosmos-sdk/MsgWithdrawDelegationReward";
const std::string AMINO_PREFIX_PUBLIC_KEY = "tendermint/PubKeySecp256k1";

json broadcastJSON(json& jsonObj) {
    json jsonMsgWrapper;

    jsonMsgWrapper["tx"] = jsonObj;
    jsonMsgWrapper["mode"] = "block";

    return jsonMsgWrapper;
}

json wrapperJSON(const std::string& type, json& jsonObj) {
    json jsonMsgWrapper;

    jsonMsgWrapper["type"] = type;
    jsonMsgWrapper["value"] = jsonObj;

    return jsonMsgWrapper;
}

json amountJSON(std::string amount, std::string denom) {
    json jsonAmount;

    jsonAmount["amount"] = amount;
    jsonAmount["denom"] = denom;

    return jsonAmount;
}

json feeJSON(const Fee& fee) {
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

json sendCoinsMessageJSON(json& amounts, std::string from_address, std::string to_address, std::string type_prefix) {
    json jsonMsg;

    jsonMsg["amount"] = amounts;
    jsonMsg["from_address"] = from_address;
    jsonMsg["to_address"] = to_address;

    return wrapperJSON(type_prefix, jsonMsg);
}

json stakeMessageJSON(json& amount, std::string delegator_address, std::string validator_address, std::string type_prefix) {
    json jsonMsg;

    jsonMsg["amount"] = amount;
    jsonMsg["delegator_address"] = delegator_address;
    jsonMsg["validator_address"] = validator_address;

    return wrapperJSON(type_prefix, jsonMsg);
}

json withdrawStakeRewardMessageJSON(std::string delegator_address, std::string validator_address, std::string type_prefix) {
    json jsonMsg;

    jsonMsg["delegator_address"] = delegator_address;
    jsonMsg["validator_address"] = validator_address;

    return wrapperJSON(type_prefix, jsonMsg);
}

json sendCoinsMessageJSON(const SendCoinsMessage& message) {
    json jsonAmounts = json::array();

    for (auto& amount : message.amounts()) {
        jsonAmounts.push_back(amountJSON(std::to_string(amount.amount()), amount.denom()));
    }

    return sendCoinsMessageJSON(jsonAmounts, message.from_address(), message.to_address(), message.type_prefix());
}


json stakeMessageJSON(const StakeMessage& message) {
    auto amount = message.amount();
    json jsonAmount = amountJSON(std::to_string(amount.amount()), amount.denom());

    return stakeMessageJSON(jsonAmount, message.delegator_address(), message.validator_address(), message.type_prefix());
}

json withdrawStakeRewardMessageJSON(const WithdrawStakeRewardMessage& message) {
    return withdrawStakeRewardMessageJSON(message.delegator_address(), message.validator_address(), message.type_prefix());
}

json messageJSON(const SigningInput& input) {
    if (input.has_send_coins_message()) {
        return sendCoinsMessageJSON(input.send_coins_message());
    } else if (input.has_stake_message()) {
        return stakeMessageJSON(input.stake_message());
    } else if (input.has_unstake_message()) {
        return stakeMessageJSON(input.unstake_message());
    } else if (input.has_withdraw_stake_reward_message()) {
        return withdrawStakeRewardMessageJSON(input.withdraw_stake_reward_message());
    }

    return nullptr;
}

json messageJSON(const Transaction& transaction) {
    if (transaction.has_send_coins_message()) {
        return sendCoinsMessageJSON(transaction.send_coins_message());
    } else if (transaction.has_stake_message()) {
        return stakeMessageJSON(transaction.stake_message());
    } else if (transaction.has_unstake_message()) {
        return stakeMessageJSON(transaction.unstake_message());
    } else if (transaction.has_withdraw_stake_reward_message()) {
        return withdrawStakeRewardMessageJSON(transaction.withdraw_stake_reward_message());
    }

    return nullptr;
}

json signatureJSON(const Signature& signature) {
    json jsonSignature;

    jsonSignature["pub_key"]["type"] = AMINO_PREFIX_PUBLIC_KEY;
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

json TW::Cosmos::transactionJSON(const Transaction& transaction, std::string type_prefix) {
    json jsonTx;

    jsonTx["type"] = type_prefix;
    jsonTx["fee"] = feeJSON(transaction.fee());
    jsonTx["memo"] = transaction.memo();
    jsonTx["msg"] = json::array({messageJSON(transaction)});
    jsonTx["signatures"] = json::array({signatureJSON(transaction.signature())});

    return broadcastJSON(jsonTx);
}
