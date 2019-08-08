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
const std::string COINEX_PREFIX_STAKE_MESSAGE = "bankx/MsgDelegate";
const std::string COINEX_PREFIX_TRANSACTION = "auth/StdTx";
const std::string COINEX_PREFIX_PUBLIC_KEY = "tendermint/PubKeySecp256k1";

json coinexHigherWrapperJSON(json& jsonObj) {
    json jsonMsgWrapper;

    jsonMsgWrapper["tx"] = jsonObj;

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

json coinexSendCoinsMessageJSON(json& amounts, std::string from_address, std::string to_address, std::int64_t unlock_time, std::string type_prefix) {
    json jsonMsg;

    jsonMsg["amount"] = amounts;
    jsonMsg["from_address"] = from_address;
    jsonMsg["to_address"] = to_address;
    jsonMsg["unlock_time"] = std::to_string(unlock_time);

    return coinexWrapperJSON(type_prefix, jsonMsg);
}

json coinexStakeMessageJSON(json& amount, std::string delegator_address, std::string validator_address, std::string type_prefix) {
    json jsonMsg;

    jsonMsg["amount"] = amount;
    jsonMsg["delegator_address"] = delegator_address;
    jsonMsg["validator_address"] = validator_address;

    return coinexWrapperJSON(type_prefix, jsonMsg);
}

json coinexSendCoinsMessageJSON(const Coinex::Proto::SendCoinsMessage& message) {
    json jsonAmounts = json::array();

    for (auto& amount : message.amounts()) {
        jsonAmounts.push_back(coinexAmountJSON(std::to_string(amount.amount()), amount.denom()));
    }

    return coinexSendCoinsMessageJSON(jsonAmounts, message.from_address(), message.to_address(), message.unlock_time(), message.type_prefix());
}

json coinexStakeMessageJSON(const Coinex::Proto::StakeMessage& message) {
    auto amount = message.amount();
    json jsonAmount = coinexAmountJSON(std::to_string(amount.amount()), amount.denom());

    return coinexStakeMessageJSON(jsonAmount, message.delegator_address(), message.validator_address(), message.type_prefix());
}

json coinexMessageJSON(const Coinex::Proto::SigningInput& input) {
    if (input.has_send_coins_message()) {
        return coinexSendCoinsMessageJSON(input.send_coins_message());
    } else if (input.has_stake_message()) {
        return coinexStakeMessageJSON(input.stake_message());
    }

    return nullptr;
}

json coinexMessageJSON(const Coinex::Proto::Transaction& transaction) {
    if (transaction.has_send_coins_message()) {
        return coinexSendCoinsMessageJSON(transaction.send_coins_message());
    } else if (transaction.has_stake_message()) {
        return coinexStakeMessageJSON(transaction.stake_message());
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
