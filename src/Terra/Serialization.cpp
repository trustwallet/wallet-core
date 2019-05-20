#include "Serialization.h"

#include "../Terra/Address.h"
#include "../Base64.h"
#include <TrustWalletCore/TWHRP.h>

using namespace TW;
using namespace TW::TERRA;
using namespace TW::Terra::Proto;

using json = nlohmann::json;


json higherWrapper(json &jsonObj) {
    json jsonMsgWrapper;

    jsonMsgWrapper["tx"] = jsonObj;

    return jsonMsgWrapper;
}

json wrapper(const std::string &type, json &jsonObj) {
    json jsonMsgWrapper;

    jsonMsgWrapper["type"] = type;
    jsonMsgWrapper["value"] = jsonObj;

    return jsonMsgWrapper;
}

json amount_json(std::string amount, std::string denom) {
    json jsonAmount;

    jsonAmount["amount"] = amount;
    jsonAmount["denom"] = denom;

    return jsonAmount;
}

json feeJson(const Fee& fee) {
    json jsonAmounts = json::array();

    for (auto& amount : fee.amounts()) {
        jsonAmounts.push_back(
                amount_json(std::to_string(amount.amount()), amount.denom()));
    }

    json jsonFee;

    jsonFee["amount"] = jsonAmounts;
    jsonFee["gas"] = std::to_string(fee.gas());

    return jsonFee;
}

json sendCoinsMessage(const SendCoinsMessage& message) {
    json jsonMsg;
    auto amount = message.amounts();
    json jsonAmount = amount_json(std::to_string(amount.amount()), amount.denom());

    jsonMsg["amount"] = jsonAmount;
    jsonMsg["from_address"] = message.from_address();
    jsonMsg["to_address"] = message.to_address();

    return wrapper("cosmos-sdk/MsgDelegate", jsonMsg);
}

json stakeMessage(const StakeMessage &message) {
    json jsonMessage;
    auto amount = message.amount();
    json jsonAmount = amount_json(std::to_string(amount.amount()), amount.denom());

    jsonMessage["amount"] = jsonAmount;
    jsonMessage["delegator_address"] = message.delegator_address();
    jsonMessage["validator_address"] = message.validator_address();

    return wrapper("cosmos-sdk/MsgDelegate", jsonMessage);
}

json message(const SigningInput &input) {
    if (input.has_stake_message()) {
        return stakeMessage(input.stake_message());
    } else if(input.has_send_coins_message()){
        return sendCoinsMessage(input.send_coins_message());
    }

    return nullptr;
}

json message(const Transaction &transaction) {
    if (transaction.has_stake_message()) {
        return stakeMessage(transaction.stake_message());
    } else if(transaction.has_send_coins_message()){
        return sendCoinsMessage(transaction.send_coins_message());
    }

    return nullptr;
}

json signature(const Signature &signature) {
    json jsonSignature;

    jsonSignature["pub_key"]["type"] = "tendermint/PubKeySecp256k1";
    jsonSignature["pub_key"]["value"] = Base64::encode(Data(signature.public_key().begin(), signature.public_key().end()));
    jsonSignature["signature"] = Base64::encode(Data(signature.signature().begin(), signature.signature().end()));

    return jsonSignature;
}

json TW::TERRA::signaturePreimage(const SigningInput &input) {
    json jsonForSigning;
    jsonForSigning["chain_id"] = input.chain_id();
    jsonForSigning["fee"] = feeJson(input.fee());
    jsonForSigning["memo"] = input.memo();
    jsonForSigning["msgs"] = json::array({message(input)});

    return jsonForSigning;
}


json TW::TERRA::transactionJson(const Transaction &transaction) {
    json jsonTx;

    jsonTx["type"] = "cosmos-sdk/MsgSend";
    jsonTx["fee"] = feeJson(transaction.fee());
    jsonTx["memo"] = transaction.memo();
    jsonTx["msg"] = json::array({message(transaction)});
    jsonTx["signatures"] = json::array({signature(transaction.signature())});

    return higherWrapper(jsonTx);
}