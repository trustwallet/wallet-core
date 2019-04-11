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
using namespace TW::Cosmos;
using namespace TW::Cosmos::Proto;

using json = nlohmann::json;

const static std::string AMINO_PREFIX_SEND_MESSAGE = "cosmos-sdk/MsgSend";
const static std::string AMINO_PREFIX_TRANSACTION = "auth/StdTx";
const static std::string AMINO_PREFIX_PUBLIC_KEY = "tendermint/PubKeySecp256k1";

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
    json jsonFee;

    json jsonAmounts = json::array();
    for (auto& amount : fee.amount()) {
        jsonAmounts.push_back(
            amountJSON(std::to_string(amount.amount()), amount.denom()));
    }

    jsonFee["amount"] = jsonAmounts;
    jsonFee["gas"] = std::to_string(fee.gas());

    return jsonFee;
}

json messageJSON(json& coins, std::string from_address, std::string to_address) {
    json jsonMsg;

    jsonMsg["amount"] = coins;
    jsonMsg["from_address"] = from_address;
    jsonMsg["to_address"] = to_address;

    return wrapperJSON(AMINO_PREFIX_SEND_MESSAGE, jsonMsg);
}

json messageJSON(const SigningInput& input) {
    json jsonMsg;
    
    if (input.has_message()) {
        json jsonCoins = json::array();
        for (auto& coin : input.message().amount()) {
            jsonCoins.push_back(amountJSON(std::to_string(coin.amount()), coin.denom()));
        }

        return messageJSON(jsonCoins, input.message().from_address(), input.message().to_address());
    }

    return nullptr;
}

json messageJSON(const Transaction& transaction) {
    json jsonCoins = json::array();

    for (auto& coin : transaction.message().amount()) {
        jsonCoins.push_back(amountJSON(std::to_string(coin.amount()), coin.denom()));
    }

    return messageJSON(jsonCoins, transaction.message().from_address(), transaction.message().to_address());
}

json signatureJSON(const Signature& signature) {
    json jsonSignature;

    jsonSignature["pub_key"]["type"] = AMINO_PREFIX_PUBLIC_KEY;
    jsonSignature["pub_key"]["value"] = Base64::encode(Data(signature.public_key().begin(), signature.public_key().end()));
    jsonSignature["signature"] = Base64::encode(Data(signature.signature().begin(), signature.signature().end()));

    return jsonSignature;
}

json TW::Cosmos::signaturePreimageJSON(const SigningInput& input) {
    json jsonTx;
    
    jsonTx["account_number"] = std::to_string(input.account_number());
    jsonTx["chain_id"] = input.chain_id();
    jsonTx["fee"] = feeJSON(input.fee());
    jsonTx["memo"] = input.memo();
    jsonTx["msgs"] = json::array({messageJSON(input)});
    jsonTx["sequence"] = std::to_string(input.sequence());

    return jsonTx;
}

json TW::Cosmos::transactionJSON(const Transaction& transaction) {
    json jsonTx;
    
    jsonTx["fee"] = feeJSON(transaction.fee());
    jsonTx["memo"] = transaction.memo();
    jsonTx["msg"] = json::array({messageJSON(transaction)});
    jsonTx["signatures"] = json::array({signatureJSON(transaction.signature())});
    
    return wrapperJSON(AMINO_PREFIX_TRANSACTION, jsonTx);  
}