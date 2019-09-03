// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Serialization.h"

#include "../Cosmos/Address.h"
#include "../Base64.h"
#include <TrustWalletCore/TWHRP.h>

using namespace TW;
using namespace TW::Coinex;
using namespace TW::Coinex::Proto;

using json = nlohmann::json;

const std::string COINEX_PREFIX_SEND_COIN_MESSAGE = "bankx/MsgSend";
// stacking
const std::string COINEX_PREFIX_STAKE_MESSAGE = "cosmos-sdk/MsgDelegate";
const std::string COINEX_PREFIX_UNSTAKE_MESSAGE = "cosmos-sdk/MsgUndelegate";
const std::string COINEX_PREFIX_WITHDRAW_STAKE_MESSAGE = "cosmos-sdk/MsgWithdrawDelegationReward";
// alias
const std::string COINEX_PREFIX_SET_ALIAS_MESSAGE = "alias/MsgAliasUpdate";
// dex market
const std::string COINEX_PREFIX_CREATE_ORDER_MESSAGE = "market/MsgCreateOrder";
const std::string COINEX_PREFIX_CANCEL_ORDER_MESSAGE = "market/MsgCancelOrder";

const std::string COINEX_PREFIX_TRANSACTION = "auth/StdTx";
const std::string COINEX_PREFIX_PUBLIC_KEY = "tendermint/PubKeySecp256k1";

json coinexHigherWrapperJSON(json& jsonObj) {
    json jsonMsgWrapper;

    jsonMsgWrapper["tx"] = jsonObj;
    jsonMsgWrapper["mode"] = "block";

    return jsonMsgWrapper;
}

json coinexWrapperJSON(const std::string& type, json& jsonObj) {
    json jsonMsgWrapper;

    jsonMsgWrapper["type"] = type;
    jsonMsgWrapper["value"] = jsonObj;

    return jsonMsgWrapper;
}

json coinexAmountJSON(std::string amount, std::string denom) {
    json jsonAmount;

    jsonAmount["amount"] = amount;
    jsonAmount["denom"] = denom;

    return jsonAmount;
}

json coinexFeeJSON(const Fee& fee) {
    json jsonAmounts = json::array();

    for (auto& amount : fee.amounts()) {
        jsonAmounts.push_back(
            coinexAmountJSON(std::to_string(amount.amount()), amount.denom()));
    }

    json jsonFee;

    jsonFee["amount"] = jsonAmounts;
    jsonFee["gas"] = std::to_string(fee.gas());

    return jsonFee;
}

// send coin
json coinexSendCoinsMessageJSON(
        json& amounts,
        std::string from_address,
        std::string to_address,
        std::int64_t unlock_time,
        std::string type_prefix) {
    json jsonMsg;

    jsonMsg["amount"] = amounts;
    jsonMsg["from_address"] = from_address;
    jsonMsg["to_address"] = to_address;
    jsonMsg["unlock_time"] = std::to_string(unlock_time);

    return coinexWrapperJSON(type_prefix, jsonMsg);
}

json coinexSendCoinsMessageJSON(const Coinex::Proto::SendCoinsMessage& message) {
    json jsonAmounts = json::array();

    for (auto& amount : message.amounts()) {
        jsonAmounts.push_back(coinexAmountJSON(std::to_string(amount.amount()), amount.denom()));
    }

    return coinexSendCoinsMessageJSON(
            jsonAmounts,
            message.from_address(),
            message.to_address(),
            message.unlock_time(),
            message.type_prefix());
}

// stack
json coinexStakeMessageJSON(
        json& amount,
        std::string delegator_address,
        std::string validator_address,
        std::string type_prefix) {
    json jsonMsg;

    jsonMsg["amount"] = amount;
    jsonMsg["delegator_address"] = delegator_address;
    jsonMsg["validator_address"] = validator_address;

    return coinexWrapperJSON(type_prefix, jsonMsg);
}

json coinexStakeMessageJSON(const Coinex::Proto::StakeMessage& message) {
    auto amount = message.amount();
    json jsonAmount = coinexAmountJSON(std::to_string(amount.amount()), amount.denom());

    return coinexStakeMessageJSON(
            jsonAmount,
            message.delegator_address(),
            message.validator_address(),
            message.type_prefix());
}

// withdraw stake
json coinexWithdrawStakeRewardMessageJSON(
        std::string delegator_address,
        std::string validator_address,
        std::string type_prefix) {
    json jsonMsg;

    jsonMsg["delegator_address"] = delegator_address;
    jsonMsg["validator_address"] = validator_address;

    return coinexWrapperJSON(type_prefix, jsonMsg);
}

json coinexWithdrawStakeRewardMessageJSON(const WithdrawStakeRewardMessage& message) {
    return coinexWithdrawStakeRewardMessageJSON(
            message.delegator_address(),
            message.validator_address(),
            message.type_prefix());
}

// alias
json coinexSetAliaMessageJSON(
        std::string owner,
        std::string alias,
        bool is_add,
        bool as_default,
        std::string type_prefix) {
    json jsonMsg;

    jsonMsg["owner"] = owner;
    jsonMsg["alias"] = alias;
    jsonMsg["is_add"] = is_add;
    jsonMsg["as_default"] = as_default;

    return coinexWrapperJSON(type_prefix, jsonMsg);
}

json coinexSetAliaMessageJSON(const SetAliasMessage& message) {
    return coinexSetAliaMessageJSON(
            message.owner(),
            message.alias(),
            message.is_add(),
            message.as_default(),
            message.type_prefix());
}

// create order
json coinexCreateOrderMessageJSON(
        std::string sender,
        std::string sequence,
        std::string trading_pair,
        std::int64_t order_type,
        std::int64_t price_precision,
        std::string price,
        std::string quantity,
        std::int64_t side,
        std::string time_in_force,
        std::string exist_blocks,
        std::string type_prefix) {
    json jsonMsg;

    jsonMsg["sender"] = sender;
    jsonMsg["sequence"] = sequence;
    jsonMsg["trading_pair"] = trading_pair;
    jsonMsg["order_type"] = order_type;
    jsonMsg["price_precision"] = price_precision;
    jsonMsg["price"] = price;
    jsonMsg["quantity"] = quantity;
    jsonMsg["side"] = side;
    jsonMsg["time_in_force"] = time_in_force;
    jsonMsg["exist_blocks"] = exist_blocks;

    return coinexWrapperJSON(type_prefix, jsonMsg);
}

json coinexCreateOrderMessageJSON(const CreateOrderMessage& message) {
    return coinexCreateOrderMessageJSON(
            message.sender(),
            message.sequence(),
            message.trading_pair(),
            message.order_type(),
            message.price_precision(),
            message.price(),
            message.quantity(),
            message.side(),
            message.time_in_force(),
            message.exist_blocks(),
            message.type_prefix());
}

// cancel order
json coinexCancelOrderMessageJSON(std::string order_id, std::string sender, std::string type_prefix) {
    json jsonMsg;

    jsonMsg["order_id"] = order_id;
    jsonMsg["sender"] = sender;

    return coinexWrapperJSON(type_prefix, jsonMsg);
}

json coinexCancelOrderMessageJSON(const CancelOrderMessage& message) {
    return coinexCancelOrderMessageJSON(message.order_id(), message.sender(), message.type_prefix());
}

//
json coinexMessageJSON(const Coinex::Proto::SigningInput& input) {
    if (input.has_send_coins_message()) {
        return coinexSendCoinsMessageJSON(input.send_coins_message());
    } else if (input.has_stake_message()) {
        return coinexStakeMessageJSON(input.stake_message());
    } else if (input.has_unstake_message()) {
        return coinexStakeMessageJSON(input.unstake_message());
    } else if (input.has_withdraw_stake_reward_message()) {
        return coinexWithdrawStakeRewardMessageJSON(input.withdraw_stake_reward_message());
    } else if (input.has_set_alias_message()) {
        return coinexSetAliaMessageJSON(input.set_alias_message());
    } else if (input.has_create_order_message()) {
        return coinexCreateOrderMessageJSON(input.create_order_message());
    } else if (input.has_cancel_order_message()) {
        return coinexCancelOrderMessageJSON(input.cancel_order_message());
    }


    return nullptr;
}

json coinexMessageJSON(const Coinex::Proto::Transaction& transaction) {
    if (transaction.has_send_coins_message()) {
        return coinexSendCoinsMessageJSON(transaction.send_coins_message());
    } else if (transaction.has_stake_message()) {
        return coinexStakeMessageJSON(transaction.stake_message());
    } else if (transaction.has_unstake_message()) {
        return coinexStakeMessageJSON(transaction.unstake_message());
    } else if (transaction.has_withdraw_stake_reward_message()) {
        return coinexWithdrawStakeRewardMessageJSON(transaction.withdraw_stake_reward_message());
    } else if (transaction.has_set_alias_message()) {
        return coinexSetAliaMessageJSON(transaction.set_alias_message());
    } else if (transaction.has_create_order_message()) {
        return coinexCreateOrderMessageJSON(transaction.create_order_message());
    } else if (transaction.has_cancel_order_message()) {
        return coinexCancelOrderMessageJSON(transaction.cancel_order_message());
    }

    return nullptr;
}

json coinexSignatureJSON(const Coinex::Proto::Signature& signature) {
    json jsonSignature;

    jsonSignature["pub_key"]["type"] = COINEX_PREFIX_PUBLIC_KEY;
    jsonSignature["pub_key"]["value"] = Base64::encode(Data(signature.public_key().begin(), signature.public_key().end()));
    jsonSignature["signature"] = Base64::encode(Data(signature.signature().begin(), signature.signature().end()));

    return jsonSignature;
}

json TW::Coinex::signaturePreimageJSON(const Coinex::Proto::SigningInput& input) {
    json jsonForSigning;

    jsonForSigning["account_number"] = std::to_string(input.account_number());
    jsonForSigning["chain_id"] = input.chain_id();
    jsonForSigning["fee"] = coinexFeeJSON(input.fee());
    jsonForSigning["memo"] = input.memo();
    jsonForSigning["msgs"] = json::array({coinexMessageJSON(input)});
    jsonForSigning["sequence"] = std::to_string(input.sequence());

    return jsonForSigning;
}

json TW::Coinex::transactionJSON(const Coinex::Proto::Transaction& transaction, std::string type_prefix) {
    json jsonTx;

    jsonTx["type"] = type_prefix;
    jsonTx["fee"] = coinexFeeJSON(transaction.fee());
    jsonTx["memo"] = transaction.memo();
    jsonTx["msg"] = json::array({coinexMessageJSON(transaction)});
    jsonTx["signatures"] = json::array({coinexSignatureJSON(transaction.signature())});

    return coinexHigherWrapperJSON(jsonTx);
}
